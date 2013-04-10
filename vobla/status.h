/**
 * \file status.h
 *
 * \brief Status -- encapsure both error code and error message.
 *
 * Copyright 2011 (c) Lei Xu <eddyxu@gmail.com>
 */

#ifndef BASE_STATUS_H_
#define BASE_STATUS_H_

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

  Status(int code, const std::string& message);

  Status(const Status& rhs);

  Status& operator=(const Status& rhs);

  int error_code() const;

  void set_error_code(int code);

  const std::string& error_message() const;

  void set_error_message(const std::string& message);

  // TODO(eddyxu): set message with format
  // void set_error_message(const char* fmt, ...)

  /// set both error code and error message
  void set_error(int code, const std::string& message);

  /// Tests whether the error code is zero
  bool ok() const;

  static const Status OK;

 private:
  int code_;
  std::string message_;
};

#endif  // BASE_STATUS_H_
