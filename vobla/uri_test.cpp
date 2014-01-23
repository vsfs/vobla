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
#include "vobla/status.h"
#include "vobla/uri.h"

using std::string;

namespace vobla {

TEST(URITest, TestHttpURI) {
  string str("http://www.google.com/drive");
  URI uri(str);
  EXPECT_EQ(uri.scheme(), "http");
  EXPECT_EQ(uri.port(), 0);
  EXPECT_EQ(uri.host(), "www.google.com");
  EXPECT_EQ(uri.path(), "/drive");
  EXPECT_EQ(uri.query(), "");

  EXPECT_TRUE(uri.parse("HTTP://www.google.com/drive").ok());
  EXPECT_EQ(uri.scheme(), "http");
}

TEST(URLTest, TestUserAndPassword) {
  URI uri("http+git://user:pass@github.com:8888/vobla/vobla");
  EXPECT_EQ("http+git", uri.scheme());
  EXPECT_EQ("user", uri.username());
  EXPECT_EQ("pass", uri.password());
  EXPECT_EQ("github.com", uri.host());
  EXPECT_EQ(8888, uri.port());
  EXPECT_EQ("/vobla/vobla", uri.path());
}

TEST(URITest, TestAbsoluteFileURI) {
  URI uri;
  EXPECT_TRUE(uri.parse("file:///path/to/file").ok());
  EXPECT_EQ("file", uri.scheme());
  EXPECT_EQ("", uri.host());
  EXPECT_EQ("/path/to/file", uri.path());
}

TEST(URITest, TestRelativeFileURI) {
// EXPECT_TRUE(uri.parse("///path/to/file").ok());
}

}  // namespace vobla
