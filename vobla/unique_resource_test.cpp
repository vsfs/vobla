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

#include <gtest/gtest.h>
#include "vobla/unique_resource.h"

namespace vobla {

class TestResource : public Resource {
 public:
  explicit TestResource(int *count) : count_(count) {
  }

  ~TestResource() = default;

  void acquire() {
    (*count_)++;
  }

  void release() {
    (*count_)++;
  }

 private:
  int* count_;
};

/*
 * It also works for the class that is not inherited from vobla::Resource, as
 * long as it has 'acquire()' and 'release()' methods.
 */
class NonInheritResource {
 public:
  explicit NonInheritResource(int *count) : count_(count) {
  }

  ~NonInheritResource() = default;

  void acquire() {
    (*count_)--;
  }

  void release() {
    (*count_)--;
  }

 private:
  int* count_;
};

typedef UniqueResource<TestResource> TestResourceGuard;
typedef UniqueResource<NonInheritResource> NonInheritResourceGuard;

TEST(UniqueResourceTest, TestConstructor) {
  {
    TestResourceGuard guard;
  }

  int count = 0;
  TestResource rsc(&count);
  for (int i = 0; i < 10; i++) {
    TestResourceGuard guard(&rsc);
  }
  EXPECT_EQ(20, count);

  count = 0;
  NonInheritResource ni_rst(&count);
  for (int i = 0; i < 5; i++) {
    NonInheritResourceGuard guard(&ni_rst);
  }
  EXPECT_EQ(-10, count);
}

}  // namespace vobla
