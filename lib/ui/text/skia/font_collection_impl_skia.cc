// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/text/skia/font_collection_impl_skia.h"

#include <mutex>

#include "flutter/lib/ui/text/txt/asset_manager_font_provider.h"
#include "flutter/lib/ui/ui_dart_state.h"
#include "flutter/lib/ui/window/window.h"
#include "flutter/runtime/test_font_data.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "third_party/skia/include/core/SkFontMgr.h"
#include "third_party/skia/include/core/SkGraphics.h"
#include "third_party/skia/include/core/SkStream.h"
#include "third_party/skia/include/core/SkTypeface.h"
#include "third_party/tonic/dart_args.h"
#include "third_party/tonic/dart_library_natives.h"
#include "third_party/tonic/logging/dart_invoke.h"
#include "third_party/tonic/typed_data/uint8_list.h"
#include "txt/asset_font_manager.h"
#include "txt/test_font_manager.h"
#include "platform.h"

namespace blink {

FontCollectionImplSkia::FontCollectionImplSkia()
    : font_collection_(sk_make_sp<SkFontCollection>())
    , dynamic_font_manager_(sk_make_sp<TypefaceFontProvider>()) {

    font_collection_->setDynamicFontManager(dynamic_font_manager_);
    font_collection_->setDefaultFontManager(GetDefaultSkiaFontManager(), GetDefaultSkiaFontFamily());
}

FontCollectionImplSkia::~FontCollectionImplSkia() {
  font_collection_.reset();
  SkGraphics::PurgeFontCache();
}

void FontCollectionImplSkia::RegisterFonts(
    std::shared_ptr<AssetManager> asset_manager) {

  std::unique_ptr<fml::Mapping> manifest_mapping =
      asset_manager->GetAsMapping("FontManifest.json");
  if (manifest_mapping == nullptr) {
    FML_DLOG(WARNING) << "Could not find the font manifest in the asset store.";
    return;
  }

  rapidjson::Document document;
  static_assert(sizeof(decltype(document)::Ch) == sizeof(uint8_t), "");
  document.Parse(reinterpret_cast<const decltype(document)::Ch*>(
                     manifest_mapping->GetMapping()),
                 manifest_mapping->GetSize());

  if (document.HasParseError()) {
    FML_DLOG(WARNING) << "Error parsing the font manifest in the asset store.";
    return;
  }

  // Structure described in https://flutter.io/custom-fonts/

  if (!document.IsArray()) {
    return;
  }

  auto font_provider = sk_make_sp<AssetFontProvider>(asset_manager);

  for (const auto& family : document.GetArray()) {
    auto family_name = family.FindMember("family");
    if (family_name == family.MemberEnd() || !family_name->value.IsString()) {
      continue;
    }

    auto family_fonts = family.FindMember("fonts");
    if (family_fonts == family.MemberEnd() || !family_fonts->value.IsArray()) {
      continue;
    }

    for (const auto& family_font : family_fonts->value.GetArray()) {
      if (!family_font.IsObject()) {
        continue;
      }

      auto font_asset = family_font.FindMember("asset");
      if (font_asset == family_font.MemberEnd() ||
          !font_asset->value.IsString()) {
        continue;
      }

      // TODO: Handle weights and styles.
      font_provider->RegisterAsset(family_name->value.GetString(),
                                   font_asset->value.GetString());
    }
  }

  font_collection_->setAssetFontManager(std::move(font_provider));
}

void FontCollectionImplSkia::RegisterTestFonts() {

  sk_sp<SkTypeface> test_typeface =
      SkTypeface::MakeFromStream(GetTestFontData());

  auto provider = sk_make_sp<TypefaceFontProvider>();
  provider->RegisterTypeface(std::move(test_typeface), GetTestFontFamilyName());
  font_collection_->setTestFontManager(provider);
  font_collection_->disableFontFallback();
}

void FontCollectionImplSkia::LoadFontFromList(
    const uint8_t* font_data,
    int length,
    std::string family_name) {

  std::unique_ptr<SkStreamAsset> font_stream =
      std::make_unique<SkMemoryStream>(font_data, length, true);
  sk_sp<SkTypeface> typeface =
      SkTypeface::MakeFromStream(std::move(font_stream));

    auto provider = static_cast<TypefaceFontProvider*>(dynamic_font_manager_.get());
    if (family_name.empty()) {
      provider->RegisterTypeface(typeface);
    } else {
      provider->RegisterTypeface(typeface, family_name);
    }
    // TODO: skia_collection_->ClearFontFamilyCache();
}

}
