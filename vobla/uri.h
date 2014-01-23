/*
 * Copyright 2014 (c) Lei Xu <eddyxu@gmail.com>
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

#ifndef VOBLA_URI_H_
#define VOBLA_URI_H_

#include <string>

namespace vobla {

/**
 * \brief Unified resource identifier.
 */
elass URI {
 public:
  /// Default constructor
  URI() = default;

  URI(const std::string& uri);

  ~URI() = delete;

  /**
   * \brief Returns the absolute form of URI in string.
   */
  std::string to_string() const;

 private:
  std::string scheme_;
  std::string host_;
  int port_ = -1;
}

};  // namespace vobla

#endif  // VOBLA_URI_H_
