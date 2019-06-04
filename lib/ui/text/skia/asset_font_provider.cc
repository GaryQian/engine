// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <algorithm>
#include "asset_font_provider.h"

#include "flutter/fml/logging.h"
#include "third_party/skia/include/core/SkData.h"
#include "third_party/skia/include/core/SkStream.h"
#include "third_party/skia/include/core/SkString.h"
#include "third_party/skia/include/core/SkTypeface.h"

namespace blink {

namespace {

void MappingReleaseProc(const void* ptr, void* context) {
  delete reinterpret_cast<fml::Mapping*>(context);
}

std::string CanonicalFamilyName(std::string family_name) {
  std::string result(family_name.length(), 0);

  // Convert ASCII characters to lower case.
  std::transform(family_name.begin(), family_name.end(), result.begin(),
                 [](char c) { return (c & 0x80) ? c : ::tolower(c); });

  return result;
}

}  // anonymous namespace

AssetFontProvider::AssetFontProvider(
    std::shared_ptr<blink::AssetManager> asset_manager)
    : asset_manager_(asset_manager) {}

AssetFontProvider::~AssetFontProvider() = default;

int AssetFontProvider::onCountFamilies() const {
  return family_names_.size();
}

void AssetFontProvider::onGetFamilyName(int index, SkString* familyName) const {
  FML_DCHECK(index >= 0 && static_cast<size_t>(index) < family_names_.size());
  familyName->set(family_names_[index].c_str());
}

SkFontStyleSet* AssetFontProvider::onMatchFamily(const char familyName[]) const {
  auto found = registered_families_.find(CanonicalFamilyName(familyName));
  if (found == registered_families_.end()) {
    return nullptr;
  }
  return SkRef(const_cast<AssetFontStyleSet*>(&found->second));
}

void AssetFontProvider::RegisterAsset(std::string family_name, std::string asset) {
  std::string canonical_name = CanonicalFamilyName(family_name);
  auto family_it = registered_families_.find(canonical_name);

  if (family_it == registered_families_.end()) {
    family_names_.push_back(family_name);
    family_it = registered_families_
        .emplace(std::piecewise_construct,
                 std::forward_as_tuple(canonical_name),
                 std::forward_as_tuple(asset_manager_))
        .first;
  }

  family_it->second.registerAsset(asset);
}

AssetFontStyleSet::AssetFontStyleSet(
    std::shared_ptr<blink::AssetManager> asset_manager)
    : asset_manager_(asset_manager) {}

AssetFontStyleSet::~AssetFontStyleSet() = default;

void AssetFontStyleSet::registerAsset(std::string asset) {
  assets_.emplace_back(asset);
}

int AssetFontStyleSet::count() {
  return assets_.size();
}

void AssetFontStyleSet::getStyle(int index,
                                 SkFontStyle*,
                                 SkString* style) {
  FML_DCHECK(false);
}

SkTypeface* AssetFontStyleSet::createTypeface(int i) {
  size_t index = i;
  if (index >= assets_.size())
    return nullptr;

  TypefaceAsset& asset = assets_[index];
  if (!asset.typeface) {
    std::unique_ptr<fml::Mapping> asset_mapping = asset_manager_->GetAsMapping(asset.asset);
    if (asset_mapping == nullptr) {
      return nullptr;
    }

    fml::Mapping* asset_mapping_ptr = asset_mapping.release();
    sk_sp<SkData> asset_data = SkData::MakeWithProc(
        asset_mapping_ptr->GetMapping(), asset_mapping_ptr->GetSize(),
        MappingReleaseProc, asset_mapping_ptr);
    std::unique_ptr<SkMemoryStream> stream = SkMemoryStream::Make(asset_data);

    // Ownership of the stream is transferred.
    asset.typeface = SkTypeface::MakeFromStream(std::move(stream));
    if (!asset.typeface) {
      return nullptr;
    }
  }

  return SkRef(asset.typeface.get());
}

SkTypeface* AssetFontStyleSet::matchStyle(const SkFontStyle& pattern) {
  if (assets_.empty()) {
    return nullptr;
  }

  for (const TypefaceAsset& asset : assets_) {

    if (asset.typeface && asset.typeface->fontStyle() == pattern) {
      return SkRef(asset.typeface.get());
    }
  }

  return SkRef(assets_[0].typeface.get());
}

AssetFontStyleSet::TypefaceAsset::TypefaceAsset(std::string a)
    : asset(std::move(a)) {}

AssetFontStyleSet::TypefaceAsset::TypefaceAsset(
    const AssetFontStyleSet::TypefaceAsset& other) = default;

AssetFontStyleSet::TypefaceAsset::~TypefaceAsset() = default;

}  // namespace blink