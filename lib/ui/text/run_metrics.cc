// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/text/run_metrics.h"

#include "flutter/fml/logging.h"
#include "third_party/tonic/dart_class_library.h"
#include "third_party/tonic/dart_state.h"
#include "third_party/tonic/logging/dart_error.h"

using namespace flutter;

namespace tonic {

namespace {

Dart_Handle GetRunMetricsType() {
  DartClassLibrary& class_library = DartState::Current()->class_library();
  Dart_Handle type =
      Dart_HandleFromPersistent(class_library.GetClass("ui", "RunMetrics"));
  FML_DCHECK(!LogIfError(type));
  return type;
}

}  // anonymous namespace

Dart_Handle DartConverter<flutter::RunMetrics>::ToDart(
    const flutter::RunMetrics& val) {
  constexpr int argc = 15;
  Dart_Handle argv[argc] = {
      tonic::ToDart(val.top),
      tonic::ToDart(val.ascent),
      tonic::ToDart(val.descent),
      tonic::ToDart(val.bottom),
      tonic::ToDart(val.leading),
      tonic::ToDart(val.avgCharWidth),
      tonic::ToDart(val.maxCharWidth),
      tonic::ToDart(val.xMin),
      tonic::ToDart(val.xMax),
      tonic::ToDart(val.xHeight),
      tonic::ToDart(val.capHeight),
      tonic::ToDart(val.underlineThickness),
      tonic::ToDart(val.underlinePosition),
      tonic::ToDart(val.strikeoutThickness),
      tonic::ToDart(val.strikeoutPosition),
  };
  return Dart_New(GetRunMetricsType(), tonic::ToDart("_"), argc, argv);
}

Dart_Handle DartListFactory<flutter::RunMetrics>::NewList(intptr_t length) {
  return Dart_NewListOfType(GetRunMetricsType(), length);
}

}  // namespace tonic
