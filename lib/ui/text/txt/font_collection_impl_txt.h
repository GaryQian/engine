// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>
#include <vector>

#include "flutter/lib/ui/text/font_collection_impl.h"
#include "flutter/assets/asset_manager.h"
#include "flutter/fml/macros.h"
#include "flutter/fml/memory/ref_ptr.h"
#include "txt/font_collection.h"

namespace tonic {
class DartLibraryNatives;
}

namespace flutter {

class FontCollectionImplTxt : public FontCollectionImpl {
 public:
  FontCollectionImplTxt();

  ~FontCollectionImplTxt() override;

  void RegisterFonts(std::shared_ptr<flutter::AssetManager> asset_manager) override;

  void RegisterTestFonts() override;

  void LoadFontFromList(const uint8_t* font_data,
                        int length,
                        std::string family_name) override;

  std::shared_ptr<txt::FontCollection> getTxtFontCollection() override { return font_collection_; }

 private:
  std::shared_ptr<txt::FontCollection> font_collection_;
  sk_sp<txt::DynamicFontManager> dynamic_font_manager_;
};

}

