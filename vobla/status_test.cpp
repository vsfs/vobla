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
 * \file status_test.cpp
 * \brief Unit tests for status class.
 */

#include <gtest/gtest.h>
#include "vobla/status.h"

TEST(StatusTest, TestConstructors) {
  // Default constructor
  Status s;
  EXPECT_TRUE(s.ok());
  EXPECT_EQ(0, s.error());
  EXPECT_TRUE(s.message().empty());

  Status s1(0, "SUCCESS");
  EXPECT_TRUE(s1.ok());
  EXPECT_EQ(0, s1.error());
  EXPECT_EQ("SUCCESS", s1.message());

  EXPECT_NE(s, s1);

  Status s2(1, "test failure");
  Status s3(s2);
  Status s4;
  s4 = s2;
  EXPECT_EQ(s2, s3);
  EXPECT_EQ(s3, s4);
}

TEST(StatusTest, TestSetterAndGetter) {
  Status s;
  s.set_error(10);
  s.set_message("10");
  EXPECT_EQ(Status(10, "10"), s);
}
