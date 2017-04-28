// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_TXT_SRC_FONT_COLLECTION_H_
#define LIB_TXT_SRC_FONT_COLLECTION_H_

#include <memory>
#include <string>
#include <vector>

#include "lib/ftl/macros.h"
#include "third_party/minikin/include/minikin/FontCollection.h"
#include "third_party/minikin/include/minikin/FontFamily.h"

namespace txt {

class FontCollection {
 public:
  static FontCollection& GetDefaultFontCollection();

  FontCollection();

  ~FontCollection();

  android::MinikinAutoUnref<android::FontCollection>
  GetAndroidFontCollectionForFamily(const std::string& family);

 private:
  // TODO(chinmaygarde): Caches go here.
  FTL_DISALLOW_COPY_AND_ASSIGN(FontCollection);
};

}  // namespace txt

#endif  // LIB_TXT_SRC_FONT_COLLECTION_H_
