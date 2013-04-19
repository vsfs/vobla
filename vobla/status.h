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
 * \file status.h
 * \brief Status -- encapsure both error code and error message.
 *
 */

#ifndef VOBLA_STATUS_H_
#define VOBLA_STATUS_H_

#include <string>

/**
 * \class Status
 * \brief encapure both error code and error message.
 *
 * It provides more informations with passing return code around.
 * It is prefered to be used as return value.
 *
 * The error code 0 is for successful.
 */
class Status {
 public:
  /// The default constructor builds a success status (error_code == 0)
  Status();

  /// Constructs a Status object with error code and error message.
  Status(int code, const std::string& message);

  /// Copy constructor.
  Status(const Status& rhs);

  /// Explicit destructor.
  ~Status();

  /// Assign operation.
  Status& operator=(const Status& rhs);

  /// Returns the error code.
  int error() const;

  /// Sets the new error code.
  void set_error(int code);

  /// Returns the error message.
  const std::string& message() const;

  /// Sets the new error message.
  void set_message(const std::string& message);

  // TODO(eddyxu): set message with format
  // void set_error_message(const char* fmt, ...)

  /// Tests whether the error code is zero.
  bool ok() const;

  /// A static Status object to prepresent OK status.
  static const Status OK;

  /// Returns true if two status objects have same error numbers and messages.
  bool operator==(const Status &rhs) const;

  bool operator!=(const Status &rhs) const {
    return !(*this == rhs);
  }

 private:
  int code_;
  std::string message_;
};

#endif  // VOBLA_STATUS_H_
