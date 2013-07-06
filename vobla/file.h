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

#ifndef VOBLA_FILE_H_
#define VOBLA_FILE_H_

#include <boost/utility.hpp>
#include <algorithm>
#include <string>
#include "vobla/status.h"

using std::string;

namespace vobla {

/**
 * \class File "vobla/file.h"
 * \brief The representation of an opened file.
 *
 * It exclusively owns the file descriptor of the opened file. When the File
 * object destroys, the file descriptor that it owns is closed automatically.
 *
 * Usage:
 * ~~~~~~~~~{cpp}
 * File file = File::open("/foo/bar", O_WRONLY);
 * if (file.fd() == -1) {
 *    LOG(ERROR) << "Failed to open file.";
 *    // Error handling...
 * }
 * ::write(file.fd(), buf, size);
 * ...
 * ~~~~~~~~~
 */
class File : boost::noncopyable {
 public:
  /**
   * \brief Open a file and return the File object.
   *
   * \note The caller needs to check the fd(), if `fd == -1`, then there is
   * some error occurred.
   */
  static File open(const string& path, int flags, mode_t mode = 0644);

  /// Construct a new file object.
  File();

  /**
   * \brief Constructs an File object with the file path and open flags.
   *
   * It does not open file in the constructor. You need to call File::open()
   * later to actually open the file.
   */
  File(const string& path, int flags, mode_t mode = 0644);

  /// Move constructor.
  File(File&&);

  /// Move assignment operator.
  File& operator=(File&& rhs);

  /// Destruct the file object.
  virtual ~File();

  /**
   * \brief Gets the file descriptor of the file.
   * \return -1 if the file was closed or the open operation failed.
   */
  int fd() const;

  /// Opens a file without mode.
  Status open();

  /// Close the file and release the file descriptor.
  Status close();

  /// Swap another file with this file.
  void swap(File& other);

  /// Returns the file descriptor and release the ownership.
  int release();

 private:
  /// File Descriptor.
  int fd_;

  /// The directory of this path.
  string path_;

  /// File open flags.
  int flags_;

  /// File open mode.
  mode_t mode_;
};

void swap(File& lhs, File& rhs);

/**
 * \class TemporaryFile "vobla/file.h"
 * \brief Temporary file.
 *
 * Usage:
 * ~~~~~~~~~{cpp}
 * TemporaryFile tmpfile;
 * FooDB test_db(tmpfile.path());  // Use this temporary file path.
 * ~~~~~~~~~
 */
class TemporaryFile : public File {
 public:
  enum class ScopeOp {
    /// Keeps the temporary file after this object being destructed.
    KEEP,
    /// Deletes the temporary file when it being destructed.
    DELETE
  };

  /// Constructor of a TemporaryFile.
  explicit TemporaryFile(ScopeOp op = ScopeOp::DELETE);

  /// Destructor.
  virtual ~TemporaryFile();

  /// Returns the path of this temporary file.
  const string& path() const {
    return path_;
  }

 private:
  ScopeOp op_;

  string path_;
};

/**
 * \brief Temporary directory.
 *
 * By default (op_ == ScopeOp::DELETE), the temporary directory will be
 * recursively deleted when this object being destructs.
 */
class TemporaryDirectory : boost::noncopyable {
 public:
  enum class ScopeOp {
    /// Keeps the temporary directory after this object being destructed.
    KEEP,
    /// Recursively deletes the temporary directory when it being destructed.
    DELETE
  };

  // TODO(eddyxu): add prefix as parameters.
  explicit TemporaryDirectory(ScopeOp op = ScopeOp::DELETE);

  /// Destructor of TemporaryDirectory.
  virtual ~TemporaryDirectory();

  /// Move constructor
  TemporaryDirectory(TemporaryDirectory&& rhs);

  /// Move assignment.
  TemporaryDirectory& operator=(TemporaryDirectory&& rhs);

  const string& path() const {
    return path_;
  }

 private:
  /// Removes the directory recursively.
  void remove_directory();

  ScopeOp op_;
  string path_;
};

}  // namespace vobla

#endif  // VOBLA_FILE_H_
