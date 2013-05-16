/*
 * Copyright 2011-2013 (c) Ziling Huang <hzlgis@gmail.com>
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
 * \file vobla/consistent_hash_map.h
 *
 * \brief A generic class for Consistent Hashing
 */

#ifndef VOBLA_CONSISTENT_HASH_MAP_H_
#define VOBLA_CONSISTENT_HASH_MAP_H_

#include <glog/logging.h>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "vobla/map_util.h"
#include "vobla/range.h"
#include "vobla/status.h"

using std::numeric_limits;
using std::string;
using std::pair;
using std::vector;

namespace vobla {

/**
 * \class ConsistentHashMap
 *
 * \brief A generic class for Consistent Hashing, clients are responsible of
 *  calculating the key when lookup an object.
 *
 *  Refer to http://en.wikipedia.org/wiki/Consistent_hashing for information
 *  about Consistent Hashing.
 *
 *  e.g. If a node1 is inserted at position 100 and node2 is at position 200,
 *  then node1 is in charge of any value falls in range (100, 200].
 *
 * \note This class is not thread-safe.
 */
template <typename Key, typename Value, size_t Partitions = 1>
class ConsistentHashMap {
  typedef std::map<Key, Value> HashMap;

 public:
  typedef Key key_type;

  typedef Value value_type;

  // a pair of range <start, end>
  typedef Range<key_type> range_type;

  // a pair of Value and its range
  typedef pair<Value, range_type> value_to_range_type;

  typedef typename HashMap::iterator iterator;

  typedef typename HashMap::const_iterator const_iterator;

  ConsistentHashMap() : num_partitions_per_node_(Partitions) {
  }

  ConsistentHashMap(const ConsistentHashMap &rhs) {
    *this = rhs;
  }

  ~ConsistentHashMap() = default;

  ConsistentHashMap& operator=(const ConsistentHashMap& rhs) {
    ring_ = rhs.ring_;
    num_partitions_per_node_ = rhs.num_partitions_per_node_;
    return *this;
  }

  /**
   * \brief Insert a node with a client specified key value to the hash map.
   */
  Status insert(key_type key, const Value& value) {
    if (contain_key(ring_, key)) {
      return Status(-EEXIST, "The key is already inserted");
    }
    static key_type kMax = numeric_limits<key_type>::max();
    for (size_t i = 0; i < num_partitions_per_node_; i++) {
      // TODO(Ziling): improve the precision of this calculation!
      key_type new_key = (key + (kMax / num_partitions_per_node_ * i)) % kMax;
      ring_[new_key] = value;
    }
    return Status::OK;
  }

  /**
   * \brief Remove a node with a client specified key value, note this key
   * value must be the same with the original key value when inserting.
   */
  Status remove(key_type key) {
    if (!contain_key(ring_, key)) {
      return Status(-ENOENT, "The key does not exist");
    }
    static key_type kMax = numeric_limits<key_type>::max();
    for (size_t i = 0; i < num_partitions_per_node_; i++) {
      key_type new_key = (key + (kMax / num_partitions_per_node_ * i)) % kMax;
      ring_.erase(new_key);
    }
    return Status::OK;
  }

  bool has_key(key_type key) const {
    return contain_key(ring_, key);
  }

  bool empty() const {
    return ring_.empty();
  }

  /**
   * \brief Gets the responsible node for a client specified key.
   */
  Status get(key_type key, value_type* value) const {
    key_type sep;
    return get(key, &sep, value);
  }

  /// Gets both the sep and the value for a client specified key.
  Status get(key_type key, key_type* sep, value_type* value) const {
    CHECK_NOTNULL(sep);
    CHECK_NOTNULL(value);
    if (ring_.empty()) {
      return Status(-ENOENT, "The ring is empty.");
    }

    auto it = ring_.upper_bound(key);
    // if key is in the range between last key and first key, return the
    // last element in the map.
    if (it == ring_.begin() || it == ring_.end()) {
      *sep = ring_.rbegin()->first;
      *value = ring_.rbegin()->second;
    } else {
      --it;
      *sep = it->first;
      *value = it->second;
    }
    return Status::OK;
  }

  /**
   * \brief If key has a responsible node, returns the responsible node.
   * If key does not belong to any nodes, insert the key and return a
   * reference to its mapped node.
   * TODO(ziling): implement this function later.
   */
  /* value_type& operator[] (key_type key) const {
  } */

  /**
   * \brief Return the partition starting points.
   * \param[out] partitions A vector to stores the partitions;
   */
  void get_partitions(vector<key_type> *partitions) const {
    CHECK_NOTNULL(partitions);
    partitions->reserve(ring_.size());
    append_keys_from_map(ring_, partitions);
  }

  /**
   * \brief Gets all values.
   * \TODO(lxu): Use iterator to avoid memory copies.
   */
  void get_values(vector<value_type> *values) const {
    CHECK_NOTNULL(values);
    auto iter = ring_.begin();
    for (size_t i = 0; i < num_nodes(); ++i, ++iter) {
      values->push_back(iter->second);
    }
  }

