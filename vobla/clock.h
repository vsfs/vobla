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

/**
 * \file vobla/clock.h
 * \brief Convinient classes to access the wall clock.
 */

#ifndef VOBLA_CLOCK_H_
#define VOBLA_CLOCK_H_

#include <boost/utility.hpp>

namespace vobla {

/**
 * \class Clock
 * \brief The basic interface of accessing wall clock time.
 */
class Clock : boost::noncopyable {
 public:
  Clock() = default;

  virtual ~Clock() {}

  /// Returns the global wall-time clock
  static Clock* real_clock();

  /// Returns the current timestamp.
  virtual double now() = 0;

  virtual void sleep(double seconds) = 0;
};

/**
 * \class FakeClock
 * \brief A fake clock object that is easy to be used in dependancy injection
 * to manipulate the output of time.
 */
class FakeClock : public Clock {
 public:
  /// Constract a FakeClock at time 0.0.
  explicit FakeClock(double start = 0);

  /// Advances the time for given seconds.
  void advance(double second);

  /// Returns the "faked" now.
  virtual double now();

  /// Sleep a faked time in seconds.
  virtual void sleep(double seconds);

 private:
  double second_;
};

}  // namespace vobla

#endif  // VOBLA_CLOCK_H_
