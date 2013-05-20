/*
 * Copyright 2013 (c) Lei Xu <eddyxu@gmail.com>
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

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include <string>
#include "vobla/file.h"

namespace fs = boost::filesystem;
using std::string;

namespace vobla {

TEST(TemporaryDirectoryTest, UseDeleteScopeOp) {
  string tmp_path;
  {
    TemporaryDirectory tmp_dir;
    tmp_path = tmp_dir.path();
    EXPECT_TRUE(fs::exists(tmp_path));
  }
  EXPECT_FALSE(fs::exists(tmp_path));
}

TEST(TemporaryDirectoryTest, TestMoveConstructor) {
  string tmp_path;
  TemporaryDirectory td1;
  tmp_path = td1.path();

  TemporaryDirectory td2 = std::move(td1);
  EXPECT_TRUE(td1.path().empty());
  EXPECT_EQ(tmp_path, td2.path());

  TemporaryDirectory td3;
  string td3_path = td3.path();
  EXPECT_TRUE(fs::exists(td3_path));
  td3 = std::move(td2);
  EXPECT_EQ(tmp_path, td3.path());
  EXPECT_FALSE(fs::exists(td3_path));
}

}  // namespace vobla
