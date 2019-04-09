// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "platform.h"
#include "flutter/fml/logging.h"

#include "SkTypes.h"
#include "SkFontMgr.h"
#include "SkFontMgr_android.h"

namespace blink {

std::string GetDefaultSkiaFontFamily() {
  return "sans-serif";
}

sk_sp<SkFontMgr> GetDefaultSkiaFontManager() {

  return SkFontMgr_New_Android(NULL);
  //return SkFontMgr_New_Custom_Directory("/usr/share/fonts/");
}

}
