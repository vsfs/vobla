/*
 * Copyright 2010-2014 (c) Lei Xu <eddyxu@gmail.com>
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
 * \file lru_cache.h
 * \brief A generic LRU cache
 */

#ifndef VOBLA_LRU_CACHE_H_
#define VOBLA_LRU_CACHE_H_

#include <boost/utility.hpp>
#include <cassert>
#include <cstddef>
#include <list>
#include <unordered_map>  // NOLINT
#include "vobla/stl_util.h"

namespace vobla {

/**
 * \class LRUCacheItem
 * \brief a basic template class for lru cache items.
 */
template <typename Key>
class LRUCacheItem {
 public:
  typedef Key cache_key_type;
  virtual ~LRUCacheItem() {}
  virtual cache_key_type cache_key() const = 0;
};

/**
 * \class LRUCache vobla/lru_cache.h
 * \brief A generic Least-Recent-Used(LRU) cache template.
 * \tparam Item the type of the entity stored in this LRUCache.
 * \tparam Key the type of the key that is used to locat LRUCacheItem.
 */
template <typename Item, typename Key = typename Item::cache_key_type,
  const int Capacity = 1024, typename LRUList = typename std::list<Item*>,
  typename Ctn = typename std::unordered_map<Key, typename LRUList::iterator>>
class LRUCache : boost::noncopyable {
 public:
  typedef Item value_type;
  typedef Item* pointer_type;
  typedef Key key_type;

  explicit LRUCache(int cap = Capacity) : capacity_(cap) {
  }

  /**
   * \brief Destructor.
   * Similar to STL containers, LRUCache does not support inheritance.
   */
  ~LRUCache() = default;

  /// Returns true if this LRUCache is full of capacity.
  bool full() const {
    return size() >= capacity();
  }

  bool empty() const {
    return cache_.empty();
  }

  /// Returns the number of LRUCacheItems it contains.
  size_t size() const {
    return cache_.size();
  }

  /// Returns the capacity of the LRU list
  size_t capacity() const {
    return capacity_;
  }

  void set_capacity(size_t new_cap) {
    capacity_ = new_cap;
  }

  /// Inserts a new item into the LRU cache.
  void insert(const Key &key, pointer_type item) {
    assert(!full());
    // the value should not existed before
    auto it = cache_.find(key);
    assert(it == cache_.end());

    lru_.push_back(item);
    auto last = lru_.end();
    --last;
    cache_.insert(typename container_type::value_type(key, last));
  }

  /// Insert a new item to LRU cache
  void insert(pointer_type item) {
    this->insert(item->cache_key(), item);
  }

  /**
   * \brief Finds an item's pointer with key
   *
   * Time complexity: O(1)
   */
  pointer_type find(const Key& key) const {
    pointer_type ret = nullptr;
    auto it = cache_.find(key);
    if (it != cache_.end()) {
      ret = *(it->second);
    }
    return ret;
  }

  /**
   * \brief Finds the victim item and remove it from this LRU cache.
   * \return The pointer to the victim item.
   *
   * \note After calling victim(), this LRU cache does not hold the ownership
   * of the victim item anymore.
   */
  pointer_type victim() {
    if (lru_.empty()) {
      return nullptr;
    }
    pointer_type ret = lru_.front();
    lru_.pop_front();
    cache_.erase(ret->cache_key());
    return ret;
  }

  /// Uses a item with the given key, and move it to the head
  void use(const Key &key) {
    auto it = cache_.find(key);
    assert(it != cache_.end());
    pointer_type value = *(it->second);
    auto pos(it->second);
    lru_.erase(pos);
    lru_.push_back(value);
    cache_[value->cache_key()] = --lru_.end();
  }

  /// Clears all items.
  void clear() {
    delete_pointers(lru_);
    lru_.clear();
    cache_.clear();
  }

 private:
  typedef LRUList lru_type;
  typedef Ctn container_type;

  lru_type lru_;
  container_type cache_;
  size_t capacity_;
};

}  // namespace vobla
#endif  // VOBLA_LRU_CACHE_H_
