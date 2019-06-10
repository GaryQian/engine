// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "platform.h"
#include "flutter/fml/logging.h"

#include "include/core/SkTypes.h"
#include "include/core/SkFontMgr.h"
#include "include/ports/SkFontMgr_android.h"

namespace flutter {

std::string GetDefaultSkiaFontFamily() {
  return "sans-serif";
}

sk_sp<SkFontMgr> GetDefaultSkiaFontManager() {
  FML_LOG(ERROR) << "Default Android" << std::endl;
  return SkFontMgr::RefDefault();
  //return SkFontMgr_New_Android(NULL);
  //return SkFontMgr_New_Custom_Directory("/usr/share/fonts/");
}

}
