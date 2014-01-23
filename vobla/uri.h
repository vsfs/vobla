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

class Status;

/**
 * \brief Unified resource identifier.
 *
 * \see RFC3986, http://tools.ietf.org/html/rfc3986
 * \see Hadoop's URI class.
 */
class URI {
 public:
  /// Default constructor
  URI() = default;

  /// Construct URI object from an URI string.
  explicit URI(const std::string& uri);

  ~URI() = default;

  /**
   * \brief Parses a string form of URI and fills the members of URI class.
   *
   * \todo(eddyxu): It currently only supports absolute URI.
   */
  vobla::Status parse(const std::string& uri);

  /**
   * \brief Returns the absolute form of URI in string.
   */
  std::string to_string() const;

  const std::string& scheme() const { return scheme_; }

  const std::string& host() const { return host_; }

  int port() const { return port_; }

  const std::string& path() const { return path_; }

  const std::string& username() const { return username_; }

  const std::string& password() const { return password_; }

  const std::string& query() const { return query_; }

  const std::string& fragment() const { return fragment_; }

 private:
  std::string scheme_;
  std::string username_;
  std::string password_;
  std::string host_;
  int port_ = 0;
  std::string path_;
  std::string query_;
  std::string fragment_;
};

}  // namespace vobla

#endif  // VOBLA_URI_H_
