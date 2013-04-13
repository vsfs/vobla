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
 *
 * \file timer.h
 * \brief Provides a timer for conveniently timing.
 */

#ifndef VOBLA_TIMER_H_
#define VOBLA_TIMER_H_

#include <boost/noncopyable.hpp>
#include <memory>

class Clock;
struct rusage;

/**
 * \class TimerInterface
 * \brief defines the interface required by a timer
 */
class TimerInterface : public boost::noncopyable {
 public:
  TimerInterface();

  virtual ~TimerInterface();

  /// Starts timing.
  virtual void start() = 0;

  /// Stops timing.
  virtual void stop() = 0;

  /// Gets the time in micro seconds.
  virtual double get_in_ms() const = 0;

  /// Gets the time consumed in seconds.
  virtual double get_in_second() const;
};

/**
 * \class Timer
 * \brief A wall time timer.
 *
 * It internally calls gettimeofday().
 */
class Timer : public TimerInterface {
 public:
  /// Constructs a Timer from the Clock::real_clock().
  Timer();

  /**
   * \brief Constructs a Timer from a given clock.
   *
   * It is mainly useful for the dependancy injection.
   */
  explicit Timer(Clock* clock);

  virtual ~Timer();

  /// Starts timing.
  virtual void start();

  /// Stops timing.
  virtual void stop();

  /**
   * \brief Gets the time consumed in microseconds.
   *
   * Note that 'get_in_second()' is implemented in TimerInterface.
   */
  virtual double get_in_ms() const;

 protected:
  /// Start time.
  double start_;

  /// End time.
  double end_;

  /// The Clock object to get timestamp.
  Clock* clock_;
};

/**
 * \class CycleTimer
 * \brief It uses realtime clock (rtsc) to get time.
 */
class CycleTimer : public TimerInterface {
 public:
  CycleTimer();

  virtual ~CycleTimer();

  /// Starts counting the clocks.
  void start();

  /// Stops counting the clocks.
  void stop();

  /// Gets the time consumed in microseconds.
  virtual double get_in_ms() const;
};

/**
 * \class CumulatedTimer
 * \brief a timer that can be called multiple times and cumulates
 * all time intervals together.
 */
class CumulatedTimer : public Timer {
 public:
  CumulatedTimer();

  virtual ~CumulatedTimer();

  /**
   * \brief Starts to record one time period.
   *
   * Note: It can be called mulitple time without clearing the internal
   * status. To reset the timer, call 'reset()' before 'start()'.
   */
  virtual void start();

  /**
   * \brief Ends one time recording and addes the interval to the cumulated
   * results
   */
  virtual void stop();

  /// Returns the cumulated microseconds of this timer.
  virtual double get_in_ms() const;

  /// Resets the internal timer.
  virtual void reset();

 private:
  double cumulated_milli_seconds_;
};

/**
 * \class UserAndSysUsageTimer
 * \brief Gets the user time and system time used during the execution.
 */
class UserAndSysUsageTimer : public TimerInterface {
 public:
  UserAndSysUsageTimer();

  virtual ~UserAndSysUsageTimer();

  /// Starts timing the user and system time.
  virtual void start();

  /// Stops timing the user and system time.
  virtual void stop();

  /// Gets total time (user+sys) in microseconds.
  virtual double get_in_ms() const;

  /// Gets user time in microseconds.
  virtual double user_time_in_ms() const;

  /// Gets user time in seconds.
  virtual double user_time_in_second() const;

  /// Gets system time in microseconds.
  virtual double sys_time_in_ms() const;

  /// Gets system time in seconds.
  virtual double sys_time_in_second() const;

 private:
  std::unique_ptr<rusage> begin_;
  std::unique_ptr<rusage> end_;
};

#endif  // VOBLA_TIMER_H_
