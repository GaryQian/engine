// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <string>
#include "flutter/fml/macros.h"

#include "third_party/skia/include/core/SkFontMgr.h"

namespace flutter {

std::string GetDefaultSkiaFontFamily();

sk_sp<SkFontMgr> GetDefaultSkiaFontManager();

}
