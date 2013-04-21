/*
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

#include <fcntl.h>
#include <glog/logging.h>
#if defined(linux) || defined(__linux__)
#include <linux/fs.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <sys/disk.h>
#include <libproc.h>
#endif /* __APPLE__ */
#include <string>
#include "vobla/sysinfo.h"

namespace vobla {

using std::string;
const int BUFSIZE = 1024;

static double cpufreq_ = 0;

double SysInfo::cpu_freq() {
  if (cpufreq_)
    return cpufreq_;

  FILE *fp;
  char buffer[BUFSIZE];
#if defined(linux) || defined(__linux__)
  if ((fp = fopen("/proc/cpuinfo", "r")) == NULL) {
    perror("get_cpu_freq: Can not get cpu information");
    return 0;
  }

  for (int i = 6; i > 0; i--) {
    if (fgets(buffer, BUFSIZE-1, fp) == NULL) {
      perror("get_cpu_freq: can not get cpu information");
      fclose(fp);
      return 0;
    }
  }

  if (fscanf(fp, "%*s%*s%*s%lf", &cpufreq_) != 1) {
    perror("get_cpu_freq: can not get cpu information");
    fclose(fp);
    return 0;
  }
  cpufreq_ *= 1000000;
#elif defined(__APPLE__) || defined(__NetBSD__) || defined(__FreeBSD__)

  if ((fp = popen("sysctl -a", "r")) == NULL) {
    perror("cpu_freq(): get cpu frequency");
    return 0;
  }

  while (fgets(buffer, BUFSIZE-1, fp) != NULL) {
#if defined(__APPLE__)
    if (strnstr(buffer, "hw.cpufrequency:", BUFSIZE) == NULL)
#else
    if (strnstr(buffer, "dev.cpu.0.freq:", BUFSIZE) == NULL)
#endif /* FreeBSD & NetBSD */
      continue;

    sscanf(buffer, "%*s%lf", &cpufreq_);  // NOLINT
#if defined(__FreeBSD__) || defined(__NetBSD__)
    cpufreq_ *= 1000000;
#endif /* FreeBSD / NetBSD */
    break;
  };
#endif /* __APPLE__ || __FreeBSD__ || __NetBSD__ */
  fclose(fp);
  return cpufreq_;
}


/**
 * Get available number of CPUs
 *
 * @see http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
 */
int SysInfo::get_num_cpus() {
  int num_cpus = 0;
#if defined(linux) || defined(__linux__) || \
  defined(__APPLE__) || defined(__FreeBSD__)
  num_cpus= sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(WIN32)
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  num_cpus = sysinfo.dwNumberOfProcessors;
#endif
  return num_cpus;
}

/*
inline uint64_t rdtsc() {
  uint32_t lo, hi;
  __asm__ __volatile__
    ("rdtsc" : "=a"(lo), "=d"(hi));
  return (uint64_t) hi << 32 | lo;
}

double get_cur_time(void) {
  return rdtsc() / cpufreq_;
}
*/

pid_t SysInfo::get_parent_pid(pid_t pid) {
  if (pid == 0) {
    return 0;
  }
#if defined(__APPLE__)
  struct kinfo_proc info;
  size_t length = sizeof(struct kinfo_proc);
  int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, pid };
  if (sysctl(mib, 4, &info, &length, NULL, 0) < 0)
    return -1;
  if (length == 0)
    return -1;
  return info.kp_eproc.e_ppid;
#elif defined(__linux__)
  char proc_filepath[BUFSIZE];
  char buffer[BUFSIZE];
  pid_t parent = -1;
  memset(proc_filepath, 0, BUFSIZE);
  snprintf(proc_filepath, BUFSIZE, "/proc/%d/status", pid);
  FILE * fp = fopen(proc_filepath, "r");
  while (fgets(buffer, BUFSIZE, fp) != NULL) {
    if (strncmp(buffer, "PPid:", 5) == 0) {
      sscanf(buffer, "PPid: %d", &parent);  // NOLINT
      break;
    }
  }
  fclose(fp);
  return parent;
#endif /* __linux__ */
}

