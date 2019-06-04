// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "flutter/assets/asset_manager.h"
#include "flutter/fml/macros.h"
#include "third_party/skia/include/core/SkFontMgr.h"
#include "third_party/skia/include/core/SkTypeface.h"

namespace flutter {

class AssetFontStyleSet : public SkFontStyleSet {
 public:
  AssetFontStyleSet(std::shared_ptr<AssetManager> asset_manager);

  ~AssetFontStyleSet() override;

  void registerAsset(std::string asset);

  int count() override;

  void getStyle(int index, SkFontStyle*, SkString* style) override;

  SkTypeface* createTypeface(int index) override;

  SkTypeface* matchStyle(const SkFontStyle& pattern) override;

 private:
  std::shared_ptr<AssetManager> asset_manager_;

  struct TypefaceAsset {
    TypefaceAsset(std::string a);

    TypefaceAsset(const TypefaceAsset& other);

    ~TypefaceAsset();

    std::string asset;
    sk_sp<SkTypeface> typeface;
  };
  std::vector<TypefaceAsset> assets_;

  FML_DISALLOW_COPY_AND_ASSIGN(AssetFontStyleSet);
};

class AssetFontProvider : public SkFontMgr {
 public:
  AssetFontProvider(std::shared_ptr<AssetManager> asset_manager);

  ~AssetFontProvider() override;

  void RegisterAsset(std::string family_name, std::string asset);

  int onCountFamilies() const override;

  void onGetFamilyName(int index, SkString* familyName) const override;

  SkFontStyleSet* onMatchFamily(const char familyName[]) const override;

  SkFontStyleSet* onCreateStyleSet(int index) const override { return nullptr; }
  SkTypeface* onMatchFamilyStyle(const char familyName[],
                                 const SkFontStyle& style) const override { return nullptr; }

  SkTypeface* onMatchFamilyStyleCharacter(const char familyName[],
                                          const SkFontStyle& style,
                                          const char* bcp47[], int bcp47Count,
                                          SkUnichar character) const override { return nullptr; }
  SkTypeface* onMatchFaceStyle(const SkTypeface* tf,
                               const SkFontStyle& style) const override { return nullptr; }
  sk_sp<SkTypeface> onMakeFromData(sk_sp<SkData>, int ttcIndex) const override { return nullptr; }
  sk_sp<SkTypeface> onMakeFromStreamIndex(std::unique_ptr<SkStreamAsset>,
                                          int ttcIndex) const override { return nullptr; }
  sk_sp<SkTypeface> onMakeFromStreamArgs(std::unique_ptr<SkStreamAsset>,
                                         const SkFontArguments&) const override { return nullptr; }
  sk_sp<SkTypeface> onMakeFromFontData(std::unique_ptr<SkFontData>) const override { return nullptr; }
  sk_sp<SkTypeface> onMakeFromFile(const char path[], int ttcIndex) const override { return nullptr; }
  sk_sp<SkTypeface> onLegacyMakeTypeface(const char familyName[],
                                         SkFontStyle style) const override { return nullptr; }

 private:
  std::shared_ptr<AssetManager> asset_manager_;
  std::unordered_map<std::string, AssetFontStyleSet> registered_families_;
  std::vector<std::string> family_names_;

  FML_DISALLOW_COPY_AND_ASSIGN(AssetFontProvider);
};

}  // namespace flutter
