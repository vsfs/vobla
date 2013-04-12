/**
 * \file map_util_test.cpp
 * \brief Unit tests for map_util.h
 *
 * Copyright 2012 (c) Lei Xu <eddyxu@gmail.com>
 */

#include <gtest/gtest.h>
#include <map>
#include <vector>
#include <unordered_map>  // NOLINT
#include "vobla/algorithm.h"
#include "vobla/map_util.h"

using std::map;
using std::unordered_map;
using std::vector;

typedef map<int, int> IntMap;
typedef unordered_map<int, int*> IntHashMap;
typedef vector<int> IntVector;

class MapUtilTest : public ::testing::Test {
};

TEST_F(MapUtilTest, ContainKey) {
  IntMap test_map;
  test_map[0] = 1;
  test_map[1] = 2;
  test_map[2] = 3;
  EXPECT_TRUE(contain_key(test_map, 2));
  EXPECT_FALSE(contain_key(test_map, 3));
}

TEST_F(MapUtilTest, AppendKeys) {
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

TEST_F(MapUtilTest, DeleteSecondPointersForUnorderedMap) {
  IntHashMap int_hash_map;
  for (int i = 0; i < 10; ++i) {
    int *obj = new int;
    *obj = i;
    int_hash_map[i] = obj;
  }
  delete_second_pointers(int_hash_map.begin(), int_hash_map.end());
}
