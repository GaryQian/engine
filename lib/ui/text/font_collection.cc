// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/text/font_collection.h"

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
#include "third_party/tonic/typed_data/typed_list.h"
#include "txt/asset_font_manager.h"
#include "txt/test_font_manager.h"

#include "flutter/lib/ui/text/skia/asset_font_provider.h"

#include "flutter/lib/ui/text/skia/font_collection_impl_skia.h"
#include "flutter/lib/ui/text/txt/font_collection_impl_txt.h"

namespace {

void LoadFontFromList(tonic::Uint8List& font_data,
                      Dart_Handle callback,
                      std::string family_name) {
  flutter::FontCollection& font_collection =
      flutter::UIDartState::Current()->window()->client()->GetFontCollection();
  font_collection.LoadFontFromList(
      font_data.data(),
      font_data.num_elements(),

      family_name);
  font_data.Release();
  tonic::DartInvoke(callback, {tonic::ToDart(0)});
}

void _LoadFontFromList(Dart_NativeArguments args) {
  tonic::DartCallStatic(LoadFontFromList, args);
}

}

namespace flutter {

void FontCollection::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register({
      {"loadFontFromList", _LoadFontFromList, 3, true},
  });
}

void FontCollection::RegisterFonts(std::shared_ptr<AssetManager> asset_manager) {
  m_fontCollectionImpl->RegisterFonts(asset_manager);
}

void FontCollection::RegisterTestFonts() {
  m_fontCollectionImpl->RegisterTestFonts();
}

void FontCollection::LoadFontFromList(const uint8_t* font_data,
                      int length,
                      std::string family_name) {
  m_fontCollectionImpl->LoadFontFromList(font_data, length, family_name);
}
}  // namespace flutter
