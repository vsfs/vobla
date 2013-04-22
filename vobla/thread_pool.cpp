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
 * \file thread_pool.cpp
 * \brief Thread pool implemetations.
 */

#include <thread>
#include "vobla/sysinfo.h"
#include "vobla/thread_pool.h"

namespace vobla {

using std::thread;
const size_t kDefaultThreadsPerCpu = 2;

ThreadPool::ThreadPool()
  : num_threads_(kDefaultThreadsPerCpu * SysInfo::get_num_cpus()),
    closed_(false) {
  for (size_t i = 0; i < num_threads_; ++i) {
    threads_.emplace_back(thread(&ThreadPool::worker, this));
  }
}

ThreadPool::ThreadPool(size_t num_threads)
  : num_threads_(num_threads), closed_(false) {
  if (num_threads_ == 0) {
    num_threads_ = kDefaultThreadsPerCpu * SysInfo::get_num_cpus();
  }
  for (size_t i = 0; i < num_threads_; ++i) {
    threads_.emplace_back(std::move(thread(&ThreadPool::worker, this)));
  }
}

ThreadPool::~ThreadPool() {
  if (!closed_) {
    close();
    join();
  }
}

void ThreadPool::close() {
  if (closed_) {
    return;
  }

  closed_ = true;
  condition_.notify_all();
}

void ThreadPool::join() {
  for (auto& thd : threads_) {
    thd.join();
  }
}

ThreadPool::FutureType ThreadPool::add_task(TaskType func) {
  FutureType future;
  PackagedTask task(func);
  {
    std::unique_lock<std::mutex> lock(mutex_);
    future = task.get_future();
    task_queue_.push(std::move(task));
  }
  condition_.notify_one();
  return future;
}

size_t ThreadPool::num_threads() const {
  return threads_.size();
}

void ThreadPool::worker() {
  while (true) {
    PackagedTask task;
    {
      std::unique_lock<std::mutex> lock(mutex_);

      while (!closed_ && task_queue_.empty()) {
        condition_.wait(lock);
      }

      if (closed_ && task_queue_.empty()) {
        break;
      }

      task = std::move(task_queue_.front());
      task_queue_.pop();
    }
    task();
  }
}

}  // namespace vobla
