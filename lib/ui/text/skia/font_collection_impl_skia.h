// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>
#include <vector>

#include "flutter/lib/ui/text/font_collection_impl.h"
#include "typeface_font_provider.h"
#include "asset_font_provider.h"
#include "flutter/assets/asset_manager.h"
#include "flutter/fml/macros.h"
#include "flutter/fml/memory/ref_ptr.h"

namespace tonic {
class DartLibraryNatives;
}

namespace blink {

class FontCollectionImplSkia : public FontCollectionImpl {
 public:
  FontCollectionImplSkia();

  ~FontCollectionImplSkia() override;

  void RegisterFonts(std::shared_ptr<AssetManager> asset_manager) override;

  void RegisterTestFonts() override;

  void LoadFontFromList(const uint8_t* font_data,
                        int length,
                        std::string family_name) override;

  sk_sp<SkFontCollection> getSkiaFontCollection() override { return font_collection_; }

 private:
  sk_sp<SkFontCollection> font_collection_;
  sk_sp<TypefaceFontProvider> dynamic_font_manager_;
};

}  // namespace blink

