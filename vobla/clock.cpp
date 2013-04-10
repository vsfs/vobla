/**
 * \file clock.cpp
 * \brief implementation of walltime clocks
 * \author Lei Xu <eddyxu@gmail.com>
 *
 * Copyright 2011 (c) Lei Xu
 */

#include <sys/time.h>
#include <unistd.h>
#include "vobla/clock.h"

/**
 * \brief Real wall clock.
 */
class RealClock : public Clock {
 public:
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

Clock::~Clock() {
}

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
