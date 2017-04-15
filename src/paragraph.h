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

#ifndef LIB_TXT_SRC_PARAGRAPH_H_
#define LIB_TXT_SRC_PARAGRAPH_H_

#include <vector>

#include <minikin/LineBreaker.h>

#include "lib/txt/src/paint_record.h"
#include "lib/txt/src/paragraph_constraints.h"
#include "lib/txt/src/styled_runs.h"
#include "third_party/skia/include/core/SkTextBlob.h"

class SkCanvas;

namespace txt {

class Paragraph {
 public:
  Paragraph();
  ~Paragraph();

  void Layout(const ParagraphConstraints& constraints);
  void Paint(SkCanvas* canvas, double x, double y);

 private:
  friend class ParagraphBuilder;

  void SetText(std::vector<uint16_t> text, StyledRuns runs);
  void AddRunsToLineBreaker();

  std::vector<uint16_t> text_;
  StyledRuns runs_;

  android::LineBreaker breaker_;
  std::vector<PaintRecord> records_;
};

}  // namespace txt

#endif  // LIB_TXT_SRC_PARAGRAPH_H_
