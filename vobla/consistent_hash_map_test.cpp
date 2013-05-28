/**
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
 * \file consistent_hash_map_test.cpp
 *
 * \brief Unit test for Consistent Hashing class
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string.h>
#include <limits>
#include <string>
#include <utility>
#include <vector>
#include "vobla/consistent_hash_map.h"
#include "vobla/hash.h"
#include "vobla/range.h"
#include "vobla/status.h"

using std::numeric_limits;
using std::string;
using std::vector;
using std::to_string;
using ::testing::ContainerEq;

namespace vobla {

typedef ConsistentHashMap<size_t, string, 4> TestMap;

TEST(ConsistentHashMapTest, TestInsert) {
  TestMap test_map;
  vector<size_t> partitions;

  const string node1("node1");
  SHA1Digest hash1(node1);
  size_t hash_value1;
  memcpy(&hash_value1, hash1.digest(), sizeof(size_t));
  test_map.insert(hash_value1, node1);
  EXPECT_EQ(4u, test_map.num_partitions());
  partitions = test_map.get_partitions();
  EXPECT_EQ(4u, partitions.size());

  const string node2("node2");
  SHA1Digest hash2(node2);
  size_t hash_value2;
  memcpy(&hash_value2, hash2.digest(), sizeof(size_t));
  test_map.insert(hash_value2, node2);
  EXPECT_EQ(8u, test_map.num_partitions());
  partitions = test_map.get_partitions();
  EXPECT_EQ(8u, partitions.size());
}

TEST(ConsistentHashMapTest, TestRemove) {
  TestMap test_map;

  const string node1("node1");
  SHA1Digest hash1(node1);
  size_t hash_value1;
  memcpy(&hash_value1, hash1.digest(), sizeof(size_t));
  test_map.insert(hash_value1, node1);

  const string node2("node2");
  SHA1Digest hash2(node2);
  size_t hash_value2;
  memcpy(&hash_value2, hash2.digest(), sizeof(size_t));
  test_map.insert(hash_value2, node2);

  test_map.remove(hash_value2);
  EXPECT_EQ(4u, test_map.num_partitions());
  test_map.remove(hash_value1);
  EXPECT_EQ(0u, test_map.num_partitions());
}

TEST(ConsistentHashMapTest, TestGetPartitionOneNode) {
  TestMap test_map;
  const string node1("node1");
  test_map.insert(0, node1);
  vector<size_t> expected_parts;
  size_t range = numeric_limits<size_t>::max() / test_map.num_partitions();
  for (unsigned int i = 0; i < test_map.num_partitions(); i++) {
    expected_parts.push_back(range*i);
  }
  vector<size_t> actual_parts = test_map.get_partitions();
  EXPECT_THAT(actual_parts, ContainerEq(expected_parts));
}

TEST(ConsistentHashMapTest, TestGetRange) {
  TestMap test_map;
  typedef std::pair<string, Range<size_t>> node_range_type;
  const string node1("node1");
  test_map.insert(0, node1);
  size_t range = numeric_limits<size_t>::max() / test_map.num_partitions();
  vector<size_t> expected_parts;
  for (unsigned int i = 0; i < test_map.num_partitions(); i++) {
    expected_parts.push_back(range*i);
  }
  vector<size_t> actual_parts = test_map.get_partitions();
  EXPECT_THAT(actual_parts, ContainerEq(expected_parts));

  Range<size_t> range1(0, range - 1);
  Range<size_t> range2(range, range*2 - 1);
  Range<size_t> range3(range*2, range*3 - 1);
  Range<size_t> range4(range*3, numeric_limits<size_t>::max());
  node_range_type range1_node1 = std::make_pair(node1, range1);
  node_range_type range2_node1 = std::make_pair(node1, range2);
  node_range_type range3_node1 = std::make_pair(node1, range3);
  node_range_type range4_node1 = std::make_pair(node1, range4);
  node_range_type tmp;
  EXPECT_TRUE(test_map.get_range(range/2, &tmp).ok());
  EXPECT_EQ(range1_node1, tmp);
  EXPECT_TRUE(test_map.get_range(range + range/2, &tmp).ok());
  EXPECT_EQ(range2_node1, tmp);
  EXPECT_TRUE(test_map.get_range(range*2 + range/2, &tmp).ok());
  EXPECT_EQ(range3_node1, tmp);
  EXPECT_TRUE(test_map.get_range(range*3 + range/2, &tmp).ok());
  EXPECT_EQ(range4_node1, tmp);
}

TEST(ConsistentHashMapTest, TestGetMaxRange) {
  ConsistentHashMap<size_t, string, 1> test_map;
  const string node1("node1");
  const string node2("node2");
  const string node3("node3");
  const string node4("node4");
  Range<size_t> range;

  EXPECT_FALSE(test_map.get_max_range(&range).ok());

  test_map.insert(10, node1);
  EXPECT_TRUE(test_map.get_max_range(&range).ok());
  EXPECT_EQ(range.lower(), 10u);
  EXPECT_EQ(range.upper(), 10u);

  test_map.insert(100, node2);
  test_map.insert(500, node3);
  EXPECT_TRUE(test_map.get_max_range(&range).ok());
  EXPECT_EQ(range.lower(), 500u);
  EXPECT_EQ(range.upper(), 10u);

  test_map.insert(numeric_limits<size_t>::max() - 1, node4);
  EXPECT_TRUE(test_map.get_max_range(&range).ok());
  EXPECT_EQ(range.lower(), 500u);
  EXPECT_EQ(range.upper(), numeric_limits<size_t>::max() - 1);
}

TEST(ConsistentHashMapTest, TestGet) {
  TestMap test_map;
  const string node1("node1");
  test_map.insert(0, node1);
  const string node2("node2");
  // insert another node in the middle of the first nodes's range
  size_t hash_node2 = numeric_limits<size_t>::max() /
                      test_map.num_partitions() / 2;
  test_map.insert(hash_node2, node2);
  size_t range_node = numeric_limits<size_t>::max() / test_map.num_partitions();
  EXPECT_EQ(hash_node2, range_node);

  string tmp;
  for (int i = 0; i < 6; i++) {
    if (i % 2 == 0) {
      EXPECT_TRUE(test_map.get(range_node*i + range_node/2, &tmp).ok());
      EXPECT_EQ(node1, tmp);
    } else {
      EXPECT_TRUE(test_map.get(range_node*i + range_node/2, &tmp).ok());
      EXPECT_EQ(node2, tmp);
    }
  }
}

TEST(ConsistentHashMapTest, TestGetValues) {
  TestMap test_map;
  vector<string> expected_nodes;
  for (int i = 0; i < 10; ++i) {
    string node_name = string("node") + to_string(i);
    test_map.insert(i * 1000, node_name);
    expected_nodes.push_back(node_name);
  }
  vector<string> actual_nodes = test_map.get_values();
  EXPECT_THAT(actual_nodes, ContainerEq(expected_nodes));
}

TEST(ConsistentHashMapTest, TestSuccByValue) {
  TestMap test_map;
  const string node1("node1");
  test_map.insert(0, node1);
  const string node2("node2");
  test_map.insert(1000, node2);
  const string node3("node3");
  test_map.insert(2000, node3);

  string tmp;
  EXPECT_TRUE(test_map.succ_by_value(node1, &tmp).ok());
  EXPECT_EQ(node2, tmp);
}

TEST(ConsistentHashMapTest, TestSucc) {
  TestMap test_map;
  const string node1("node1");
  test_map.insert(0, node1);
  const string node2("node2");
  test_map.insert(1000, node2);
  const string node3("node3");
  test_map.insert(2000, node3);

  string tmp;
  EXPECT_EQ(-ENOENT, test_map.succ(500, &tmp).error());
  EXPECT_TRUE(test_map.succ(1000, &tmp).ok());
  EXPECT_EQ(node3, tmp);
  EXPECT_TRUE(test_map.succ(2000, &tmp).ok());
  EXPECT_EQ(node1, tmp);
}

TEST(ConsistentHashMapTest, TestPrevByValue) {
  TestMap test_map;
  const string node1("node1");
  test_map.insert(0, node1);
  const string node2("node2");
  test_map.insert(1000, node2);
  const string node3("node3");
  test_map.insert(2000, node3);

  string tmp;
  EXPECT_TRUE(test_map.prev_by_value(node3, &tmp).ok());
  EXPECT_EQ(node2, tmp);
  EXPECT_TRUE(test_map.prev_by_value(node1, &tmp).ok());
  EXPECT_EQ(node3, tmp);
}

TEST(ConsistentHashMapTest, TestPrev) {
  TestMap test_map;
  const string node1("node1");
  test_map.insert(0, node1);
  const string node2("node2");
  test_map.insert(1000, node2);
  const string node3("node3");
  test_map.insert(2000, node3);

  string tmp;
  EXPECT_EQ(-ENOENT, test_map.prev(500, &tmp).error());
  EXPECT_TRUE(test_map.prev(0, &tmp).ok());
  EXPECT_EQ(node3, tmp);
}

TEST(ConsistentHashMapTest, TestCopyConstructor) {
  TestMap test_map;
  for (int i = 0; i < 10; i++) {
    test_map.insert(0*100, string("node") + to_string(i));
  }
  vector<TestMap::key_type> partitions = test_map.get_partitions();

  TestMap copy_map = test_map;
  vector<TestMap::key_type> copy_partitions = copy_map.get_partitions();
  EXPECT_EQ(partitions, copy_partitions);
}

TEST(ConsistentHashMapTest, TestIterator) {
  TestMap test_map;
  for (int i = 0; i < 10; i++) {
    test_map.insert(i*100, string("node") + to_string(i));
  }

  TestMap::iterator it = test_map.begin();
  for (size_t i = 0; i < test_map.num_nodes(); ++i, ++it) {
    EXPECT_EQ(i*100, it->first);
    EXPECT_EQ(string("node") + to_string(i), it->second);
  }

  TestMap::const_iterator cit = test_map.begin();
  for (size_t i = 0; i < test_map.num_nodes(); ++i, ++cit) {
    EXPECT_EQ(i*100, cit->first);
    EXPECT_EQ(string("node") + to_string(i), cit->second);
  }
}

TEST(ConsistentHashMapTest, TestSinglePartition) {
  ConsistentHashMap<uint64_t, string, 1> test_map;
  test_map.insert(0, "p0");
  uint64_t sep = std::numeric_limits<uint64_t>::max() / 2;
  test_map.insert(sep, "p1");
}

TEST(ConsistentHashMapTest, TestNumNodes) {
  TestMap test_map;
  for (int i = 0; i < 10; i++) {
    test_map.insert(i*100, string("node") + to_string(i));
  }
  EXPECT_EQ(10u, test_map.num_nodes());
}

TEST(ConsistentHashMapTest, TestNumPartitions) {
  TestMap test_map;
  for (int i = 0; i < 10; i++) {
    test_map.insert(i*100, string("node") + to_string(i));
  }
  EXPECT_EQ(40u, test_map.num_partitions());
}

TEST(ConsistentHashMapTest, TestNumPartitionsPerNode) {
  TestMap test_map;
  EXPECT_EQ(4u, test_map.num_partitions_per_node());
}

TEST(ConsistentHashMapTest, TestSwapTwoMaps) {
  TestMap map0 = { {1, "node1"}, {10, "node2"} };
  TestMap map1;
  map0.swap(map1);

  EXPECT_TRUE(map0.empty());

  size_t sep;
  string node;
  EXPECT_TRUE(map1.get(5, &sep, &node).ok());
  EXPECT_EQ(1u, sep);
  EXPECT_EQ("node1", node);
}

}  // namespace vobla
