// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_TEXT_RUN_METRICS_H_
#define FLUTTER_LIB_UI_TEXT_RUN_METRICS_H_

#include "third_party/dart/runtime/include/dart_api.h"
#include "third_party/tonic/converter/dart_converter.h"

namespace flutter {

struct RunMetrics {
  double top;           // extent above baseline
  double ascent;        // distance to reserve above baseline
  double descent;       // distance to reserve below baseline
  double bottom;        // extent below baseline
  double leading;       // distance to add between lines
  double avgCharWidth;  // average character width
  double maxCharWidth;  // maximum character width
  double xMin;          // minimum x
  double xMax;          // maximum x
  double xHeight;       // height of lower-case 'x'
  double capHeight;     // height of an upper-case letter

  // The following properties can be null.
  double underlineThickness;  // underline thickness
  double underlinePosition;   // underline position relative to baseline
  double strikeoutThickness;  // strikeout thickness
  double strikeoutPosition;   // strikeout position relative to baseline

  RunMetrics(double top,
             double ascent,
             double descent,
             double bottom,
             double leading,
             double avgCharWidth,
             double maxCharWidth,
             double xMin,
             double xMax,
             double xHeight,
             double capHeight,

             double underlineThickness,
             double underlinePosition,
             double strikeoutThickness,
             double strikeoutPosition)
      : top(top),
        ascent(ascent),
        descent(descent),
        bottom(bottom),
        leading(leading),
        avgCharWidth(avgCharWidth),
        maxCharWidth(maxCharWidth),
        xMin(xMin),
        xMax(xMax),
        xHeight(xHeight),
        capHeight(capHeight),
        underlineThickness(underlineThickness),
        underlinePosition(underlinePosition),
        strikeoutThickness(strikeoutThickness),
        strikeoutPosition(strikeoutPosition) {}
};

}  // namespace flutter

namespace tonic {

template <>
struct DartConverter<flutter::RunMetrics> {
  static Dart_Handle ToDart(const flutter::RunMetrics& val);
};

template <>
struct DartListFactory<flutter::RunMetrics> {
  static Dart_Handle NewList(intptr_t length);
};

}  // namespace tonic

#endif  // FLUTTER_LIB_UI_TEXT_RUN_METRICS_H_