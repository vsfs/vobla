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
#include <unistd.h>
#include "vobla/timer.h"

namespace vobla {

TEST(TimerTest, TestGetTime) {
  Timer timer;
  timer.start();
  usleep(100);
  timer.stop();
  EXPECT_LE(0.0001f, timer.get_in_second());
  EXPECT_NEAR(timer.get_in_second(), timer.get_in_ms() / 1000000, 0.000001);
}

TEST(TimerTest, TestCumulatedTimer) {
  CumulatedTimer timer;
  for (int i = 1; i <= 10; i++) {
    timer.start();
    usleep(100);
    timer.stop();
    EXPECT_LE(0.0001f * i, timer.get_in_second());
  }
}

}  // namespace vobla
