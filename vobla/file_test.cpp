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
#include <algorithm>
#include <cerrno>
#include <memory>
#include <string>
#include <vector>
#include "vobla/file.h"

namespace fs = boost::filesystem;
using std::string;
using std::unique_ptr;
using std::vector;

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

  // Construct an empty file object.
  File empty;
  EXPECT_EQ(-1, empty.fd());
}

TEST_F(FileTest, TestOpenFromStaticMethod) {
  File file = File::open(dirpath_ + "/test", O_CREAT);
  EXPECT_GT(file.fd(), 2);

  // Open an non-existed file for reading.
  auto file1 = File::open(dirpath_ + "/nonexited", O_RDONLY);
  EXPECT_EQ(file1.fd(), -1);
}

TEST_F(FileTest, TestMoveFileFd) {
  File file = File::open(dirpath_ + "/test", O_CREAT);
  int fd = file.fd();
  File other = std::move(file);
  EXPECT_EQ(fd, other.fd());
  EXPECT_EQ(-1, file.fd());

  File one_more;
  one_more = std::move(other);
  EXPECT_EQ(fd, one_more.fd());
  EXPECT_EQ(-1, other.fd());
}

TEST_F(FileTest, TestOpenFile) {
  File file1(dirpath_ + "/file1", O_CREAT);
  EXPECT_TRUE(file1.open().ok());

  // A file should not be opened multiple times.
  EXPECT_FALSE(file1.open().ok());

  File file2(dirpath_ + "/file2", O_WRONLY);
  auto status = file2.open();
  EXPECT_FALSE(status.ok());
  EXPECT_EQ(-ENOENT, status.error());

  File file3;  // No path was given.
  EXPECT_FALSE(file3.open().ok());
}

TEST_F(FileTest, TestSwap) {
  File file1(dirpath_ + "/file1", O_CREAT);
  int fd = file1.fd();
  File file2;
  file1.swap(file2);
  EXPECT_EQ(fd, file2.fd());
  EXPECT_EQ(-1, file1.fd());

  File file3;
  std::swap(file2, file3);
  EXPECT_EQ(-1, file2.fd());
  EXPECT_EQ(fd, file3.fd());
}

TEST(TemporaryFileTest, UseDeleteScopeOp) {
  string tmpfile_path;
  {
    TemporaryFile tmpfile;
    tmpfile_path = tmpfile.path();
    EXPECT_FALSE(fs::exists(tmpfile_path));
    File file = File::open(tmpfile_path, O_CREAT);
    EXPECT_GT(file.fd(), 0);
    EXPECT_TRUE(fs::exists(tmpfile_path));

    file.close();
  }

  // This file is automatically deleted after tmpfile going out of scope.
  EXPECT_FALSE(fs::exists(tmpfile_path));
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
