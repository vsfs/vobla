/*
 * Copyright 2011-2014 (c) Lei Xu <eddyxu@gmail.com>
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
 * \brief Unit tests for Range and MultiDimRange.
 */

#include <gtest/gtest.h>
#include <limits>
#include "vobla/range.h"

namespace vobla {

typedef Range<int> IntRange;
typedef MultiDimRange<int, 8> TestRange;

TEST(RangeTest, TestInitialization) {
  IntRange range(1, 15);
  EXPECT_EQ(1, range.lower());
  EXPECT_FALSE(range.is_lower_opened());
  EXPECT_EQ(15, range.upper());
  EXPECT_FALSE(range.is_upper_opened());

  IntRange range1 = {1, 15};
  EXPECT_EQ(range, range1);
}

TEST(RangeTest, TestNotEqualOp) {
  IntRange range0 = {1, 20};
  IntRange range1 = {2, 35};
  EXPECT_NE(range0, range1);

  IntRange range2;
  range2.set_lower(1, true);
  range2.set_upper(20, true);
  EXPECT_NE(range0, range2);
}

TEST(RangeTest, TestLengthNoWrap) {
  Range<int> range0 = {1, 10};
  EXPECT_EQ(9, range0.length());
  Range<int> range1 = {-1, 10};
  EXPECT_EQ(11, range1.length());

  Range<uint32_t> range2 = {1, 10};
  EXPECT_EQ(9u, range2.length());

  Range<float> range3 = {1.2, 3.5};
  EXPECT_NEAR(2.3, range3.length(), 0.00001);
  Range<float> range4 = {-1.2, 3.5};
  EXPECT_NEAR(4.7, range4.length(), 0.00001);
}

TEST(RangeTest, TestLengthWraps) {
  const int kMaxIntRange = std::numeric_limits<int>::max() -
      std::numeric_limits<int>::min();
  Range<int> r0 = {10, 1};
  EXPECT_EQ(kMaxIntRange - 9, r0.length());
  Range<int> r1 = {5, -4};
  EXPECT_EQ(kMaxIntRange - 9, r1.length());

  const uint32_t kMaxUintRange = std::numeric_limits<uint32_t>::max() -
      std::numeric_limits<uint32_t>::min();
  Range<uint32_t> r2 = {10, 1};
  EXPECT_EQ(kMaxUintRange - 9, r2.length());

  const float kMaxFloatRange = std::numeric_limits<float>::max() -
      std::numeric_limits<float>::min();
  Range<float> r3 = { 1.6, 0.4 };
  EXPECT_NEAR(kMaxFloatRange - 2.0, r3.length(), 0.0001);
}

TEST(MultiDimRangeTest, Initialization) {
  TestRange empty;
  for (int i = 0; i < TestRange::dimention(); i++) {
    EXPECT_EQ(TestRange::kMin, empty.lower(i));
    EXPECT_EQ(TestRange::kMax, empty.upper(i));
  }

  TestRange range = { {1, 2}, {3, 4} };
  EXPECT_EQ(1, range.lower(0));
  EXPECT_EQ(2, range.upper(0));
  EXPECT_EQ(3, range.lower(1));
  EXPECT_EQ(4, range.upper(1));
  for (int i = 2; i < TestRange::dimention(); i++) {
    EXPECT_EQ(TestRange::kMin, range.lower(i));
    EXPECT_EQ(TestRange::kMax, range.upper(i));
  }
}

}  // namespace vobla
