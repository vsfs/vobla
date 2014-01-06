/*
 * Copyright 2011-2013 (c) Lei Xu <eddyxu@gmail.com>
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
 * \file map_util.h
 * \brief Map (and unordered_map) functions to simplify map manunipulations.
 */

#ifndef VOBLA_MAP_UTIL_H_
#define VOBLA_MAP_UTIL_H_

#include <glog/logging.h>
#include <utility>

namespace vobla {

/// Returns true if the map contains the given key.
template <typename Map>
bool contain_key(const Map& map,
                 const typename Map::value_type::first_type& key) {
  return map.find(key) != map.end();
}

/// Returns true if the map contains both the key and the value.
template <typename Map>
bool contain_key_and_value(const Map& map,
                           const typename Map::value_type::first_type& key,
                           const typename Map::value_type::second_type& value) {
  auto it = map.find(key);
  if (it == map.end()) {
    return false;
  }
  return it->second == value;
}

/**
 * \brief Returns the pointer of the value in the map for the given key. If
 * the key does not exist, return nullptr.
 */
template <typename Map>
const typename Map::mapped_type*
find_or_null(const Map& container,  // NOLINT
             const typename Map::key_type& key) {
  auto it = container.find(key);  // NOLINT
  if (it == container.end()) {
    return nullptr;
  }
  return &it->second;
}

/**
 * \brief The non-const version of find_or_null()
 */
template <typename Map>
typename Map::mapped_type*
find_or_null(Map& map,  // NOLINT
             const typename Map::key_type& key) {
  auto it = map.find(key);  // NOLINT
  if (it == map.end()) {
    return nullptr;
  }
  return &it->second;
}

/**
 * \brief Returns the value in map for the given key. If the key does not
 * exist, the program dies.
 */
template <typename Map>
typename Map::mapped_type
find_or_die(const Map& map,
            const typename Map::key_type& key) {
  auto it = map.find(key);
  CHECK(it != map.end());
  return it->second;
}

/**
 * \brief Returns the second value as pointer. If the key does not exist,
 * return nullptr.
 *
 * The second type of this map must be a pointer.
 */
template <typename Map>
typename Map::mapped_type
find_pointer_or_null(const Map& map,
                     const typename Map::key_type& key) {
  auto it = map.find(key);
  if (it != map.end()) {
    return it->second;
  }
  return nullptr;
}

/// Returns the second value as a pointer, otherwise dies.
template <typename Map>
typename Map::mapped_type
find_pointer_or_die(const Map& map,
                    const typename Map::key_type& key) {
  auto it = map.find(key);
  CHECK(it != map.end());
  return it->second;
}

/// The non-const version of `find_pointer_or_null()`.
template <typename Map>
typename Map::value_type::second_type
find_pointer_or_null(Map& map,  // NOLINT
    const typename Map::value_type::first_type& key) {
  auto it = map.find(key);
  if (it == map.end()) {
    return nullptr;
  }
  return it->second;
}

/**
 * \brief Insert a key, value pair to the map. If the key exists, delete its
 * old content and assign the new value to it.
 */
template <typename Map>
void insert_and_delete(Map* map,
    const typename Map::value_type::first_type& key,
    const typename Map::value_type::second_type& value) {
  typename Map::iterator it = map->find(key);
  if (it != map->end() && it->second != value) {
    delete it->second;
  }
  (*map)[key] = value;
}

/**
 * \brief Insert a key into a map if the key does not exists and initilize the
 * value as the default value. Dies if the key is already existed.
 */
template <typename Map>
typename Map::value_type::second_type insert_key_or_die(
    Map& map, const typename Map::value_type::first_type& key) {  // NOLINT
  auto result = map.insert(
      typename Map::value_type(key, typename Map::value_type::second_type()));
  CHECK(result.second);
  return result.first->second;
}

template <typename Map>
void erase_and_delete(Map* map,
                      const typename Map::value_type::first_type& key) {
  // It is ok to delete a NULL pointer.
  delete find_pointer_or_null(*map, key);
  map->erase(key);
}

/**
 * \brief Delete the pointers of map-like iterator it->second
 */
template <typename InputIterator>
void delete_second_pointers(InputIterator begin, InputIterator end) {
  for (; begin != end; ++begin) {
    delete begin->second;
  }
}

/**
 * \brief Delete the pointers of map-like iterator it->second
 */
template <typename Container>
void delete_second_pointers(const Container& container) {
  delete_second_pointers(container.begin(), container.end());
}

/**
 * \brief Deletes the pointer of map-like iterator it->second, and
 * erase the item.
 *
 * \param container a map-like (std::map or hash_map)
 * \param key the key of the point item
 */
template <typename Container>
void delete_and_erase(Container& container,  // NOLINT
                      const typename Container::key_type& key) {
  auto it = container.find(key);
  if (it != container.end()) {
    delete it->second;
    container.erase(it);
  }
}

/**
 * \brief Append the keys of the given map to the container.
 *
 * \param[in] map a map or unordered_map
 * \param[out] a container that appends all keys from the map.
 */
template <typename Map, typename Container>
void append_keys_from_map(const Map& map, Container* out) {
  for (const auto &key_and_value : map) {  // NOLINT
    out->push_back(key_and_value.first);
  }
}

/**
 * \brief Append the values of the given map to the container.
 *
 * \param[in] map a map or unordered_map
 * \param[out] a container that appends all values from the map.
 */
template <typename Map, typename Container>
void append_values_from_map(const Map& map, Container* out) {
  for (const auto &key_and_value : map) {  // NOLINT
    out->push_back(key_and_value.second);
  }
}

template <typename Map>
typename Map::value_type::first_type sum_keys(const Map& map) {
  typename Map::value_type::first_type result = 0;
  for (const auto &key_and_value : map) {  // NOLINT
    result += key_and_value.first;
  }
  return result;
}

template <typename Map>
typename Map::value_type::second_type sum_values(const Map& map) {
  typename Map::value_type::second_type result = 0;
  for (const auto &key_and_value : map) {  // NOLINT
    result += key_and_value.second;
  }
  return result;
}

}  // namespace vobla

#endif  // VOBLA_MAP_UTIL_H_
