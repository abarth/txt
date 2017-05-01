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

#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "lib/ftl/macros.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"

class RenderTest : public ::testing::Test {
 public:
  RenderTest();

  ~RenderTest();

  SkCanvas* GetCanvas();

  bool Snapshot();

  size_t GetTestCanvasWidth() const;

  size_t GetTestCanvasHeight() const;

 protected:
  void SetUp() override;

  void TearDown() override;

 private:
  size_t snapshots_;
  std::unique_ptr<SkBitmap> bitmap_;
  std::unique_ptr<SkCanvas> canvas_;

  std::string GetNextSnapshotName();

  FTL_DISALLOW_COPY_AND_ASSIGN(RenderTest);
};
