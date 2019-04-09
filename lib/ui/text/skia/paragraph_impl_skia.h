// Copyright 2019 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "flutter/lib/ui/painting/canvas.h"
#include "flutter/lib/ui/text/paragraph_impl.h"
#include "flutter/lib/ui/text/text_box.h"

namespace blink {

class ParagraphImplSkia : public ParagraphImpl {
 public:
  ~ParagraphImplSkia() override;

  explicit ParagraphImplSkia(std::unique_ptr<SkParagraph> paragraph);

  double width() override;
  double height() override;
  double minIntrinsicWidth() override;
  double maxIntrinsicWidth() override;
  double alphabeticBaseline() override;
  double ideographicBaseline() override;
  bool didExceedMaxLines() override;

  void layout(double width) override;
  void paint(Canvas* canvas, double x, double y) override;

  std::vector<TextBox> getRectsForRange(
      unsigned start,
      unsigned end,
      txt::Paragraph::RectHeightStyle rect_height_style,
      txt::Paragraph::RectWidthStyle rect_width_style) override;
  Dart_Handle getPositionForOffset(double dx, double dy) override;
  Dart_Handle getWordBoundary(unsigned offset) override;

 private:
  std::unique_ptr<SkParagraph> m_paragraph;
  double m_width = -1.0;
};

}

