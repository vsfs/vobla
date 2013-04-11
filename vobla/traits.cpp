/**
 * Copyright 2012 Lei Xu <eddyxu@gmail.com>
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
 *
 * \file type_meta.cpp
 * \brief Implemantion of type meta functions.
 */

#include <algorithm>
#include <map>
#include <string>
#include "vobla/traits.h"

using std::map;
using std::string;

int parse_type_string_to_int(const std::string &str) {
  string low_case_str = str;
  std::transform(low_case_str.begin(),
                 low_case_str.end(), low_case_str.begin(), ::tolower);
  static const std::map<std::string, int> kTypeStringMap = {
    {"char", CHAR}, {"uint8", UINT8}, {"int8", INT8 },
    {"int16", INT16 }, {"uint16", UINT16 },
    {"int32", INT32 }, {"uint32", UINT32 },
    {"int64", INT64 }, {"uint64", UINT64 },
    {"float", FLOAT }, {"double", DOUBLE },
    {"string", STRING },
  };
  auto iter = kTypeStringMap.find(low_case_str);
  if (iter == kTypeStringMap.end()) {
    return UNKNOWN;
  }
  return iter->second;
}

const char* type_strings[] = { "unknown", "char", "uint8", "int8",
  "int16", "uint16", "int32", "uint32", "int64", "uint64", "float",
  "double", "string" };

string type_int_to_string(int type_id) {
    if (type_id < 0 || type_id > STRING) {
    return type_strings[UNKNOWN];
  }
  return type_strings[type_id];
}
