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

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <glog/logging.h>
#include <string>
#include "vobla/file.h"

namespace fs = boost::filesystem;

namespace vobla {

TemporaryDirectory::TemporaryDirectory(ScopeOp op) : op_(op) {
  fs::path tmp_path = fs::temp_directory_path() / fs::unique_path();
  path_ = tmp_path.string();
  fs::create_directories(tmp_path);
}

TemporaryDirectory::~TemporaryDirectory() {
  if (op_ == ScopeOp::DELETE) {
    boost::system::error_code ec;
    fs::remove_all(path_, ec);
    if (ec) {
      LOG(WARNING) << "Failed to delete temporary directory: " << ec;
    }
  }
}

}  // namespace vobla
