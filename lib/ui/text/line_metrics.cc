// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/text/line_metrics.h"

#include "flutter/fml/logging.h"
#include "third_party/tonic/dart_class_library.h"
#include "third_party/tonic/dart_state.h"
#include "third_party/tonic/logging/dart_error.h"

using namespace flutter;

namespace tonic {

namespace {

Dart_Handle GetLineMetricsType() {
  DartClassLibrary& class_library = DartState::Current()->class_library();
  Dart_Handle type =
      Dart_HandleFromPersistent(class_library.GetClass("ui", "LineMetrics"));
  FML_DCHECK(!LogIfError(type));
  return type;
}

}  // anonymous namespace

Dart_Handle DartConverter<flutter::LineMetrics>::ToDart(
    const flutter::LineMetrics& val) {
  constexpr int argc = 15;
  std::vector<size_t> indexes;
  std::vector<RunMetrics> metrics;
  for (auto it = val.run_metrics.begin(); it != val.run_metrics.end(); ++it) {
    indexes.push_back(it->first);
    metrics.emplace_back(
        // clang-format off
        it->second.font_metrics_.fTop,
        it->second.font_metrics_.fAscent,
        it->second.font_metrics_.fDescent,
        it->second.font_metrics_.fBottom,
        it->second.font_metrics_.fLeading,
        it->second.font_metrics_.fAvgCharWidth,
        it->second.font_metrics_.fMaxCharWidth,
        it->second.font_metrics_.fXMin,
        it->second.font_metrics_.fXMax,
        it->second.font_metrics_.fXHeight,
        it->second.font_metrics_.fCapHeight,
        it->second.font_metrics_.fUnderlineThickness,
        it->second.font_metrics_.fUnderlinePosition,
        it->second.font_metrics_.fStrikeoutThickness,
        it->second.font_metrics_.fStrikeoutPosition);
    // clang-format on
  }

  Dart_Handle argv[argc] = {tonic::ToDart(val.start_index),
                            tonic::ToDart(val.end_index),
                            tonic::ToDart(val.end_excluding_whitespace),
                            tonic::ToDart(val.end_including_newline),
                            tonic::ToDart(val.hard_break),
                            tonic::ToDart(val.ascent),
                            tonic::ToDart(val.descent),
                            tonic::ToDart(val.unscaled_ascent),
                            tonic::ToDart(val.height),
                            tonic::ToDart(val.width),
                            tonic::ToDart(val.left),
                            tonic::ToDart(val.baseline),
                            tonic::ToDart(val.line_number),
                            tonic::ToDart(metrics),
                            tonic::ToDart(indexes)};
  return Dart_New(GetLineMetricsType(), tonic::ToDart("_"), argc, argv);
}

Dart_Handle DartListFactory<flutter::LineMetrics>::NewList(intptr_t length) {
  return Dart_NewListOfType(GetLineMetricsType(), length);
}

}  // namespace tonic
