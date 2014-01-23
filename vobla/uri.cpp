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

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <string>
#include "vobla/status.h"
#include "vobla/uri.h"

using boost::algorithm::to_lower;
using boost::regex;
using boost::regex_match;
using boost::smatch;
using std::string;

namespace vobla {

URI::URI(const string& uri) {
  parse(uri);
}

/// See Facebook's folly/URI.cpp
Status URI::parse(const string& uri) {
  static const boost::regex uri_regex(
      "([a-zA-Z][a-zA-Z0-9+.-]*):"  // scheme:
      "([^?#]*)"                    // authority and path
      "(?:\\?([^#]*))?"             // ?query
      "(?:#(.*))?");                // #fragment
  smatch results;
  if (!regex_match(uri, results, uri_regex)) {
    return Status(-1, "Invalid URI");
  }

  scheme_ = results[1];
  to_lower(scheme_);

  static const regex authority_and_path_regex("//([^/]*)(/.*)?");
  smatch authority_and_path_matches;
  string authority_and_path_str = results[2];
  if (!regex_match(authority_and_path_str, authority_and_path_matches,
                   authority_and_path_regex)) {
    path_ = results[2];
  } else {
    static const regex authority_regex(
        "(?:([^@:]*)(?::([^@]*))?@)?"  // username, password
        "(\\[[^\\]]*\\]|[^\\[:]*)"     // host (IP-literal, dotted-IPv4, or
                                       // named host)
        "(?::(\\d*))?");               // port
    string authority = authority_and_path_matches[1];
    smatch authority_match;
    if (!regex_match(authority, authority_match, authority_regex)) {
      return Status(-1, "Invalid URI authroity");
    }

    string port_str(authority_match[4].first, authority_match[4].second);
    if (!port_str.empty()) {
      port_ = stoi(port_str);
    }

    username_ = authority_match[1];
    password_ = authority_match[2];
    host_ = authority_match[3];
    path_ = authority_and_path_matches[2];
  }
  return Status::OK;
}

}  // namespace vobla
