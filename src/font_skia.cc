/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "lib/txt/src/font_skia.h"

#include <minikin/MinikinFont.h>

namespace txt {

FontSkia::FontSkia(sk_sp<SkTypeface> typeface)
    : MinikinFont(typeface->uniqueID()), mTypeface(std::move(typeface)) {}

FontSkia::~FontSkia() = default;

static void FontSkia_SetSkiaPaint(sk_sp<SkTypeface> typeface,
                                  SkPaint* skPaint,
                                  const android::MinikinPaint& paint) {
  skPaint->setTypeface(std::move(typeface));
  skPaint->setTextEncoding(SkPaint::kGlyphID_TextEncoding);
  // TODO: set more paint parameters from Minikin
  skPaint->setTextSize(paint.size);
}

float FontSkia::GetHorizontalAdvance(uint32_t glyph_id,
                                     const android::MinikinPaint& paint) const {
  SkPaint skPaint;
  uint16_t glyph16 = glyph_id;
  SkScalar skWidth;
  FontSkia_SetSkiaPaint(mTypeface, &skPaint, paint);
  skPaint.getTextWidths(&glyph16, sizeof(glyph16), &skWidth, NULL);
#ifdef VERBOSE
    ALOGD("width for typeface %d glyph %d = %f", mTypeface->uniqueID(), glyph_id
#endif
    return skWidth;
}

void FontSkia::GetBounds(android::MinikinRect* bounds,
                         uint32_t glyph_id,
                         const android::MinikinPaint& paint) const {
  SkPaint skPaint;
  uint16_t glyph16 = glyph_id;
  SkRect skBounds;
  FontSkia_SetSkiaPaint(mTypeface, &skPaint, paint);
  skPaint.getTextWidths(&glyph16, sizeof(glyph16), NULL, &skBounds);
  bounds->mLeft = skBounds.fLeft;
  bounds->mTop = skBounds.fTop;
  bounds->mRight = skBounds.fRight;
  bounds->mBottom = skBounds.fBottom;
}

const void* FontSkia::GetTable(uint32_t tag,
                               size_t* size,
                               android::MinikinDestroyFunc* destroy) {
  // we don't have a buffer to the font data, copy to own buffer
  const size_t tableSize = mTypeface->getTableSize(tag);
  *size = tableSize;
  if (tableSize == 0) {
    return nullptr;
  }
  void* buf = malloc(tableSize);
  if (buf == nullptr) {
    return nullptr;
  }
  mTypeface->getTableData(tag, 0, tableSize, buf);
  *destroy = free;
  return buf;
}

const sk_sp<SkTypeface>& FontSkia::GetSkTypeface() {
  return mTypeface;
}

}  // namespace txt
