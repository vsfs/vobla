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

#include <stddef.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "vobla/clock.h"
#include "vobla/timer.h"

namespace vobla {

static double kMicroSecond = 1000000;

TimerInterface::~TimerInterface() {
}

double TimerInterface::get_in_second() const {
  return get_in_ms() / kMicroSecond;
}

namespace {

// Calculate the interval between start and end time.
double get_delta_time(const timeval& start, const timeval& end) {
  return (end.tv_sec - start.tv_sec) * kMicroSecond
      + (end.tv_usec - start.tv_usec);
}

}  // anonymous namespace

//------ Timer -------
Timer::Timer() : clock_(Clock::real_clock()) {
}

Timer::Timer(Clock* clock) : clock_(clock) {
}

Timer::~Timer() {
}

void Timer::start() {
  start_ = clock_->now();
}

void Timer::stop() {
  end_ = clock_->now();
}

double Timer::get_in_ms() const {
  return (end_ - start_) * kMicroSecond;
}

CumulatedTimer::~CumulatedTimer() {
}

void CumulatedTimer::start() {
  Timer::start();
}

void CumulatedTimer::stop() {
  Timer::stop();
  cumulated_milli_seconds_ += Timer::get_in_ms();
}

double CumulatedTimer::get_in_ms() const {
  return cumulated_milli_seconds_;
}

void CumulatedTimer::reset() {
  cumulated_milli_seconds_ = 0;
}

UserAndSysUsageTimer::UserAndSysUsageTimer() : begin_(new struct rusage),
    end_(new struct rusage) {
}

UserAndSysUsageTimer::~UserAndSysUsageTimer() {
}

void UserAndSysUsageTimer::start() {
  getrusage(RUSAGE_SELF, begin_.get());
}

void UserAndSysUsageTimer::stop() {
  getrusage(RUSAGE_SELF, end_.get());
}

double UserAndSysUsageTimer::get_in_ms() const {
  return user_time_in_ms() + sys_time_in_ms();
}

double UserAndSysUsageTimer::user_time_in_ms() const {
  return get_delta_time(begin_->ru_utime, end_->ru_utime);
}

double UserAndSysUsageTimer::user_time_in_second() const {
  return user_time_in_ms() / kMicroSecond;
}

double UserAndSysUsageTimer::sys_time_in_ms() const {
  return get_delta_time(begin_->ru_stime, end_->ru_stime);
}

double UserAndSysUsageTimer::sys_time_in_second() const {
  return sys_time_in_ms() / kMicroSecond;
}

}  // namespace vobla
