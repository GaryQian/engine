// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_TEXT_FONT_COLLECTION_H_
#define FLUTTER_LIB_UI_TEXT_FONT_COLLECTION_H_

#include <memory>
#include <vector>
#include <flutter/lib/ui/text/skia/font_collection_impl_skia.h>

#include "third_party/skia/modules/skparagraph/include/FontCollection.h"
#include "txt/font_collection.h"
#include "flutter/lib/ui/text/font_collection_impl.h"
#include "flutter/assets/asset_manager.h"
#include "flutter/fml/macros.h"
#include "flutter/fml/memory/ref_ptr.h"

namespace tonic {
class DartLibraryNatives;
} // namespace tonic

namespace flutter {

class FontCollection {
 public:

  FontCollection() {
     m_fontCollectionImpl = nullptr;
  }

  void SetImplementation(bool skia) {
    m_skia = skia;
    m_fontCollectionImpl = skia
        ? flutter::FontCollectionImpl::create(true)
        : flutter::FontCollectionImpl::create(false);
  }

  ~FontCollection() = default;

  void RegisterFonts(std::shared_ptr<AssetManager> asset_manager);

  void RegisterTestFonts();

  void LoadFontFromList(const uint8_t* font_data,
                        int length,
                        std::string family_name);

  sk_sp<skia::textlayout::FontCollection> getSkiaFontCollection() {
    return m_fontCollectionImpl->getSkiaFontCollection();
  }

  std::shared_ptr<txt::FontCollection> getTxtFontCollection() {
    return m_fontCollectionImpl->getTxtFontCollection();
  }

  bool skiaShaperEnabled() { return m_skia; }

  static void RegisterNatives(tonic::DartLibraryNatives* natives);

 private:

  std::unique_ptr<flutter::FontCollectionImpl> m_fontCollectionImpl;
  bool m_skia;

  FML_DISALLOW_COPY_AND_ASSIGN(FontCollection);
};

}  // namespace flutter

#endif  // FLUTTER_LIB_UI_TEXT_FONT_COLLECTION_H_
