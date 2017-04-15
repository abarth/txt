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

#include <unicode/unistr.h>

#include <SkCanvas.h>
#include <SkGraphics.h>
#include <SkImageEncoder.h>

#include "flutter/fml/icu_util.h"
#include "lib/txt/src/paragraph_builder.h"

namespace txt {

int runTest() {
  const char* utf8_text =
      "fine world that we live in is called Earth. It's pretty nice, as far as "
      "I can tell. Rock, rock on. "
      "\xe0\xa4\xa8\xe0\xa4\xae\xe0\xa4\xb8\xe0\xa5\x8d\xe0\xa4\xa4\xe0\xa5"
      "\x87";
  icu::UnicodeString icu_text = icu::UnicodeString::fromUTF8(utf8_text);
  std::u16string u16_text(icu_text.getBuffer(), icu_text.getBuffer() + icu_text.length());

  ParagraphStyle paragraph_style;
  ParagraphBuilder builder(paragraph_style);
  TextStyle style;
  style.font_size = 32.0;
  builder.PushStyle(style);
  builder.AddText(u16_text);
  builder.Pop();
  auto paragraph = builder.Build();

  int width = 800;
  int height = 600;
  paragraph->Layout(ParagraphConstraints(width));

  SkAutoGraphics ag;
  SkBitmap bitmap;
  bitmap.allocN32Pixels(width, height);
  SkCanvas canvas(bitmap);
  paragraph->Paint(&canvas, 10.0, 300.0);

  SkFILEWStream file("foo.png");
  SkEncodeImage(&file, bitmap, SkEncodedImageFormat::kPNG, 100);
  return 0;
}

}  // namespace txt

int main(int argc, const char** argv) {
  fml::icu::InitializeICU(
      "/Users/abarth/git/flutter/engine/src/out/host_debug_unopt/icudtl.dat");
  return txt::runTest();
}
