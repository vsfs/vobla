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
 * \brief Implementation of Status class
 */

#include "vobla/status.h"
#include <string>

using std::string;

const Status Status::OK = Status();

Status::Status() : code_(0) {
}

Status::Status(int code, const string& message) : code_(code),
    message_(message) {
}

Status::Status(const Status& rhs) {
  *this = rhs;
}

Status& Status::operator=(const Status& rhs) {
  code_ = rhs.code_;
  message_ = rhs.message_;
  return *this;
}

void Status::set_error(int code) {
  code_ = code;
}

int Status::error() const {
  return code_;
}

void Status::set_message(const string& message) {
  message_ = message;
}

const string& Status::message() const {
  return message_;
}

bool Status::ok() const {
  return code_ == 0;
}

bool Status::operator==(const Status &rhs) const {
  return code_ == rhs.code_ && message_ == rhs.message_;
}
