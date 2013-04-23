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

#include <gtest/gtest.h>
#include <map>
#include <vector>
#include <unordered_map>  // NOLINT
#include "vobla/stl_util.h"
#include "vobla/map_util.h"

using std::map;
using std::unordered_map;
using std::vector;

namespace vobla {

typedef map<int, int> IntMap;
typedef unordered_map<int, int*> IntHashMap;
typedef vector<int> IntVector;

TEST(MapUtilTest, ContainKey) {
  IntMap test_map;
  test_map[0] = 1;
  test_map[1] = 2;
  test_map[2] = 3;
  EXPECT_TRUE(contain_key(test_map, 2));
  EXPECT_FALSE(contain_key(test_map, 3));
}

TEST(MapUtilTest, AppendKeys) {
  IntMap test_map;
  for (int i = 0; i < 10; i++) {
    test_map[i] = 100 + i;
  }
  IntVector keys;
  append_keys_from_map(test_map, &keys);
  EXPECT_EQ(static_cast<size_t>(10), keys.size());
  for (int i = 0; i < 10; i++) {
    EXPECT_TRUE(contains(keys.begin(), keys.end(), i));
  }
}

TEST(MapUtilTest, DeleteSecondPointersForUnorderedMap) {
  IntHashMap int_hash_map;
  for (int i = 0; i < 10; ++i) {
    int *obj = new int;
    *obj = i;
    int_hash_map[i] = obj;
  }
  delete_second_pointers(int_hash_map.begin(), int_hash_map.end());
}

}  // namespace vobla
