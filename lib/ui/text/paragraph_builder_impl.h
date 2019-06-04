// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>
#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/lib/ui/painting/paint.h"
#include "flutter/lib/ui/text/paragraph.h"
#include "flutter/third_party/txt/src/txt/paragraph_builder.h"
#include "third_party/skia/modules/skparagraph/include/ParagraphBuilder.h"
#include "third_party/tonic/typed_data/int32_list.h"
#include "flutter/lib/ui/text/font_collection.h"

namespace blink {

class Paragraph;

class ParagraphBuilderImpl {

 public:

  ParagraphBuilderImpl() = default;

  virtual ~ParagraphBuilderImpl() = default;

  virtual void pushStyle(const txt::TextStyle& style) = 0;

  virtual txt::TextStyle peekStyle() const = 0;

  virtual void pop() = 0;

  virtual bool addText(const std::u16string& text) = 0;

  virtual fml::RefPtr<Paragraph> build() = 0;
};

}

