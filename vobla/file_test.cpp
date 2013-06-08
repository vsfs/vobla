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
#include <fcntl.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "vobla/file.h"

namespace fs = boost::filesystem;
using std::string;
using std::unique_ptr;

namespace vobla {

class FileTest : public ::testing::Test {
 protected:
  void SetUp() {
    dir_.reset(new TemporaryDirectory);
    dirpath_ = dir_->path();
  }

  unique_ptr<TemporaryDirectory> dir_;
  string dirpath_;
};

TEST_F(FileTest, TestConstructor) {
  string path = dirpath_ + "/test";
  File file(path, O_WRONLY | O_CREAT);
  EXPECT_EQ(-1, file.fd());
  auto status = file.open();
  EXPECT_TRUE(status.ok());
  EXPECT_GT(file.fd(), 2);  // FD is larger than stdin/stdout/stderr.
}

TEST_F(FileTest, TestOpenFromStaticMethod) {
  File file = File::open(dirpath_ + "/test", O_CREAT);
  EXPECT_GT(file.fd(), 2);
}

TEST_F(FileTest, TestMoveFileFd) {
  File file = File::open(dirpath_ + "/test", O_CREAT);
  int fd = file.fd();
  File other = std::move(file);
  EXPECT_EQ(fd, other.fd());
  EXPECT_EQ(-1, file.fd());
}

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
