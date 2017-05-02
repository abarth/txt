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

#ifndef LIB_TXT_SRC_MINIKIN_REF_COUNT_H_
#define LIB_TXT_SRC_MINIKIN_REF_COUNT_H_

#include <utility>

#include "lib/ftl/macros.h"
#include "third_party/minikin/include/minikin/MinikinRefCounted.h"

namespace txt {

template <class T>
class RefPtr {
 public:
  using MinikinType = T;

  template <typename... Args>
  static RefPtr Make(Args&&... args) {
    return RefPtr(new MinikinType(std::forward<Args>(args)...));
  }

  RefPtr() : obj_(nullptr) {}

  RefPtr(MinikinType* obj) : obj_(obj) {}

  RefPtr(const RefPtr<MinikinType>& other) : obj_(other.obj_) {
    if (obj_) {
      obj_->Ref();
    }
  }

  RefPtr(RefPtr<MinikinType>&& other) : obj_(other.obj_) {
    other.obj_ = nullptr;
  }

  ~RefPtr() { reset(); }

  RefPtr<MinikinType>& operator=(const RefPtr<T>& other) {
    if (other.obj_ != nullptr) {
      other.obj_->Ref();
    }
    MinikinType* old_obj = obj_;
    obj_ = other.obj_;
    if (old_obj != nullptr) {
      old_obj->Unref();
    }
    return *this;
  }

  RefPtr<MinikinType>& operator=(RefPtr<MinikinType>&& other) {
    RefPtr<MinikinType>(std::move(other)).swap(*this);
    return *this;
  }

  operator bool() const { return obj_ != nullptr; }

  MinikinType* get() const { return obj_; }

  MinikinType* operator->() const { return obj_; }

  MinikinType& operator*() const {
    FTL_DCHECK(obj_);
    return obj_;
  }

  void reset() {
    if (obj_ != nullptr) {
      obj_->Unref();
      obj_ = nullptr;
    }
  }

  void swap(RefPtr<MinikinType>& other) {
    MinikinType* temp = obj_;
    obj_ = other.obj_;
    other.obj_ = temp;
  }

 private:
  MinikinType* obj_;
};

}  // namespace txt

#endif  // LIB_TXT_SRC_MINIKIN_REF_COUNT_H_
