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

namespace flutter {
std::unique_ptr<FontCollectionImpl> FontCollectionImpl::create(bool skiaShaperEnabled) {
  if (skiaShaperEnabled) {
    return std::make_unique<FontCollectionImplSkia>();
  } else {
    return std::make_unique<FontCollectionImplTxt>();
  }
}
}
