/*
 * Copyright (C) 2017 Google, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "lib/txt/src/paragraph.h"

#include <vector>

#include <minikin/Layout.h>
#include <minikin/LineBreaker.h>

#include "lib/txt/src/font_skia.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "third_party/skia/include/core/SkTextBlob.h"
#include "third_party/skia/include/core/SkTypeface.h"

namespace txt {
namespace {

android::FontCollection* MakeFontCollection() {
    std::vector<android::FontFamily*>typefaces;
    const char *fns[] = {
        "/Users/abarth/git/flutter/flutter/bin/cache/artifacts/material_fonts/Roboto-Regular.ttf",
        "/Users/abarth/git/flutter/flutter/bin/cache/artifacts/material_fonts/Roboto-Italic.ttf",
        "/Users/abarth/git/flutter/flutter/bin/cache/artifacts/material_fonts/Roboto-BoldItalic.ttf",
        "/Users/abarth/git/flutter/flutter/bin/cache/artifacts/material_fonts/Roboto-Light.ttf",
        "/Users/abarth/git/flutter/flutter/bin/cache/artifacts/material_fonts/Roboto-Thin.ttf",
        "/Users/abarth/git/flutter/flutter/bin/cache/artifacts/material_fonts/Roboto-Bold.ttf",
        "/Users/abarth/git/flutter/flutter/bin/cache/artifacts/material_fonts/Roboto-ThinItalic.ttf",
        "/Users/abarth/git/flutter/flutter/bin/cache/artifacts/material_fonts/Roboto-LightItalic.ttf",
    };

    android::FontFamily *family = new android::FontFamily();
    for (size_t i = 0; i < sizeof(fns)/sizeof(fns[0]); i++) {
        const char *fn = fns[i];
        android::MinikinFont *font = new FontSkia(SkTypeface::MakeFromFile(fn));
        family->addFont(font);
    }
    typefaces.push_back(family);

    family = new android::FontFamily();
    const char *fn = "/Users/abarth/git/flutter/engine/src/DroidSansDevanagari-Regular.ttf";
    android::MinikinFont *font = new FontSkia(SkTypeface::MakeFromFile(fn));
    family->addFont(font);
    typefaces.push_back(family);

    return new android::FontCollection(typefaces);
}

android::FontCollection* g_font_collection = nullptr;

android::FontCollection* GetFontCollection() {
  if (!g_font_collection)
    g_font_collection = MakeFontCollection();
  return g_font_collection;
}

} // namespace

Paragraph::Paragraph() = default;

Paragraph::~Paragraph() = default;

void Paragraph::Layout(const ParagraphConstraints& constraints) {
  android::LineBreaker breaker;
  breaker.resize(text_.size());
  memcpy(breaker.buffer(), text_.data(), text_.size() / sizeof(text_[0]));
  breaker.setText();
  breaker.setLineWidths(0.0f, 0, constraints.width());

  android::FontCollection* collection = GetFontCollection();

  android::FontStyle fontStyle;
  android::MinikinPaint paint;
  paint.size = 32;

  breaker.addStyleRun(&paint, collection, fontStyle, 0, text_.size(), false);
  breaker.computeBreaks();
}

void Paragraph::Paint(SkCanvas* canvas, double x, double y) {
  SkPaint paint;
  // fontData->platformData().setupPaint(&paint);
  paint.setTextEncoding(SkPaint::kGlyphID_TextEncoding);


  SkTextBlobBuilder builder;

  android::Layout layout;

  // auto buffer = builder.allocRunPosH();
}

}  // namespace txt
