// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "platform.h"
#include "flutter/fml/logging.h"

namespace flutter {

std::string GetDefaultFontFamily() {
  return "Arial";
}

sk_sp<SkFontMgr> GetDefaultFontManager() {
  FML_LOG(ERROR) << "Default Default" << std::endl;
  return SkFontMgr::RefDefault();
}

}  // namespace txt
