/**
 * \file map_util.h
 * \brief Map (and unordered_map) functions to simplify map manunipulations.
 *
 * Copyright 2012 (c) Lei Xu <eddyxu@gmail.com>
 */

#ifndef VOBLA_MAP_UTIL_H_
#define VOBLA_MAP_UTIL_H_

#include <glog/logging.h>
#include <utility>

template <typename Map>
bool contain_key(const Map& map,
                 const typename Map::value_type::first_type& key) {
  return map.find(key) != map.end();
}

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

template <typename Container>
const typename Container::mapped_type*
find_or_null(const Container& container,  // NOLINT
             const typename Container::key_type& key) {
  auto it = container.find(key);  // NOLINT
  if (it == container.end()) {
    return nullptr;
  }
  return &it->second;
}

template <typename Container>
typename Container::mapped_type*
find_or_null(Container& container,  // NOLINT
             const typename Container::key_type& key) {
  auto it = container.find(key);  // NOLINT
  if (it == container.end()) {
    return nullptr;
  }
  return &it->second;
}

template <typename Container>
typename Container::mapped_type
find_or_die(const Container& container,
            const typename Container::key_type& key) {
  auto it = container.find(key);
  CHECK(it != container.end());
  return it->second;
}

template <typename Container>
typename Container::mapped_type
find_pointer_or_null(const Container& container,
                     const typename Container::key_type& key) {
  auto it = container.find(key);
  if (it != container.end()) {
    return it->second;
  }
  return NULL;
}

template <typename Container>
typename Container::mapped_type
find_pointer_or_die(const Container& container,
                    const typename Container::key_type& key) {
  auto it = container.find(key);
  CHECK(it != container.end());
  return it->second;
}

template <typename Map>
typename Map::value_type::second_type
find_pointer_or_null(Map &map,  // NOLINT
    const typename Map::value_type::first_type &key) {
  auto it = map.find(key);
  if (it == map.end()) {
    return NULL;
  }
  return it->second;
}

/**
 * \brief Insert a key, value pair to the map. If the key exists, delete its
 * old content and assign the new value to it.
 */
template <typename Map>
void insert_and_delete(Map *map,
    const typename Map::value_type::first_type &key,
    const typename Map::value_type::second_type &value) {
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
    Map& map, const typename Map::value_type::first_type &key) {  // NOLINT
  auto result = map.insert(
      typename Map::value_type(key, typename Map::value_type::second_type()));
  CHECK(result.second);
  return result.first->second;
}

template <typename Map>
void erase_and_delete(Map *map,
                      const typename Map::value_type::first_type &key) {
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
typename Map::value_type::first_type sum_keys(const Map &map) {
  typename Map::value_type::first_type result = 0;
  for (const auto &key_and_value : map) {  // NOLINT
    result += key_and_value.first;
  }
  return result;
}

template <typename Map>
typename Map::value_type::second_type sum_values(const Map &map) {
  typename Map::value_type::second_type result = 0;
  for (const auto &key_and_value : map) {  // NOLINT
    result += key_and_value.second;
  }
  return result;
}

#endif  // VOBLA_MAP_UTIL_H_