int SysInfo::get_process_name(pid_t pid, string *name) {
#if defined(__APPLE__)
  char buffer[BUFSIZE];
  int ret = 0;
  ret = proc_name(pid, buffer, BUFSIZE);
  if (ret) {
    return ret;
  }
#elif defined(__linux__)
  char proc_filepath[BUFSIZE];
  char line[BUFSIZE];
  memset(proc_filepath, 0, BUFSIZE);
  memset(line, 0, BUFSIZE);
  snprintf(proc_filepath, BUFSIZE, "/proc/%d/cmdline", pid);
  FILE *fp = fopen(proc_filepath, "r");
  if (!fp) {
    perror("get_process_name: failed to open /proc/<pid>/cmdline");
    return -1;
  }
  if (fgets(line, BUFSIZE, fp) == NULL) {
    perror("get_process_name: failed to extract cmdline from "
           "/proc/<pid>/cmdline");
    fclose(fp);
    return -1;
  }
  char *pos = strrchr(line, '/');
  if (pos) {
    *name = pos+1;
  } else {
    *name = line;
  }
  fclose(fp);
#else  /* __linux__ */
#error "Unsupported platform"
#endif
  return 0;
}

bool SysInfo::is_block_device(const string &dev_path) {
  struct stat stbuf;
  if (stat(dev_path.c_str(), &stbuf) < 0) {
    fprintf(stderr, "SysInfo::is_block_device: stat file %s: %s\n",
            dev_path.c_str(), strerror(errno));
    return false;
  }

  return S_ISBLK(stbuf.st_mode);
}

off_t SysInfo::get_block_device_size(const string &dev_path) {
  off_t num_bytes = 0;

  int fd;
  fd = open(dev_path.c_str(), O_RDONLY);
  if (fd < 0) {
    fprintf(stderr, "SysInfo::get_block_device_size: failed to open device [%s]"
            ": %s\n", dev_path.c_str(), strerror(errno));
    return -1;
  }

#if defined(linux)
  if (ioctl(fd, BLKGETSIZE64, &num_bytes) < 0) {
    fprintf(stderr, "SysInfo::get_block_device_size: iotcl BLKGETSIZE64 %s"
            ": %s\n", dev_path.c_str(), strerror(errno));
    return -1;
  }
#elif defined(__APPLE__)
  uint32_t blocksize;
  uint64_t blockcount;
  if (ioctl(fd, DKIOCGETBLOCKSIZE, &blocksize) < 0) {
    fprintf(stderr, "SysInfo::get_block_device_size: "
            "ioctl DKIOCGETBLOCKSIZE %s: %s\n", dev_path.c_str(),
            strerror(errno));
    return -1;
  }
  if (ioctl(fd, DKIOCGETBLOCKCOUNT, &blockcount) < 0) {
    fprintf(stderr, "SysInfo::get_block_device_size: "
            "ioctl DKIOGETBLOCKCOUNT %s: %s\n", dev_path.c_str(),
            strerror(errno));
    return -1;
  }
#else
  LOG(FATAL) << "Not implemented.";
#endif

  close(fd);
  return num_bytes;
}

off_t SysInfo::get_file_size(const string &file_path) {
  struct stat stbuf;
  if (stat(file_path.c_str(), &stbuf) < 0) {
    fprintf(stderr, "SysInfo::get_file_size: stat file %s: %s\n",
            file_path.c_str(), strerror(errno));
    return -1;
  }
  CHECK(S_ISREG(stbuf.st_mode));
  return stbuf.st_size;
}

off_t SysInfo::get_file_or_device_size(const string &file_path) {
  struct stat stbuf;
  if (stat(file_path.c_str(), &stbuf) < 0) {
    fprintf(stderr, "SysInfo::get_file_size: stat file %s: %s\n",
            file_path.c_str(), strerror(errno));
    return -1;
  }
  if (S_ISREG(stbuf.st_mode)) {
    return stbuf.st_size;
  } else if (S_ISBLK(stbuf.st_mode)) {
    return get_block_device_size(file_path);
  } else {
    LOG(FATAL) << "Unsupport file type: " << stbuf.st_mode;
  }
  return -1;
}

}  // namespace vobla
