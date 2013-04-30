/*
 * Copyright 2010-2013 (c) Lei Xu <eddyxu@gmail.com>
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

#include <gtest/gtest.h>
#include <time.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include "vobla/lru_cache.h"

using std::string;
using std::unique_ptr;

namespace vobla {

class CacheItem : public LRUCacheItem<int> {
 public:
  CacheItem(int key, int value) : k(key), v(value) {}

  virtual cache_key_type cache_key() const { return k; }

  int k;
  int v;
};

typedef LRUCache<CacheItem, CacheItem::cache_key_type, 32> lru_type;

TEST(LRUCacheTest, TestInert) {
  lru_type lru;
  CacheItem i0(1, 1);
  lru.insert(&i0);
  EXPECT_EQ(size_t(1), lru.size());
  EXPECT_EQ(&i0, lru.find(1));
  EXPECT_EQ(1, lru.victim()->k);
  EXPECT_EQ(size_t(0), lru.size());
}

TEST(LRUCacheTest, TestCheckFull) {
  lru_type lru;
  for (size_t i = 0; i < 100; i++) {
    if (i >= lru.capacity()) {
      EXPECT_TRUE(lru.full());
    } else {
      EXPECT_FALSE(lru.full());
    }

    if (lru.full()) {
      unique_ptr<CacheItem> item(lru.victim());
      ASSERT_TRUE(item != NULL);
      EXPECT_EQ(static_cast<int>(i-32), item->cache_key());
    }
    lru.insert(i, new CacheItem(i, i));
  }

  while (!lru.empty()) {
    unique_ptr<CacheItem> item(lru.victim());
  }
}

TEST(LRUCacheTest, TestLeastRecentItems) {
  lru_type lru;
  for (int i = 0; i < 100; i++) {
    if (lru.full()) {
      unique_ptr<CacheItem> item(lru.victim());
      EXPECT_EQ(static_cast<int>(i - lru.capacity()),
                item->k);
    }
    lru.insert(i, new CacheItem(i, i));
  }
  lru.clear();

  for (int i = 0; i < static_cast<int>(lru.capacity()); i++) {
    lru.insert(i, new CacheItem(i, i));
  }
  for (int i = 4; i >= 0; i--) {
    lru.use(i);
  }
  for (int i = 5; i < static_cast<int>(lru.capacity()); i++) {
    unique_ptr<CacheItem> item(lru.victim());
    EXPECT_EQ(i, item->k);
  }
  for (int i = 4; i >= 0; i--) {
    unique_ptr<CacheItem> item(lru.victim());
    EXPECT_EQ(i, item->k);
  }
  EXPECT_TRUE(lru.empty());
}

}  // namespace vobla
