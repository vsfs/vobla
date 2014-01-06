/*
 * Copyright 2011-2014 (c) Lei Xu <eddyxu@gmail.com>
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

/**
 * \file vobla/stl_util.h
 * \brief Provide extensed template algorithms.
 */

#ifndef VOBLA_STL_UTIL_H_
#define VOBLA_STL_UTIL_H_

#include <algorithm>

/// Returns true if the container has the value.
template <typename InputIterator, typename Value>
bool contains(InputIterator begin, InputIterator end, const Value& value) {
  return std::find(begin, end, value) != end;
}

/**
 * \brief Does the container has a element satisifying the predictate
 */
template <typename InputIterator, typename Predicate>
bool contains_if(InputIterator begin, InputIterator end, Predicate pred) {
  return std::find_if(begin, end, pred) != end;
}

/**
 * \brief Delete the pointers in a container
 */
template <typename InputIterator>
void delete_pointers(InputIterator begin, InputIterator end) {
  for (; begin != end; ++begin) {
    delete *begin;
  }
}

/**
 * \brief Delete the pointers in a container
 */
template <typename Container>
void delete_pointers(const Container& container) {
  delete_pointers(container.begin(), container.end());
}

#endif  // VOBLA_STL_UTIL_H_
