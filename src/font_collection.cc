/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "lib/txt/src/font_collection.h"

#include <mutex>

#include "lib/ftl/logging.h"
#include "lib/txt/src/font_skia.h"
#include "third_party/skia/include/ports/SkFontMgr.h"

namespace txt {

FontCollection& FontCollection::GetDefaultFontCollection() {
  static FontCollection* collection = nullptr;
  static std::once_flag once;
  std::call_once(once, []() { collection = new FontCollection(); });
  return *collection;
}

FontCollection::FontCollection() = default;

FontCollection::~FontCollection() = default;

RefPtr<android::FontCollection>
FontCollection::GetAndroidFontCollectionForFamily(const std::string& family) {
  // Get the Skia font manager.
  auto skia_font_manager = SkFontMgr::RefDefault();
  FTL_DCHECK(skia_font_manager != nullptr);

  // Ask Skia to resolve a font style set for a font family name.
  // FIXME(chinmaygarde): The name "Hevetica" is hardcoded because CoreText
  // crashes when passed a null string. This seems to be a bug in Skia as
  // SkFontMgr explicitly says passing in nullptr gives the default font.
  auto font_style_set = skia_font_manager->matchFamily(
      family.length() == 0 ? "Helvetica" : family.c_str());
  FTL_DCHECK(font_style_set != nullptr);

  // Create a Minikin font family.
  android::MinikinAutoUnref<android::FontFamily> minikin_family(
      new android::FontFamily());

  // Add fonts to the Minikin font family.
  for (int i = 0, style_count = font_style_set->count(); i < style_count; ++i) {
    auto skia_typeface =
        sk_ref_sp<SkTypeface>(font_style_set->createTypeface(i));
    android::MinikinAutoUnref<FontSkia> font(
        new FontSkia(std::move(skia_typeface)));
    minikin_family->addFont(font.get());
  }

  // Create a vector of font families for the Minkin font collection. For now,
  // we only have one family in our collection.
  std::vector<android::FontFamily*> minikin_families = {
      minikin_family.get(),
  };

  // Return the font collection.
  return RefPtr<android::FontCollection>::Make(minikin_families);
}

}  // namespace txt
