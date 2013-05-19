/*
 * Copyright 2013 (c) Lei Xu <eddyxu@gmail.com>
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

#ifndef VOBLA_FILE_H_
#define VOBLA_FILE_H_

#include <string>
#include "vobla/macros.h"

using std::string;

namespace vobla {

class File {
};

class TemporaryFile : public File {
};

/**
 * \brief Temporary directory.
 *
 */
class TemporaryDirectory {
 public:
  enum class ScopeOp {
    /// Keeps the temporary directory after this object being destructed.
    KEEP,
    /// Recursively deletes the temporary directory when it being destructed.
    DELETE
  };

  explicit TemporaryDirectory(ScopeOp op = ScopeOp::DELETE);

  ~TemporaryDirectory();

  TemporaryDirectory(TemporaryDirectory &&rhs);

  const string& path() const {
    return path_;
  }

 private:
  ScopeOp op_;
  string path_;

  DISALLOW_COPY_AND_ASSIGN(TemporaryDirectory);
};

}  // namespace vobla

#endif  // VOBLA_FILE_H_
