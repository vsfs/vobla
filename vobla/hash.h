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
 *
 */

/**
 * \file vobla/hash.h
 * \brief Hash Digests
 *
 * This file provides few classes to encapture the common usages of various
 * hash functions:
 *   - MD5
 *   - SHA1
 */

#ifndef VOBLA_HASH_H_
#define VOBLA_HASH_H_

#if defined(__APPLE__) && defined(__MACH__)
  #define COMMON_DIGEST_FOR_OPENSSL
  #include <CommonCrypto/CommonDigest.h>
  #define SHA1 CC_SHA1
#else
#include <openssl/md5.h>
#include <openssl/sha.h>
#endif

#include <array>
#include <memory>
#include <string>

namespace vobla {

/**
 * \class BaseHashDigest
 * \brief The base class of HashDigest.
 */
template <typename Context, size_t L>
class BaseHashDigest {
 public:
  enum { LENGTH = L };

  /// Copy constractor
  BaseHashDigest(const BaseHashDigest& rhs);

  BaseHashDigest(BaseHashDigest&& rhs) {
    digest_ = std::move(rhs.digest_);
  }

  BaseHashDigest& operator=(const BaseHashDigest& rhs);

  BaseHashDigest& operator=(BaseHashDigest&& rhs) {
    digest_ = std::move(rhs.digest_);
    return *this;
  }

  bool operator<(const BaseHashDigest& rhs) const {
    return digest_ < rhs.digest_;
  }

  bool operator==(const BaseHashDigest &rhs) const {
    return digest_ == rhs.digest_;
  }

  /// Resets the value of this hash from the new content.
  virtual void reset(const std::string& buffer);

  /// Initial this digest for feeding data.
  virtual void init() = 0;

  /// Feeds data to this hash digest.
  virtual void update(const std::string& buffer) = 0;

  /// Finalize the data feeding process.
  virtual void final() = 0;

  /// get underlay digest array.
  const unsigned char* digest() const { return digest_.data(); }

  /// return the hexadecimal digest.
  std::string hexdigest() const;

  /// parse the digest from a hexadecimal format string
  void parse_hexdigest(const std::string& hash_str);

 protected:
  // Disallow to create base digest directly.
  BaseHashDigest();

  virtual ~BaseHashDigest() {}

  std::array<unsigned char, LENGTH> digest_;

  Context context_;
};

/**
 * \class MD5Digest
 * \brief MD5 digest.
 */
class MD5Digest : public BaseHashDigest<MD5_CTX, 16> {
 public:
  // Factory methods
  /// Creates a MD5Digest from a string.
  static MD5Digest* create(const std::string& buffer);

  /// Constructs an empty MD5Digest.
  MD5Digest();

  /// Constructs a MD5Digest from a string.
  explicit MD5Digest(const std::string& buffer);

  ~MD5Digest();

  /// Initializes a SHA1Digest update.
  void init();

  /// Updates the content.
  void update(const std::string& buf);

  /**
   * \brief Finalizes the MD5Digest content generation and move the MD5 value
   * to the digest_ field.
   */
  void final();
};

/**
 * \class SHA1Digest
 * \brief SHA1 digest.
 */
class SHA1Digest : public BaseHashDigest<SHA_CTX, 20> {
 public:
  // Factory methods
  /// Creates a SHA1Digest from a string.
  static SHA1Digest* create(const std::string &buffer);

  /// Constructs an empty SHA1Digest.
  SHA1Digest();

  /// Constructs a SHA1Digest from a string.
  explicit SHA1Digest(const std::string &buffer);

  ~SHA1Digest();

  /// Initializes a SHA1Digest update.
  void init();

  /// Updates the content.
  void update(const std::string& buffer);

  /**
   * \brief Finalizes the SHA1Digest content generation and move the SHA1 value
   * to the digest_ field.
   */
  void final();
};

template <typename Ctx, size_t L>
BaseHashDigest<Ctx, L>::BaseHashDigest() {
  digest_.fill(0);
}

template <typename Ctx, size_t L>
BaseHashDigest<Ctx, L>::BaseHashDigest(const BaseHashDigest& rhs) {
  *this = rhs;
}

template <typename Ctx, size_t L>
BaseHashDigest<Ctx, L>& BaseHashDigest<Ctx, L>::operator=(
    const BaseHashDigest& rhs) {
  digest_ = rhs.digest_;
  return *this;
}

template <typename Ctx, size_t L>
void BaseHashDigest<Ctx, L>::reset(const std::string& buf) {
  init();
  update(buf);
  final();
}

template <typename Ctx, size_t L>
std::string BaseHashDigest<Ctx, L>::hexdigest() const {
  char buffer[LENGTH * 2 + 1];
  static const char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  for (int j = 0; j < LENGTH; j++) {
    buffer[j*2] = hexval[((digest_[j] >> 4) & 0xF)];
    buffer[(j*2) + 1] = hexval[(digest_[j]) & 0x0F];
  }
  buffer[LENGTH * 2] = '\0';
  return buffer;
}

}  // namespace vobla

#endif  // VOBLA_HASH_H_
