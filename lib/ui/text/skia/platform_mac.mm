// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TargetConditionals.h>
#include "flutter/fml/platform/darwin/platform_version.h"
#include "platform.h"
#include "flutter/fml/logging.h"

#if TARGET_OS_EMBEDDED || TARGET_OS_SIMULATOR
#include <UIKit/UIKit.h>
#define FONT_CLASS UIFont
#else  // TARGET_OS_EMBEDDED
#include <AppKit/AppKit.h>
#define FONT_CLASS NSFont
#endif  // TARGET_OS_EMBEDDED

namespace flutter {

std::string GetDefaultSkiaFontFamily() {
  if (fml::IsPlatformVersionAtLeast(9)) {
    return [FONT_CLASS systemFontOfSize:14].familyName.UTF8String;
  } else {
    return "Helvetica";
  }
}

sk_sp<SkFontMgr> GetDefaultSkiaFontManager() {
  return SkFontMgr::RefDefault();
}

}
