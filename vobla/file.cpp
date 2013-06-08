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
#include <errno.h>
#include <fcntl.h>
#include <glog/logging.h>
#include <algorithm>
#include <cstring>
#include <string>
#include "vobla/file.h"

namespace fs = boost::filesystem;

namespace vobla {

File File::open(const string& path, int flags, mode_t mode) {
  File tmp(path, flags, mode);
  auto status = tmp.open();
  if (!status.ok()) {
    LOG(ERROR) << "Failed to open file: " << status.message();
  }
  return tmp;
}

File::File() : fd_(-1) {
}

File::File(const string& path, int flags, mode_t mode)
    : fd_(-1), path_(path), flags_(flags), mode_(mode) {
}

File::File(File&& rhs) : fd_(rhs.release()) {
}

File::~File() {
  this->close();
}

File& File::operator=(File&& rhs) {
  *this = std::move(rhs);
  return *this;
}

int File::fd() const {
  return fd_;
}

Status File::open() {
  if (fd_ >= 0) {
    return Status(-1, "The file has already been opened.");
  }
  if (path_.empty()) {
    return Status(-1, "The file path is empty.");
  }

  fd_ = ::open(path_.c_str(), flags_, mode_);
  if (fd_ == -1) {
    return Status(-errno, strerror(errno));
  }
  return Status::OK;
}

Status File::close() {
  if (fd_ < 0) {
    return Status::OK;
  }
  if (::close(fd_) < 0) {
    return Status(-errno, strerror(errno));
  }
  release();
  return Status::OK;
}

int File::release() {
  int ret = fd_;
  fd_ = -1;
  return ret;
}

void File::swap(File& other) {
  std::swap(fd_, other.fd_);
}

void swap(File& lhs, File& rhs) {
  lhs.swap(rhs);
}

TemporaryDirectory::TemporaryDirectory(ScopeOp op) : op_(op) {
  fs::path tmp_path = fs::temp_directory_path() / fs::unique_path();
  path_ = tmp_path.string();
  fs::create_directories(tmp_path);
}

TemporaryDirectory::~TemporaryDirectory() {
  remove_directory();
}

TemporaryDirectory::TemporaryDirectory(TemporaryDirectory &&rhs)
  : op_(rhs.op_), path_(std::move(rhs.path_)) {
  rhs.op_ = ScopeOp::KEEP;
}

TemporaryDirectory& TemporaryDirectory::operator=(TemporaryDirectory&& rhs) {
  if (this != &rhs) {
    remove_directory();
    op_ = rhs.op_;
    path_ = std::move(rhs.path_);
    rhs.op_ = ScopeOp::KEEP;
  }
  return *this;
}

void TemporaryDirectory::remove_directory() {
  if (op_ == ScopeOp::DELETE) {
    boost::system::error_code ec;
    fs::remove_all(path_, ec);
    if (ec) {
      LOG(WARNING) << "Failed to delete temporary directory: " << ec;
    }
  }
}

}  // namespace vobla
