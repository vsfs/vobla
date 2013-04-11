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
 * \brief defines widely used macros
 */

#ifndef VOBLA_MACROS_H_
#define VOBLA_MACROS_H_

#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
  ClassName(const ClassName&); \
  ClassName& operator=(const ClassName&);

#define DISALLOW_IMPLICIT_CONSTRUCTORS(ClassName) \
  ClassName(); \
  DISABLE_COPY_AND_ASSIGN(ClassName)

// Refer to facebook's folly library.
#define ONE_OR_NONE(a, ...) THIRD(a, ## __VA_ARGS__, a)
#define THIRD(a, b, ...) __VA_ARGS__

#endif  // VOBLA_MACROS_H_
