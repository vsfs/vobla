/**
 * \file hash_test.h
 *
 * \brief Unit tests for Hash Digests.
 *
 * Copyright 2012 (c) Lei Xu <eddyxu@gmail.com>
 */

#include <gtest/gtest.h>
#include <string>
#include "vobla/hash.h"

using std::string;

TEST(HashDigestTest, MD5Create) {
  const string buf("This is a buffer.");
  MD5Digest md5_0(buf);
  MD5Digest md5_1(buf.data(), buf.size());
  EXPECT_EQ(md5_0, md5_1);

  MD5Digest md5_2("abcdefg\n");
  EXPECT_EQ(md5_2.hexdigest(), "020861c8c3fe177da19a7e9539a5dbac");
}

TEST(HashDigestTest, SHA1Create) {
  const string buf("SHA1's buffer");
  SHA1Digest sha1_0(buf);
  SHA1Digest sha1_1(buf.data(), buf.size());
  EXPECT_EQ(sha1_0, sha1_1);

  SHA1Digest sha1_2("abcdefg\n");
  EXPECT_EQ(sha1_2.hexdigest(), "69bca99b923859f2dc486b55b87f49689b7358c7");
}
