/*
 * Copyright 2018 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "flutter/fml/macros.h"
#include "third_party/skia/include/core/SkFontMgr.h"

namespace blink {

class TypefaceFontStyleSet : public SkFontStyleSet {
 public:
  TypefaceFontStyleSet();

  ~TypefaceFontStyleSet() override;

  void registerTypeface(sk_sp<SkTypeface> typeface);

  int count() override;

  void getStyle(int index, SkFontStyle*, SkString* style) override;

  SkTypeface* createTypeface(int index) override;

  SkTypeface* matchStyle(const SkFontStyle& pattern) override;

 private:
  std::vector<sk_sp<SkTypeface>> typefaces_;

  FML_DISALLOW_COPY_AND_ASSIGN(TypefaceFontStyleSet);
};

class TypefaceFontProvider : public SkFontMgr {
 public:
  TypefaceFontProvider();
  ~TypefaceFontProvider() override;

  void RegisterTypeface(sk_sp<SkTypeface> typeface);

  void RegisterTypeface(sk_sp<SkTypeface> typeface, std::string family_name_alias);

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
  std::unordered_map<std::string, TypefaceFontStyleSet> registered_families_;
  std::vector<std::string> family_names_;

  FML_DISALLOW_COPY_AND_ASSIGN(TypefaceFontProvider);
};

}
