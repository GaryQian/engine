// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/text/skia/paragraph_builder_impl_skia.h"

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

#include "third_party/skia/modules/skparagraph/include/SkTextStyle.h"

namespace blink {

namespace {
  SkTextStyle txt_to_skia(const txt::TextStyle& txt) {
    SkTextStyle skia;

    skia.setColor(txt.color);
    skia.setDecoration((SkTextDecoration)txt.decoration);
    skia.setDecorationColor(txt.decoration_color);
    skia.setDecorationStyle((SkTextDecorationStyle)txt.decoration_style);
    skia.setDecorationThicknessMultiplier(SkDoubleToScalar(txt.decoration_thickness_multiplier));

    SkFontStyle fontStyle(
        (SkFontStyle::Weight)((int)txt.font_weight * 100 + 100),
        SkFontStyle::Width::kNormal_Width,
        txt.font_style == txt::FontStyle::normal
            ? SkFontStyle::Slant::kUpright_Slant
            : SkFontStyle::Slant::kItalic_Slant);
    skia.setFontStyle(fontStyle);
    skia.setTextBaseline((SkTextBaseline)txt.text_baseline);
    skia.setFontFamilies(txt.font_families);

    skia.setFontSize(SkDoubleToScalar(txt.font_size));
    skia.setLetterSpacing(SkDoubleToScalar(txt.letter_spacing));
    skia.setWordSpacing(SkDoubleToScalar(txt.word_spacing));
    skia.setHeight(SkDoubleToScalar(txt.height));

    skia.setLocale(txt.locale);
    if (txt.has_background) skia.setBackgroundColor(txt.background);
    if (txt.has_foreground) skia.setForegroundColor(txt.foreground);

    skia.resetShadows();
    for (auto& shadow : txt.text_shadows) {
      SkTextShadow shadow1;
      shadow1.fOffset = shadow.offset;
      shadow1.fBlurRadius = shadow.blur_radius;
      shadow1.fColor = shadow.color;
      skia.addShadow(shadow1);
    }

    return skia;
  };

  txt::TextStyle skia_to_txt(const SkTextStyle& skia) {

    txt::TextStyle txt;

    txt.color = skia.getColor();
    txt.decoration = (txt::TextDecoration)skia.getDecoration();
    txt.decoration_color = skia.getDecorationColor();
    txt.decoration_style = (txt::TextDecorationStyle) skia.getDecorationStyle();
    txt.decoration_thickness_multiplier = skia.getDecorationThicknessMultiplier();
    txt.font_weight = (txt::FontWeight) ((int)skia.getFontStyle().weight()/100 - 1);
    txt.font_style = skia.getFontStyle().slant() == SkFontStyle::kItalic_Slant
          ? txt::FontStyle::italic
          : txt::FontStyle::normal;
    txt.text_baseline = (txt::TextBaseline) skia.getTextBaseline();
    txt.font_families = { skia.getFirstFontFamily() };
    txt.font_size  = skia.getFontSize();
    txt.letter_spacing = skia.getLetterSpacing();
    txt.word_spacing = skia.getWordSpacing();
    txt.height = skia.getHeight();
    txt.locale = skia.getLocale();
    if ((txt.has_background = skia.hasBackground())) {
      txt.background = skia.getBackground();
    }
    if ((txt.has_foreground = skia.hasForeground())) {
      txt.foreground = skia.getForeground();
    }
    txt.text_shadows.clear();
    for (auto& shadow : skia.getShadows()) {
      txt::TextShadow shadow1;
      shadow1.color = shadow.fColor;
      shadow1.blur_radius = shadow.fBlurRadius;
      shadow1.offset = shadow.fOffset;
      txt.text_shadows.emplace_back(shadow1);
    }

    return txt;
  };

