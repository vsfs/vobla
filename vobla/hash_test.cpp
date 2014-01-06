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
 * \file vobla/hash_test.cpp
 * \brief Unit tests for Hash Digests.
 */

#include <gtest/gtest.h>
#include <string>
#include "vobla/hash.h"

using std::string;

namespace vobla {

TEST(HashDigestTest, MD5Create) {
  const string buf("This is a buffer.");
  MD5Digest md5_0(buf);
  MD5Digest md5_3;
  md5_3.reset(buf);
  EXPECT_EQ(md5_3, md5_0);

  MD5Digest md5_2("abcdefg\n");
  EXPECT_EQ(md5_2.hexdigest(), "020861c8c3fe177da19a7e9539a5dbac");
}

TEST(HashDigestTest, SHA1Create) {
  const string buf("SHA1's buffer");
  SHA1Digest sha1_0(buf);
  SHA1Digest sha1_1;
  sha1_1.reset(buf);
  EXPECT_EQ(sha1_0, sha1_1);

  SHA1Digest sha1_2("abcdefg\n");
  EXPECT_EQ(sha1_2.hexdigest(), "69bca99b923859f2dc486b55b87f49689b7358c7");
}

}  // namespace vobla
