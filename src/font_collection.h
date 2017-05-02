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

#ifndef LIB_TXT_SRC_FONT_COLLECTION_H_
#define LIB_TXT_SRC_FONT_COLLECTION_H_

#include <memory>
#include <string>
#include <vector>

#include "lib/ftl/macros.h"
#include "lib/txt/src/minikin_ref_count.h"
#include "third_party/minikin/include/minikin/FontCollection.h"
#include "third_party/minikin/include/minikin/FontFamily.h"

namespace txt {

class FontCollection {
 public:
  static FontCollection& GetDefaultFontCollection();

  FontCollection();

  ~FontCollection();

  RefPtr<android::FontCollection> GetAndroidFontCollectionForFamily(
      const std::string& family);

 private:
  // TODO(chinmaygarde): Caches go here.
  FTL_DISALLOW_COPY_AND_ASSIGN(FontCollection);
};

}  // namespace txt

#endif  // LIB_TXT_SRC_FONT_COLLECTION_H_
