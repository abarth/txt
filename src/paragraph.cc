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

const sk_sp<SkTypeface>& GetTypefaceForGlyph(const android::Layout& layout, size_t index) {
  FontSkia* font = static_cast<FontSkia*>(layout.getFont(index));
  return font->GetSkTypeface();
}

size_t GetRunLength(const android::Layout& layout, size_t start) {
  const size_t glyph_count = layout.nGlyphs();
  const sk_sp<SkTypeface>& typeface = GetTypefaceForGlyph(layout, start);
  for (size_t end = start + 1; end < glyph_count; ++end) {
    if (GetTypefaceForGlyph(layout, end).get() != typeface.get())
      return end - start;
  }
  return glyph_count - start;
}

} // namespace

Paragraph::Paragraph() = default;

Paragraph::~Paragraph() = default;

void Paragraph::Layout(const ParagraphConstraints& constraints) {
  android::LineBreaker breaker;
  breaker.setLocale(icu::Locale(), nullptr);
  breaker.resize(text_.size());
  memcpy(breaker.buffer(), text_.data(), text_.size() * sizeof(text_[0]));
  breaker.setText();
  breaker.setLineWidths(0.0f, 0, constraints.width());

  android::FontCollection* collection = GetFontCollection();

  android::FontStyle fontStyle;
  android::MinikinPaint minikinPaint;
  minikinPaint.size = 32.0f;

  breaker.addStyleRun(&minikinPaint, collection, fontStyle, 0, text_.size(), false);

  size_t count = breaker.computeBreaks();
  const int* breaks = breaker.getBreaks();

  SkTextBlobBuilder builder;

  SkPaint paint;
  paint.setAntiAlias(true);
  paint.setTextSize(32.0f);
  paint.setTextEncoding(SkPaint::kGlyphID_TextEncoding);

  android::Layout layout;
  layout.setFontCollection(collection);
  SkScalar y = 0.0f;
  size_t line_start = 0;
  for (size_t i = 0; i < count; ++i) {
    int bidiFlags = 0;
    const size_t line_end = breaks[i];
    layout.doLayout(text_.data(), line_start, line_end - line_start,
                    text_.size(), bidiFlags, fontStyle, minikinPaint);

    const size_t glyph_count = layout.nGlyphs();
    size_t run_start = 0;
    while (run_start < glyph_count) {
      const size_t run_length = GetRunLength(layout, run_start);
      // TODO(abarth): Precompute when we can use allocRunPosH.
      paint.setTypeface(GetTypefaceForGlyph(layout, run_start));
      auto buffer = builder.allocRunPos(paint, run_length);

      for (size_t run_index = 0; run_index < run_length; ++run_index) {
        const size_t glyph_index = run_start + run_index;
        buffer.glyphs[run_index] = layout.getGlyphId(glyph_index);
        const size_t pos_index = 2 * run_index;
        buffer.pos[pos_index] = layout.getX(glyph_index);
        buffer.pos[pos_index + 1] = layout.getY(glyph_index) + y;
      }

      run_start += run_length;
    }

    layout.reset();
    line_start = line_end;
    y += 32.0f;
  }

  blob_ = builder.make();
}

void Paragraph::Paint(SkCanvas* canvas, double x, double y) {
  SkPaint paint;
  paint.setARGB(255, 0, 0, 128);
  canvas->drawTextBlob(blob_.get(), x, y, paint);
}

}  // namespace txt
