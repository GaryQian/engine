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

#include <algorithm>
#include "typeface_font_provider.h"

#include "flutter/fml/logging.h"
#include "third_party/skia/include/core/SkString.h"
#include "third_party/skia/include/core/SkTypeface.h"

namespace blink {

namespace {

std::string CanonicalFamilyName(std::string family_name) {
  std::string result(family_name.length(), 0);

  // Convert ASCII characters to lower case.
  std::transform(family_name.begin(), family_name.end(), result.begin(),
                 [](char c) { return (c & 0x80) ? c : ::tolower(c); });

  return result;
}

}
TypefaceFontProvider::TypefaceFontProvider() = default;

TypefaceFontProvider::~TypefaceFontProvider() = default;

int TypefaceFontProvider::onCountFamilies() const {
  return family_names_.size();
}

void TypefaceFontProvider::onGetFamilyName(int index, SkString* familyName) const {
  familyName->set(family_names_[index].c_str());
}

SkFontStyleSet* TypefaceFontProvider::onMatchFamily(const char familyName[]) const {
  auto found = registered_families_.find(CanonicalFamilyName(familyName));
  if (found == registered_families_.end()) {
    return nullptr;
  }
  return SkRef(const_cast<TypefaceFontStyleSet*>(&found->second));
}

void TypefaceFontProvider::RegisterTypeface(sk_sp<SkTypeface> typeface) {
  if (typeface == nullptr) {
    return;
  }

  SkString sk_family_name;
  typeface->getFamilyName(&sk_family_name);

  std::string family_name(sk_family_name.c_str(), sk_family_name.size());
  RegisterTypeface(std::move(typeface), std::move(family_name));
}

void TypefaceFontProvider::RegisterTypeface(
    sk_sp<SkTypeface> typeface,
    std::string family_name_alias) {
  if (family_name_alias.empty()) {
    return;
  }

  std::string canonical_name = CanonicalFamilyName(family_name_alias);
  auto family_it = registered_families_.find(canonical_name);
  if (family_it == registered_families_.end()) {
    family_names_.push_back(family_name_alias);
    family_it = registered_families_
        .emplace(std::piecewise_construct,
                 std::forward_as_tuple(canonical_name),
                 std::forward_as_tuple())
        .first;
  }
  family_it->second.registerTypeface(std::move(typeface));
}

TypefaceFontStyleSet::TypefaceFontStyleSet() = default;

TypefaceFontStyleSet::~TypefaceFontStyleSet() = default;

void TypefaceFontStyleSet::registerTypeface(sk_sp<SkTypeface> typeface) {
  if (typeface == nullptr) {
    return;
  }
  typefaces_.emplace_back(std::move(typeface));
}

int TypefaceFontStyleSet::count() {
  return typefaces_.size();
}

void TypefaceFontStyleSet::getStyle(int index, SkFontStyle*, SkString* style) {
  FML_DCHECK(false);
}

SkTypeface* TypefaceFontStyleSet::createTypeface(int i) {
  size_t index = i;
  if (index >= typefaces_.size()) {
    return nullptr;
  }
  return SkRef(typefaces_[index].get());
}

SkTypeface* TypefaceFontStyleSet::matchStyle(const SkFontStyle& pattern) {
  if (typefaces_.empty())
    return nullptr;

  for (const sk_sp<SkTypeface>& typeface : typefaces_)
    if (typeface->fontStyle() == pattern)
      return SkRef(typeface.get());

  return SkRef(typefaces_[0].get());
}

}