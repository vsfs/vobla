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

#include <algorithm>
#include <cstdarg>
#include <cstring>
#include <stdexcept>
#include <string>
#include "vobla/string_util.h"

using std::string;

namespace vobla {

namespace {

/**
 * \brief Create a string from the format and va_list.
 *
 * Modified from Facebook's folly library with the modificaitons of coding
 * style and C++11 features.
 *
 * \see facebook/folly/String.h
 */
inline void stringprintf_impl(string* output, const char* format,
                              va_list args) {
  // Tru to the space at the end of output for our output buffer.
  // Find out write point then inflate its size temporarily to its
  // capacity; we will later shrink it to the size needed to represent
  // the formatted string.  If this buffer isn't large enough, we do a
  // resize and try again.

  const auto write_point = output->size();
  int remaining = output->capacity() - write_point;
  output->resize(output->capacity());

  va_list args_copy;
  va_copy(args_copy, args);
  int bytes_used = vsnprintf(&(*output)[write_point], remaining, format,
                             args_copy);
  va_end(args_copy);
  if (bytes_used < 0) {
    throw std::runtime_error(
        string("Invalid format string; snprintf returned negative "
               "with format string: ") + format);
  } else if (bytes_used < remaining) {
    // There was enough room, just shrink and return.
    output->resize(write_point + bytes_used);
  } else {
    output->resize(write_point + bytes_used + 1);
    remaining = bytes_used + 1;
    va_list args_copy;
    va_copy(args_copy, args);
    bytes_used = vsnprintf(&(*output)[write_point], remaining, format,
                           args_copy);
    va_end(args_copy);
    if (bytes_used + 1 != remaining) {
      throw std::runtime_error(
        string("vsnprint retry did not manage to work "
               "with format string: ") + format);
    }
    output->resize(write_point + bytes_used);
  }
}

}  // anon namespace

std::string stringprintf(const char* format, ...) {
  // snprintf will tell us how large the output buffer should be, but
  // we then have to call it a second time, which is costly.  By
  // guestimating the final size, we avoid the double snprintf in many
  // cases, resulting in a performance win.  We use this constructor
  // of std::string to avoid a double allocation, though it does pad
  // the resulting string with nul bytes.  Our guestimation is twice
  // the format string size, or 32 bytes, whichever is larger.  This
  // is a hueristic that doesn't affect correctness but attempts to be
  // reasonably fast for the most common cases.
  std::string ret(std::max(32UL, strlen(format) * 2), '\0');
  ret.resize(0);

  va_list ap;
  va_start(ap, format);
  stringprintf_impl(&ret, format, ap);
  va_end(ap);
  return ret;
}

}   // namespace vobla
