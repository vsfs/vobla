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

#include <gtest/gtest.h>
#include <memory>
#include <mutex>
#include <vector>
#include "vobla/status.h"
#include "vobla/sysinfo.h"
#include "vobla/thread_pool.h"

namespace vobla {

using std::mutex;

TEST(ThreadPoolTest, TestLambdas) {
  ThreadPool pool(4);
  int execute_count = 0;
  std::mutex mutex;
  for (int i = 0; i < 100; i++) {
    // Use lambda to demostrate
    pool.add_task([&mutex, &execute_count]() -> Status {
          std::unique_lock<std::mutex> lock(mutex);
          execute_count++;
          return Status::OK;
        });
  }
  pool.close();
  pool.join();
  EXPECT_EQ(100, execute_count);
}

TEST(ThreadPoolTest, TestJoinInDestructor) {
  int execute_count = 0;
  std::mutex mutex;
  {
    ThreadPool pool;
    for (int i = 0; i < 5; i++) {
      // Use lambda to demostrate
      pool.add_task([&mutex, &execute_count]() -> Status {
            std::unique_lock<std::mutex> lock(mutex);
            execute_count++;
            return Status::OK;
          });
    }
  }
  EXPECT_EQ(5, execute_count);
}

struct TestTask {
  int* global_count;
  std::mutex* global_mutex;

  TestTask(int *gc, std::mutex *gm) : global_count(gc), global_mutex(gm) {
  }

  Status operator()() {
    std::unique_lock<std::mutex> lock(*global_mutex);
    (*global_count)++;
    return Status::OK;
  }
};

TEST(ThreadPoolTest, TestTaskObjects) {
  ThreadPool pool;
  int execute_count = 0;
  std::mutex mutex;
  std::vector<TestTask> tasks;
  std::vector<ThreadPool::FutureType> results;
  for (int i = 0; i < 100; i++) {
    tasks.emplace_back(&execute_count, &mutex);
    results.emplace_back(pool.add_task(std::bind(tasks.back())));
  }
  pool.close();
  pool.join();
  for (auto& rst : results) {
    EXPECT_TRUE(rst.get().ok());
  }
  EXPECT_EQ(100, execute_count);
}

TEST(ThreadPoolTest, TestDefaultNumberThreads) {
  ThreadPool p1;
  EXPECT_EQ(2u * SysInfo::get_num_cpus(), p1.num_threads());

  ThreadPool p2(0);
  EXPECT_EQ(2u * SysInfo::get_num_cpus(), p2.num_threads());

  ThreadPool p3(3);
  EXPECT_EQ(3u, p3.num_threads());
}

}  // namespace vobla
