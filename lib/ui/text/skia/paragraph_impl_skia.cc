// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "paragraph_impl_skia.h"

#include "flutter/common/task_runners.h"
#include "flutter/fml/logging.h"
#include "flutter/fml/task_runner.h"
#include "flutter/lib/ui/text/paragraph.h"
#include "flutter/lib/ui/text/paragraph_impl.h"
#include "third_party/skia/include/core/SkPoint.h"
#include "third_party/tonic/converter/dart_converter.h"
#include "third_party/skia/modules/skparagraph/include/SkDartTypes.h"

using tonic::ToDart;

namespace blink {

ParagraphImplSkia::ParagraphImplSkia(std::unique_ptr<SkParagraph> paragraph)
    : m_paragraph(std::move(paragraph)) {}

ParagraphImplSkia::~ParagraphImplSkia() {}

double ParagraphImplSkia::width() {
  return m_paragraph->getMaxWidth();
}

double ParagraphImplSkia::height() {
  return m_paragraph->getHeight();
}

double ParagraphImplSkia::minIntrinsicWidth() {
  return m_paragraph->getMinIntrinsicWidth();
}

double ParagraphImplSkia::maxIntrinsicWidth() {
  return m_paragraph->getMaxIntrinsicWidth();
}

double ParagraphImplSkia::alphabeticBaseline() {
  return m_paragraph->getAlphabeticBaseline();
}

double ParagraphImplSkia::ideographicBaseline() {
  return m_paragraph->getIdeographicBaseline();
}

bool ParagraphImplSkia::didExceedMaxLines() {
  return m_paragraph->didExceedMaxLines();
}

void ParagraphImplSkia::layout(double width) {
  m_width = width;
  m_paragraph->layout(width);
}

void ParagraphImplSkia::paint(Canvas* canvas, double x, double y) {
  SkCanvas* sk_canvas = canvas->canvas();
  if (!sk_canvas) {
    return;
  }
  m_paragraph->paint(sk_canvas, x, y);
}

std::vector<TextBox> ParagraphImplSkia::getRectsForRange(
    unsigned start,
    unsigned end,
    txt::Paragraph::RectHeightStyle rect_height_style,
    txt::Paragraph::RectWidthStyle rect_width_style) {
  std::vector<TextBox> result;
  auto boxes = m_paragraph->getRectsForRange(
      start, end, (RectHeightStyle)rect_height_style, (RectWidthStyle)rect_width_style);
  for (const auto& box : boxes) {
    result.emplace_back(box.rect,
                        static_cast<blink::TextDirection>(box.direction));
  }
  return result;
}

Dart_Handle ParagraphImplSkia::getPositionForOffset(double dx, double dy) {
  Dart_Handle result = Dart_NewListOf(Dart_CoreType_Int, 2);
  auto pos = m_paragraph->getGlyphPositionAtCoordinate(dx, dy);
  Dart_ListSetAt(result, 0, ToDart(pos.position));
  Dart_ListSetAt(result, 1, ToDart(static_cast<int>(pos.affinity)));
  return result;
}

Dart_Handle ParagraphImplSkia::getWordBoundary(unsigned offset) {
  auto point = m_paragraph->getWordBoundary(offset);
  Dart_Handle result = Dart_NewListOf(Dart_CoreType_Int, 2);
  Dart_ListSetAt(result, 0, ToDart(point.start));
  Dart_ListSetAt(result, 1, ToDart(point.end));
  return result;
}

}
