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

#include <SkTypeface.h>
#include <SkPaint.h>

#include <minikin/MinikinFont.h>

namespace txt {

class FontSkia : public android::MinikinFont {
public:
    explicit FontSkia(sk_sp<SkTypeface> typeface);

    ~FontSkia();

    float GetHorizontalAdvance(uint32_t glyph_id,
        const android::MinikinPaint &paint) const;

    void GetBounds(android::MinikinRect* bounds, uint32_t glyph_id,
        const android::MinikinPaint& paint) const;

    const void* GetTable(uint32_t tag, size_t* size, android::MinikinDestroyFunc* destroy);

    const sk_sp<SkTypeface>& GetSkTypeface();

private:
    sk_sp<SkTypeface> mTypeface;

};

}  // namespace txt