  /**
   * \brief Gets the largest hash range of the current ring.
   * \param[out] range a (Key, Key) pair. If first key is smaller than the
   * second key, then the range start clockwise from first key and ends
   * at second key and does not cross zero. If first key is larger than
   * the second key, then it is a range starting clockwise from first key
   * and crossed zero to second key.
   */
  Status get_max_range(range_type *range) {
    CHECK_NOTNULL(range);
    // if there is no element, return error.
    if (ring_.empty()) {
      return Status(-ENOENT, "Cannot get max range.");
    }
    // if there is only one element, set upper bound same as lower bound
    // and let caller deal with it.
    if (ring_.size() == 1) {
      range->set_lower(ring_.begin()->first);
      range->set_upper(ring_.begin()->first);
      return Status::OK;
    }

    auto it = ring_.begin();
    range->set_lower(it->first);
    range->set_upper((++it)->first);
    key_type max_range = range->length();
    for (size_t i = 0; i < num_nodes(); i++) {
      if (it == ring_.begin()) {
        break;
      }
      key_type prev = it->first;
      key_type current;
      if (++it == ring_.end()) {
        it = ring_.begin();
      }
      if (it->first > prev) {
        current = it->first - prev;
      } else {
        // if it->first is the beginning now
        current = (numeric_limits<key_type>::max() - prev) + it->first;
      }
      if (current > max_range) {
        max_range = current;
        range->set_lower(prev);
        range->set_upper(it->first);
      }
    }
    return Status::OK;
  }

  /**
   * \brief Gets the successive value of the value this is responsible
   * for the given key.
   * \param[in] key the current key specified by client.
   * \param[out] successive the successive value returned to client.
   */
  Status succ(key_type key, Value* successive) const {
    CHECK_NOTNULL(successive);
    // O(logN)
    auto it = ring_.find(key);
    if (it == ring_.end()) {
      return Status(-ENOENT, "The value is not in the ring.");
    }
    ++it;
    if (it == ring_.end()) {
      it = ring_.begin();
    }
    *successive = it->second;
    return Status::OK;
  }

  /**
   * \brief Gets the successive value of current value on the ring.
   * \param[in] current the current value specified by client.
   * \param[out] successive the successive value returned to client.
   */
  Status succ_by_value(const Value& current, Value* successive) const {
    CHECK_NOTNULL(successive);
    // O(N)
    auto it = ring_.begin();
    for (; it != ring_.end(); ++it) {
      if (it->second == current) {
        break;
      }
    }
    if (it == ring_.end()) {
      return Status(-ENOENT, "The value is not in the ring.");
    }
    ++it;
    // if current value is the last, then the next is the begin on the ring.
    if (it == ring_.end()) {
      it = ring_.begin();
    }
    *successive = it->second;
    return Status::OK;
  }

  /**
   * \brief Gets the previous value of the value that is responsible for
   * the given key.
   * \param[in] key the current key specified by client.
   * \param[out] previous the previous value returned to client.
   */
  Status prev(key_type key, Value* previous) const {
    CHECK_NOTNULL(previous);
    // O(logN)
    auto it = ring_.find(key);
    if (it == ring_.end()) {
      return Status(-ENOENT, "The key is not in the ring.");
    }
    if (it == ring_.begin()) {
      *previous = ring_.rbegin()->second;
    } else {
      --it;
      *previous = it->second;
    }
    return Status::OK;
  }

  /**
   * \brief Gets the previous value of current value on the ring.
   * \param[in] current the current value specified by client.
   * \param[out] prev the previous value returned to client.
   */
  Status prev_by_value(const Value& current, Value* previous) const {
    CHECK_NOTNULL(previous);
    // O(N)
    auto it = ring_.begin();
    for (; it != ring_.end(); ++it) {
      if (it->second == current) {
        break;
      }
    }
    if (it == ring_.end()) {
      return Status(-ENOENT, "The value is not in the ring.");
    }
    // if current value is the beginning, the next is the end on the ring.
    if (it == ring_.begin()) {
      *previous = ring_.rbegin()->second;
    } else {
      --it;
      *previous = it->second;
    }
    return Status::OK;
  }

  /**
   * \brief Return a pair which contains a node and its range by the given key.
   */
  Status get_range(key_type key, value_to_range_type* range) const {
    CHECK_NOTNULL(range);
    if (ring_.empty()) {
      return Status(-ENOENT, "The ring is empty.");
    }

    range_type range_pair;
    auto it = ring_.upper_bound(key);
    // if it's after the last element
    if (it == ring_.end()) {
      // lower bound is the last element
      range_pair.set_lower(ring_.rbegin()->first);
      range->first = ring_.rbegin()->second;
      // upper bound is the first element
      if (ring_.begin()->first == 0) {
        range_pair.set_upper(numeric_limits<key_type>::max());
      } else {
        range_pair.set_upper(ring_.begin()->first - 1);
      }
    } else {
      range_pair.set_upper(it->first - 1);
      range_pair.set_lower((--it)->first);
      range->first = it->second;
    }
    range->second = range_pair;
    return Status::OK;
  }

  /**
   * \brief Gets the number of physical nodes.
   */
  size_t num_nodes() const {
    return ring_.size() / num_partitions_per_node_;
  }

  /**
   * \brief Gets the number of vnodes.
   */
  size_t num_partitions() const {
    return ring_.size();
  }

  /**
   * \brief Gets the number of partitions.
   */
  size_t num_partitions_per_node() const {
    return num_partitions_per_node_;
  }

  iterator begin() {
    return ring_.begin();
  }

  const_iterator begin() const {
    return ring_.begin();
  }

  iterator end() {
    return ring_.end();
  }

  const_iterator end() const {
    return ring_.end();
  }

 private:
  HashMap ring_;

  size_t num_partitions_per_node_;
};

}  // namespace vobla

#endif  // VOBLA_CONSISTENT_HASH_MAP_H_
