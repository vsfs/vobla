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

#ifndef VOBLA_STRING_UTIL_H_
#define VOBLA_STRING_UTIL_H_

#include <string>

namespace vobla {

/**
 * \brief A printf-like function to create std::string.
 * \see to facebook/vobla/String.h
 */
std::string stringprintf(const char* format, ...)
    __attribute__((format(printf, 1, 2)));

}

#endif  // VOBLA_STRING_UTIL_H_
