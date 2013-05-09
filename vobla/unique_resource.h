/*
 * Copyright 2011-2013 (c) Lei Xu <eddyxu@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file unique_resource.h
 * \brief Auto-releasing resource.
 */

#ifndef VOBLA_UNIQUE_RESOURCE_H_
#define VOBLA_UNIQUE_RESOURCE_H_

namespace vobla {

/**
 * \class Resource.
 * \brief The interface to declare a resource type.
 */
class Resource {
 public:
  Resource() = default;

  virtual ~Resource() {
  }

  /// Acquires resource.
  virtual void acquire() = 0;

  /// Releases resource.
  virtual void release() = 0;
};

/**
 * \brief It provides an exception-safe way to auto-release the resource.
 *
 * It follows the smart pointer design patten, but provides the auto-releasing
 * function for customized resource classes. It requires the resource classes
 * having 'acquire()' and 'release()' methods.
 *
 * Similar works are:
 *   - std::unique_ptr
 *   - std::lock_guard
 */
template <typename R>
class UniqueResource {
 public:
  typedef R value_type;

  UniqueResource() : value_(nullptr) {
  }

  explicit UniqueResource(value_type* resource) : value_(resource) {
    if (value_) {
      value_->acquire();
    }
  }

  ~UniqueResource() {
    if (value_) {
      value_->release();
    }
  }

  void reset(value_type* new_value) {
    if (value_ && value_ != new_value) {
      value_->release();
    }
    value_ = new_value;
    if (value_) {
      value_->acquire();
    }
  }

 private:
  value_type* value_;
};

}  // namespace vobla

#endif  // VOBLA_UNIQUE_RESOURCE_H_
