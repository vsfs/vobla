/**
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
 *
 * \brief Unit tests for Range and MultiDimRange.
 */

#include <gtest/gtest.h>
#include "vobla/range.h"

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
