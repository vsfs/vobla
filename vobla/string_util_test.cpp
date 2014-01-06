/*
 * Copyright 2014 (c) Lei Xu <eddyxu@gmail.com>
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
#include <string>
#include "vobla/string_util.h"

using std::string;

namespace vobla {

TEST(StringUtilTest, TestStringPrintf) {
  string t1 = stringprintf("abc");
  EXPECT_EQ("abc", t1);

  string t2 = stringprintf("abc %1.1f %d %s", 1.0f, 20, "test");
  EXPECT_EQ("abc 1.0 20 test", t2);
}

}  // namespace vobla
