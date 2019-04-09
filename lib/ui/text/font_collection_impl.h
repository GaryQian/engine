// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>
#include <vector>

#include "third_party/skia/modules/skparagraph/include/SkFontCollection.h"
#include "txt/font_collection.h"
#include "flutter/lib/ui/text/font_collection_impl.h"
#include "flutter/assets/asset_manager.h"
#include "flutter/fml/macros.h"
#include "flutter/fml/memory/ref_ptr.h"

namespace tonic {
class DartLibraryNatives;
}

namespace blink {

class FontCollectionImpl {
 public:

  virtual ~FontCollectionImpl() = default;

  virtual void RegisterFonts(std::shared_ptr<AssetManager> asset_manager) = 0;

  virtual void RegisterTestFonts() = 0;

  virtual void LoadFontFromList(const uint8_t* font_data,
                                int length,
                                std::string family_name) = 0;

  virtual sk_sp<SkFontCollection> getSkiaFontCollection() { return nullptr; }

  virtual std::shared_ptr<txt::FontCollection> getTxtFontCollection() { return nullptr; }

  static std::unique_ptr<FontCollectionImpl> create(bool skiaShaperEnabled);
};

}

