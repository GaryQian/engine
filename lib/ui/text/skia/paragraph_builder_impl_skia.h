// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>
#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/lib/ui/painting/paint.h"
#include "flutter/lib/ui/text/paragraph.h"
#include "flutter/lib/ui/text/paragraph_builder_impl.h"
#include "third_party/skia/modules/skparagraph/src/ParagraphBuilderImpl.h"
#include "third_party/tonic/typed_data/typed_list.h"

namespace flutter {

class ParagraphBuilderImplSkia : public ParagraphBuilderImpl {

 public:

  ParagraphBuilderImplSkia(const txt::ParagraphStyle& style, sk_sp<skia::textlayout::FontCollection> fontCollection);

  ~ParagraphBuilderImplSkia() = default;

  void pushStyle(const txt::TextStyle& style) override;

  txt::TextStyle peekStyle() const override;

  void pop() override;

  bool addText(const std::u16string& text) override;

  fml::RefPtr<Paragraph> build() override;

 private:
  std::shared_ptr<skia::textlayout::ParagraphBuilderImpl> m_builder;

};

}

