/**
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
 *
 * \brief    Hash Digests
 */

#include <fcntl.h>
#include <glog/logging.h>
#include <unistd.h>
#include <cstdio>
#include <memory>
#include <string>
#include "vobla/hash.h"

using std::string;
using std::unique_ptr;

const size_t BUFSIZE = 16 * 1024;  // 16KB

MD5Digest::MD5Digest() {
}

MD5Digest::MD5Digest(const string &buffer) {
  init();
  update(buffer.data(), buffer.size());
  final();
}

MD5Digest::MD5Digest(const char* buffer, size_t size) {
  init();
  update(buffer, size);
  final();
}

MD5Digest::~MD5Digest() {
}

void MD5Digest::init() {
  MD5_Init(&context_);
}

void MD5Digest::update(const char* buffer, size_t size) {
  CHECK_NOTNULL(buffer);
  MD5_Update(&context_, buffer, size);
}

void MD5Digest::final() {
  MD5_Final(digest_.data(), &context_);
}

//----------- SHA-1 -----------------
SHA1Digest::SHA1Digest() {
}

SHA1Digest::SHA1Digest(const string& buffer) {
  init();
  update(buffer.data(), buffer.size());
  final();
}

SHA1Digest::SHA1Digest(const char* buffer, size_t size) {
  init();
  update(buffer, size);
  final();
}

SHA1Digest::~SHA1Digest() {
}

void SHA1Digest::init() {
  SHA1_Init(&context_);
}

void SHA1Digest::update(const char* buffer, size_t size) {
  CHECK_NOTNULL(buffer);
  SHA1_Update(&context_, buffer, size);
}

void SHA1Digest::final() {
  SHA1_Final(digest_.data(), &context_);
}
