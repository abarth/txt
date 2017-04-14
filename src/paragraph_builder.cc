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

#include "lib/txt/src/paragraph_builder.h"

namespace txt {

ParagraphBuilder::ParagraphBuilder(ParagraphStyle style) {}

ParagraphBuilder::~ParagraphBuilder() = default;

void ParagraphBuilder::PushStyle(const TextStyle& style) {
  const size_t text_index = text_.size();
  runs_.EndRunIfNeeded(text_index);
  const size_t style_index = runs_.AddStyle(style);
  runs_.StartRun(style_index, text_index);
  style_stack_.push_back(style_index);
}

void ParagraphBuilder::Pop() {
  if (style_stack_.empty())
    return;
  const size_t text_index = text_.size();
  runs_.EndRunIfNeeded(text_index);
  style_stack_.pop_back();
  if (style_stack_.empty())
    return;
  const size_t style_index = style_stack_.back();
  runs_.StartRun(style_index, text_index);
}

void ParagraphBuilder::AddText(const std::u16string& text) {
  text_.insert(text_.end(), text.begin(), text.end());
}

std::unique_ptr<Paragraph> ParagraphBuilder::Build() {
  runs_.EndRunIfNeeded(text_.size());
  std::unique_ptr<Paragraph> paragraph = std::make_unique<Paragraph>();
  paragraph->text_.swap(text_);
  paragraph->runs_.swap(runs_);
  return paragraph;
}

}  // namespace txt
