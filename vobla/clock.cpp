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
 * \file vobla/clock.cpp
 * \brief implementation of walltime clocks
 */

#include <sys/time.h>
#include <unistd.h>
#include "vobla/clock.h"

namespace vobla {

/**
 * \brief Real wall clock.
 */
class RealClock : public Clock {
 public:
  RealClock() = default;

  double now();

  void sleep(double seconds);
};

double RealClock::now() {
  timeval curtime;
  gettimeofday(&curtime, NULL);
  return curtime.tv_sec + static_cast<double>(curtime.tv_usec) / 1000000;
}

void RealClock::sleep(double seconds) {
  usleep(seconds * 1000000);
}

static RealClock* global_clock = new RealClock;

// static
Clock* Clock::real_clock() {
  return global_clock;
}

// --- Fake Clock ----

FakeClock::FakeClock(double start) : second_(start) {
}

void FakeClock::advance(double seconds) {
  second_ += seconds;
}

double FakeClock::now() {
  return second_;
}

void FakeClock::sleep(double seconds) {
  advance(seconds);
}

}  // namespace vobla
