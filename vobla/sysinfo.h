/**
 * Copyright 2011-2013 (c) Lei Xu <eddyxu@gmail.com>
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
 * \file vobla/sysinfo.h
 * \brief Get system information
 */

#ifndef VOBLA_SYSINFO_H_
#define VOBLA_SYSINFO_H_

#include <sys/types.h>
#include <string>
#include "vobla/macros.h"

namespace vobla {

/**
 * \class SysInfo.
 * \brief Access system information.
 */
class SysInfo {
 public:
  /// Gets CPU frequency.
  static double cpu_freq();

  /// Gets the total number of logical CPUs.
  static int get_num_cpus();

  /**
   * \brief Gets the parent process id of a given process.
   *
   * \param pid the ID of a running process
   * \return the parent process ID of the given process
   */
  static pid_t get_parent_pid(pid_t pid);

  /**
   * \brief Gets the process name (executable name) for a given  process.
   *
   * \param[in] pid the id of a running process
   * \param[out] name process name
   * \return 0 if success.
   */
  static int get_process_name(pid_t pid, std::string* name);

  static bool is_block_device(const std::string &dev_path);

  /**
   * \brief Get the size of the given block device.
   *
   * \param[in] dev_path the path of the block device.
   * \return the number of bytes of this block device. Returns -1 if an error
   * occurred.
   */
  static off_t get_block_device_size(const std::string &dev_path);

  /**
   * \brief Get file size
   * \return the size of a file, returns -1 if failed.
   * TODO(eddyxu): Move to another class.
   */
  static off_t get_file_size(const std::string &file_path);

  // Get the size of either a file or a block device.
  static off_t get_file_or_device_size(const std::string &file_path);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(SysInfo);
};

}  // namespace vobla

#endif  // VOBLA_SYSINFO_H_