  SkParagraphStyle txt_to_skia(const txt::ParagraphStyle& txt) {
    SkParagraphStyle skia;

    SkTextStyle textStyle;
    SkFontStyle fontStyle(
        (SkFontStyle::Weight)txt.font_weight,
        SkFontStyle::Width::kNormal_Width,
        txt.font_style == txt::FontStyle::normal
        ? SkFontStyle::Slant::kUpright_Slant
        : SkFontStyle::Slant::kItalic_Slant);
    textStyle.setFontStyle(fontStyle);
    textStyle.setFontSize(SkDoubleToScalar(txt.font_size));
    textStyle.setHeight(SkDoubleToScalar(txt.height));
    textStyle.setFontFamily(txt.font_family);
    textStyle.setLocale(txt.locale);
    skia.setTextStyle(textStyle);

    // TODO: strut

    // General paragraph properties.
    skia.setTextAlign((SkTextAlign)txt.text_align);
    skia.setTextDirection((SkTextDirection)txt.text_direction);
    skia.setMaxLines(txt.max_lines);
    skia.setEllipsis(txt.ellipsis);

    // TODO: break strategy?...

    // This is something flutter expect us to do
    skia.turnHintingOff();

    return skia;
  }

  void print_text_style(const SkTextStyle& ts) {
    //FML_LOG(ERROR) << "TextStyle2: " << std::endl;
    //FML_LOG(ERROR) <<
    //"color: " << ts.getColor() << " " <<
    //"FontWeight: " << (int)ts.getFontStyle().weight() << " " <<
    //"FontFamily: " << ts.getFirstFontFamily().c_str() << " " <<
    //"FontStyle: " << (int)ts.getFontStyle().slant() << " " <<
    //(ts.getFontStyle().slant() == SkFontStyle::Slant::kUpright_Slant ? "normal" : "italic") << " " <<
    //"FontSize: " << ts.getFontSize() << " " <<
    //"LetterSpacing: " << ts.getLetterSpacing() << " " <<
    //"WordSpacing: " << ts.getWordSpacing() << " " <<
    //"Height: " << ts.getHeight() << " " <<
    //"Locale: " << ts.getLocale() << " " <<
    //"Background: " << (ts.hasBackground() ? std::to_string(ts.getBackground().getColor()) : "none") << " " <<
    //"Foreground: " << (ts.hasForeground() ? std::to_string(ts.getForeground().getColor()) : "none") << " " <<
    //std::endl;
  }

  void print_paragraph_style(SkParagraphStyle ps) {
    /*
    FML_LOG(ERROR) << "ParagraphStyle2: " << std::endl;
    FML_LOG(ERROR) <<
                   "TextAlign: " << (int)ps.effective_align() << " " <<
                   "TextDirection: " << (int)ps.getTextDirection() << " " <<
                   "MaxLines: " << ps.getMaxLines() << " " <<
                   "Ellipsis: " << ps.getEllipsis().c_str() << " " <<
                   std::endl;
    */
    auto ts = ps.getTextStyle();
    print_text_style(ts);
  }
}

ParagraphBuilderImplSkia::ParagraphBuilderImplSkia(
    const txt::ParagraphStyle& style, sk_sp<SkFontCollection> fontCollection)
  : m_builder(std::make_shared<SkParagraphBuilder>(txt_to_skia(style), fontCollection)) {
  print_paragraph_style(txt_to_skia(style));
}

void ParagraphBuilderImplSkia::pushStyle(const txt::TextStyle& style) {
  m_builder->pushStyle(txt_to_skia(style));
  print_text_style(txt_to_skia(style));
}

txt::TextStyle ParagraphBuilderImplSkia::peekStyle() const {
  return skia_to_txt(m_builder->peekStyle());
}

void ParagraphBuilderImplSkia::pop() {
  m_builder->pop();
}

bool ParagraphBuilderImplSkia::addText(const std::u16string& text) {

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

  m_builder->addText(text);
  return true;
};

fml::RefPtr<Paragraph> ParagraphBuilderImplSkia::build() {
    return Paragraph::Create(m_builder->Build());
}

}
