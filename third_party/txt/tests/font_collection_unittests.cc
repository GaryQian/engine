/*
 * Copyright 2017 Google, Inc.
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

#include "flutter/fml/command_line.h"
#include "flutter/fml/logging.h"
#include "gtest/gtest.h"
#include "txt/font_collection.h"
#include "txt_test_utils.h"

namespace txt {

TEST(FontCollectionTest, MAC_ONLY(FallbackCacheOrder)) {
  auto collection = GetTestFontCollection();

  std::string locale = "";
  // Chinese 置 can be trad or simp.
  std::shared_ptr<minikin::FontFamily> ff =
      collection->DoMatchFallbackFont(32622, locale);

  ASSERT_EQ(collection->fallback_fonts_for_locale_[locale].size(), 1ull);
  ASSERT_EQ(collection->fallback_fonts_for_locale_[locale][0], "hello");

  // Korean 한
  collection->MatchFallbackFont(54620, locale);
  ASSERT_EQ(collection->fallback_fonts_for_locale_[locale].size(), 2ull);
  ASSERT_EQ(collection->fallback_fonts_for_locale_[locale][0], "hello");
  ASSERT_EQ(collection->fallback_fonts_for_locale_[locale][1], "hello");
}

}  // namespace txt
