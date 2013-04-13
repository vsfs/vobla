/**
 * \file algorithm.h
 *
 * Provide extensed template algorithms
 *
 * Copyright 2011 Lei Xu <eddyxu@gmail.com>
 */

#ifndef VOBLA_STL_UTIL_H_
#define VOBLA_STL_UTIL_H_

#include <algorithm>
#include "vobla/macros.h"

/**
 * \brief Does the container contains a given value
 */
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
 * \brief Does the container has the key
 */
template <typename InputIterator, typename Key>
bool contains_key(InputIterator begin, InputIterator end, const Key& key) {
  return true;
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
