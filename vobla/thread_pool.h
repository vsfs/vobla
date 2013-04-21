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
 * \file thread_pool.h
 * \brief A generaic threads pool.
 */

#ifndef VOBLA_THREAD_POOL_H_
#define VOBLA_THREAD_POOL_H_

#include <condition_variable>
#include <functional>
#include <future>  // NOLINT
#include <mutex>  // NOLINT
#include <queue>
#include <thread>  // NOLINT
#include <vector>
#include "vobla/macros.h"
#include "vobla/status.h"

namespace vobla {

/**
 * \class ThreadPool
 * \brief A simple FIFO-queue based thread pool.
 */
class ThreadPool {
 public:
  typedef Status ReturnType;

  typedef std::function<ReturnType()> TaskType;

  typedef std::future<ReturnType> FutureType;

  /// Constructs a thread pool with `2 * num_cpus` threads.
  ThreadPool();

  /// Constructs a thread pool with 'num_threads' threads.
  explicit ThreadPool(size_t num_threads);

  virtual ~ThreadPool();

  /// Stops this pool and waits all threads to finish.
  virtual void stop();

  /**
   * \brief Add a task to the task queue.
   *
   * It can add function, functional object and lambda.
   */
  virtual FutureType add_task(TaskType task);

 private:
  void worker();

  size_t num_threads_;

  std::vector<std::thread> threads_;

  typedef std::packaged_task<ReturnType()> PackagedTask;

  std::queue<PackagedTask> task_queue_;

  std::mutex mutex_;

  std::condition_variable condition_;

  bool has_stoped_;

  DISALLOW_COPY_AND_ASSIGN(ThreadPool);
};

}  // namespace vobla

#endif  // VOBLA_THREAD_POOL_H_