/**
 * Copyright 2012-2013 (c) Lei Xu <eddyxu@gmail.com>
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
 * \file "vobla/range.h"
 * \brief Defines Range and MultiDimRange classes.
 */

#ifndef VOBLA_RANGE_H_
#define VOBLA_RANGE_H_

#include <glog/logging.h>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <utility>

using std::array;
using std::pair;

/**
 * \class Range
 * \brief A one-dimentional value range (or interval).
 *
 * It can represent both open interval or closed interval on either endpoints.
 *
 * For example:
 *  - The interval '[10, 20]' is a closed interval, which means its
 *  'is_upper_opened() = false' and 'is_lower_opened() = false';
 *  - The interval '(10, 20)' is open interval, which means its
 *  'is_upper_opened() = true' and 'is_lower_opened() = true';
 *
 * Of course, it also supports half open / half closed intervals such as
 *  - '(10, 20]'
 *  - '[10, 20)'
 *
 *  \note By default, both endpoints are closed.
 */
template <typename Value>
class Range {
 public:
  typedef Value value_type;

  /// Minimal value.
  static const Value kMin;
  /// Maximal value.
  static const Value kMax;

  /// Default constructor. It constructs a full-range Range object.
  Range() : lower_(std::numeric_limits<value_type>::min()),
    upper_(std::numeric_limits<value_type>::max()),
    lower_opened_(false), upper_opened_(false) {
  }

  /**
   * \brief Constructs a Range object with lower bound and upper bound.
   * \param l lower bound.
   * \param u upper bound.
   *
   * \note The lower bound must be smaller than or equal to upper bound.
   */
  Range(value_type l, value_type u) : lower_(l), upper_(u),
      lower_opened_(false), upper_opened_(false) {
    CHECK_LE(l, u);
  }

  /**
   * \brief Constructs a Range object with an intialize list.
   *
   * E.g.
   * ~~~~~~~~~~~~~~~~{.cpp}
   * Range r = {1, 100}  // a range to represents: [0, 100]
   * ~~~~~~~~~~~~~~~~
   */
  Range(std::initializer_list<value_type> values)  // NOLINT
      : lower_opened_(false), upper_opened_(false) {
    CHECK_GE(2, values.size());
    auto it = values.begin();
    lower_ = *it;
    ++it;
    upper_ = *it;
    CHECK_LE(lower_, upper_);
  }

  /// Returns the lower endpoint.
  value_type lower() const {
    return lower_;
  }

  /// Returns true if the lower endpoint is opened.
  bool is_lower_opened() const {
    return lower_opened_;
  }

  /// Sets a closed lower endpiont.
  void set_lower(value_type l) {
    set_lower(l, false);
  }

  void set_lower(value_type l, bool is_opened) {
    lower_ = l;
    lower_opened_ = is_opened;
  }

  value_type upper() const {
    return upper_;
  }

  /// Returns true if the upper endpoint is opened.
  bool is_upper_opened() const {
    return upper_opened_;
  }

  void set_upper(value_type u) {
    set_upper(u, false);
  }

  void set_upper(value_type u, bool is_opened) {
    upper_ = u;
    upper_opened_ = is_opened;
  }

  bool operator==(const Range &rhs) const {
    return lower_ == rhs.lower_ && upper_ == rhs.upper_ &&
        lower_opened_ == rhs.lower_opened_ &&
        upper_opened_ == rhs.upper_opened_;
  }

  bool operator!=(const Range &rhs) const {
    return !(*this == rhs);
  }

  /// Returns true if this range includes the range 'rhs'.
  bool contains(const Range &rhs) const {
    return lower_ <= rhs.lower_ && upper_ >= rhs.upper_;
  }

 private:
  Value lower_;
  Value upper_;
  bool lower_opened_;
  bool upper_opened_;
};

template <> template<typename V>
const V Range<V>::kMin = std::numeric_limits<V>::min();

template <> template<typename V>
const V Range<V>::kMax = std::numeric_limits<V>::max();

/**
 * \class MultiDimRange
 * \brief Multi-dimentional ranges.
 * \tparam Value value type.
 * \tparam Dimention the number of dimentions.
 */
template <typename Value, int Dimention>
class MultiDimRange {
 public:
  typedef Value value_type;
  typedef Range<Value> range_type;

  static const Value kMin;
  static const Value kMax;

  enum { DIMENSION = Dimention };

  static int dimention() {
    return DIMENSION;
  }

  MultiDimRange() {
  }

  MultiDimRange(
      std::initializer_list<pair<value_type, value_type>> values) {
    size_t i = 0;
    for(const auto &lower_and_upper : values) {  // NOLINT
      ranges_[i].set_lower(lower_and_upper.first);
      ranges_[i].set_upper(lower_and_upper.second);
      i++;
      if (i >= DIMENSION) {
        break;
      }
    }
  }

  MultiDimRange(const MultiDimRange &rhs) {
    ranges_ = rhs.ranges_;
  }

  value_type lower(int dim) const {
    CHECK_LT(dim, DIMENSION);
    return ranges_[dim].lower();
  }

  void set_lower(int dim, value_type new_value) {
    CHECK_LT(dim, DIMENSION);
    ranges_[dim].set_lower(new_value);
  }

  value_type upper(int dim) const {
    CHECK_LT(dim, DIMENSION);
    return ranges_[dim].upper();
  }

  void set_upper(int dim, value_type new_value) {
    CHECK_LT(dim, DIMENSION);
    ranges_[dim].set_upper(new_value);
  }

  range_type& range(int dim) const {
    CHECK_LT(dim, DIMENSION);
    return ranges_[dim];
  }

  void set_range(int dim, const range_type& new_range) {
    CHECK_LT(dim, DIMENSION);
    ranges_[dim] = new_range;
  }

  range_type& operator[](int dim) {
    CHECK_LT(dim, DIMENSION);
    return ranges_[dim];
  }

  const range_type& operator[](int dim) const {
    return ranges_[dim];
  }

  /*
   * \brief Resets all lowerer bound to the minimal value and all upper bound
   * to the max value.
   */
  void clear() {
    ranges_.fill(range_type());
  }

  bool operator==(const MultiDimRange &rhs) const {
    return ranges_ == rhs.ranges_;
  }

  bool contains(const MultiDimRange &rhs) const {
    for (size_t i = 0; i < DIMENSION; i++) {
      if (!ranges_[i].contains(rhs.ranges_[i])) {
        return false;
      }
    }
    return true;
  }

 private:
  array<range_type, DIMENSION> ranges_;
};

template <> template<typename V, int D>
const V MultiDimRange<V, D>::kMin = std::numeric_limits<value_type>::min();

template <> template<typename V, int D>
const V MultiDimRange<V, D>::kMax = std::numeric_limits<value_type>::max();

#endif  // VOBLA_RANGE_H_
