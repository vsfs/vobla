/**
 * \brief implementation of Status class
 *
 * Copyright 2011 (c) Lei Xu <eddyxu@gmail.com>
 */

#include "base/status.h"

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

void Status::set_error_code(int code) {
  code_ = code;
}

int Status::error_code() const {
  return code_;
}

void Status::set_error_message(const string& message) {
  message_ = message;
}

const string& Status::error_message() const {
  return message_;
}

void Status::set_error(int code, const std::string& message) {
  set_error_code(code);
  set_error_message(message);
}

bool Status::ok() const {
  return code_ == 0;
}
