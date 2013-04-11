/**
 * Copyright 2012 (c) Lei Xu <eddyxu@gmail.com>
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
 * \file type_meta_test.cpp
 * \brief Unit tests for type meta-programming routines.
 */

#include <gtest/gtest.h>
#include <string>
#include <type_traits>  // NOLINT
#include "vobla/traits.h"

using std::string;
using std::is_same;

TEST(TypeToIntTest, ConvertTypeToInt) {
  EXPECT_EQ(TypeToInt<char>::value(), CHAR);
  EXPECT_EQ(TypeToInt<int8_t>::value(), INT8);
  EXPECT_NE(TypeToInt<char>::value(), TypeToInt<int8_t>::value());
}

TEST(IntToTypeTest, ConvertIntToType) {
  bool result = is_same<Int2Type<CHAR>::type, char>::value;
  EXPECT_TRUE(result);

  result = is_same<Int2Type<INT32>::type, uint32_t>::value;
  EXPECT_FALSE(result);

  result = is_same<Int2Type<FLOAT>::type, float>::value;
  EXPECT_TRUE(result);

  result = is_same<Int2Type<STRING>::type, std::string>::value;
  EXPECT_TRUE(result);
}

TEST(TypeToIntTest, TestParseTypeStringToInt) {
  EXPECT_EQ(INT32, parse_type_string_to_int("int32"));
  EXPECT_EQ(UINT32, parse_type_string_to_int("uint32"));
  EXPECT_EQ(FLOAT, parse_type_string_to_int("float"));

  EXPECT_EQ(UNKNOWN, parse_type_string_to_int("int33"));
}
