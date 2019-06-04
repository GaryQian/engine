// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/text/txt/paragraph_builder_impl_txt.h"

#include "flutter/common/settings.h"
#include "flutter/common/task_runners.h"
#include "flutter/fml/logging.h"
#include "flutter/fml/task_runner.h"
#include "flutter/lib/ui/text/font_collection.h"
#include "flutter/lib/ui/ui_dart_state.h"
#include "flutter/lib/ui/window/window.h"
#include "flutter/third_party/txt/src/txt/font_style.h"
#include "flutter/third_party/txt/src/txt/font_weight.h"
#include "flutter/third_party/txt/src/txt/paragraph_style.h"
#include "flutter/third_party/txt/src/txt/text_decoration.h"
#include "flutter/third_party/txt/src/txt/text_style.h"
#include "third_party/icu/source/common/unicode/ustring.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/tonic/converter/dart_converter.h"
#include "third_party/tonic/dart_args.h"
#include "third_party/tonic/dart_binding_macros.h"
#include "third_party/tonic/dart_library_natives.h"
#include "third_party/tonic/typed_data/dart_byte_data.h"

namespace flutter {

ParagraphBuilderImplTxt::ParagraphBuilderImplTxt(
    const txt::ParagraphStyle& style, std::shared_ptr<txt::FontCollection> fontCollection)
  : m_paragraphBuilder(std::make_unique<txt::ParagraphBuilder>(style, fontCollection)) {
}

void ParagraphBuilderImplTxt::pushStyle(const txt::TextStyle& textStyle) {
  m_paragraphBuilder->PushStyle(textStyle);
}

void ParagraphBuilderImplTxt::pop() {
  m_paragraphBuilder->Pop();
}

txt::TextStyle ParagraphBuilderImplTxt::peekStyle() const {
  return m_paragraphBuilder->PeekStyle();
}

bool ParagraphBuilderImplTxt::addText(const std::u16string& text) {

  if (text.empty())
    return true;

  // Use ICU to validate the UTF-16 input.  Calling u_strToUTF8 with a null
  // output buffer will return U_BUFFER_OVERFLOW_ERROR if the input is well
  // formed.
  const UChar* text_ptr = reinterpret_cast<const UChar*>(text.data());
  UErrorCode error_code = U_ZERO_ERROR;
  u_strToUTF8(nullptr, 0, nullptr, text_ptr, text.size(), &error_code);
  if (error_code != U_BUFFER_OVERFLOW_ERROR)
    return false;

  m_paragraphBuilder->AddText(text);

  return true;
}

fml::RefPtr<Paragraph> ParagraphBuilderImplTxt::build() {
  return Paragraph::Create(m_paragraphBuilder->Build());
}

}
