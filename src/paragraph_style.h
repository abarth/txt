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

#ifndef LIB_TXT_SRC_PARAGRAPH_STYLE_H_
#define LIB_TXT_SRC_PARAGRAPH_STYLE_H_

#include <string>

#include "lib/txt/src/font_style.h"
#include "lib/txt/src/font_weight.h"
#include "lib/txt/src/text_align.h"

namespace txt {

class ParagraphStyle {
 public:
  TextAlign textAlign = TextAlign::left;
  FontWeight fontWeight = FontWeight::w400;
  FontStyle fontStyle = FontStyle::normal;
  int maxLines = 0;
  std::string fontFamily;
  double fontSize = 14.0;
  double lineHeight = 1.0;
  std::string ellipsis;
};

}  // namespace txt

#endif  // LIB_TXT_SRC_PARAGRAPH_STYLE_H_
