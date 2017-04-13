/*
 * Copyright (C) 2013 The Android Open Source Project
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

// This is a test program that uses Minikin to layout and draw some text.
// At the moment, it just draws a string into /data/local/tmp/foo.pgm.

#include <stdio.h>
#include <vector>
#include <fstream>

#include <unicode/unistr.h>
#include <unicode/utf16.h>

#include <minikin/MinikinFontFreeType.h>
#include <minikin/Layout.h>

#include <SkCanvas.h>
#include <SkGraphics.h>
#include <SkImageEncoder.h>
#include <SkTypeface.h>
#include <SkPaint.h>

#include "lib/txt/src/font_skia.h"
#include "flutter/fml/icu_util.h"

using std::vector;
using namespace android;

namespace txt {

FT_Library library;  // TODO: this should not be a global

FontCollection *makeFontCollection() {
    vector<FontFamily *>typefaces;
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

    FontFamily *family = new FontFamily();
    for (size_t i = 0; i < sizeof(fns)/sizeof(fns[0]); i++) {
        const char *fn = fns[i];
        MinikinFont *font = new FontSkia(SkTypeface::MakeFromFile(fn));
        family->addFont(font);
    }
    typefaces.push_back(family);

#if 1
    family = new FontFamily();
    const char *fn = "/Users/abarth/git/flutter/engine/src/DroidSansDevanagari-Regular.ttf";
    MinikinFont *font = new FontSkia(SkTypeface::MakeFromFile(fn));
    family->addFont(font);
    typefaces.push_back(family);
#endif

    return new FontCollection(typefaces);
}

// Maybe move to MinikinSkia (esp. instead of opening GetSkTypeface publicly)?

void drawToSkia(SkCanvas *canvas, SkPaint *paint, Layout *layout, float x, float y) {
    size_t nGlyphs = layout->nGlyphs();
    uint16_t *glyphs = new uint16_t[nGlyphs];
    SkPoint *pos = new SkPoint[nGlyphs];
    sk_sp<SkTypeface> lastFace = NULL;
    sk_sp<SkTypeface> skFace = NULL;
    size_t start = 0;

    paint->setTextEncoding(SkPaint::kGlyphID_TextEncoding);
    for (size_t i = 0; i < nGlyphs; i++) {
        FontSkia *mfs = static_cast<FontSkia *>(layout->getFont(i));
        skFace = mfs->GetSkTypeface();
        glyphs[i] = layout->getGlyphId(i);
        pos[i].fX = x + layout->getX(i);
        pos[i].fY = y + layout->getY(i);
        if (i > 0 && skFace.get() != lastFace.get()) {
            paint->setTypeface(lastFace);
            canvas->drawPosText(glyphs + start, (i - start) << 1, pos + start, *paint);
            start = i;
        }
        lastFace = skFace;
    }
    paint->setTypeface(skFace);
    canvas->drawPosText(glyphs + start, (nGlyphs - start) << 1, pos + start, *paint);
    delete[] glyphs;
    delete[] pos;
}

int runMinikinTest() {
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        return -1;
    }
    Layout::init();

    FontCollection *collection = makeFontCollection();
    Layout layout;
    layout.setFontCollection(collection);
    const char *text = "fine world \xe0\xa4\xa8\xe0\xa4\xae\xe0\xa4\xb8\xe0\xa5\x8d\xe0\xa4\xa4\xe0\xa5\x87";
    int bidiFlags = 0;
    FontStyle fontStyle;
    MinikinPaint minikinPaint;
    minikinPaint.size = 32;
    icu::UnicodeString icuText = icu::UnicodeString::fromUTF8(text);
    layout.doLayout(icuText.getBuffer(), 0, icuText.length(), icuText.length(), bidiFlags, fontStyle, minikinPaint);
    layout.dump();

    SkAutoGraphics ag;

    int width = 800;
    int height = 600;
    SkBitmap bitmap;
    bitmap.allocN32Pixels(width, height);
    SkCanvas canvas(bitmap);
    SkPaint paint;
    paint.setARGB(255, 0, 0, 128);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(2);
    paint.setTextSize(32);
    paint.setAntiAlias(true);
    canvas.drawLine(10, 300, 10 + layout.getAdvance(), 300, paint);
    paint.setStyle(SkPaint::kFill_Style);
    drawToSkia(&canvas, &paint, &layout, 10, 300);

    SkFILEWStream file("foo.png");
    SkEncodeImage(&file, bitmap, SkEncodedImageFormat::kPNG, 100);
    return 0;
}

}  // namespace txt

int main(int argc, const char** argv) {
    fml::icu::InitializeICU("/Users/abarth/git/flutter/engine/src/out/host_debug_unopt/icudtl.dat");
    return txt::runMinikinTest();
}
