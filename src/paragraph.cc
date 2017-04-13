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

#include <minikin/LineBreaker.h>

#include "lib/txt/src/paragraph.h"

namespace txt {

Paragraph::Paragraph() = default;

Paragraph::~Paragraph() = default;

void Paragraph::Layout(const ParagraphConstraints& constraints) {
  android::LineBreaker breaker;
  breaker.resize(text_.size());
  memcpy(breaker.buffer(), text_.data(), text_.size() / sizeof(text_[0]));
  breaker.setText();
  breaker.setLineWidths(0.0f, 0, constraints.width());

  android::FontStyle fontStyle;
  android::MinikinPaint paint;
  paint.size = 32;

  breaker.addStyleRun(&paint, nullptr, fontStyle, 0, text_.size(), false);
  breaker.computeBreaks();
}

void Paragraph::Paint(SkCanvas* canvas, double x, double y) {}

}  // namespace txt
