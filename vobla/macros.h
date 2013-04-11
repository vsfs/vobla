/**
 * \brief defines widely used macros
 *
 * Copyright 2011 (c) Lei Xu <eddyxu@gmail.com>
 */

#ifndef VOBLA_MACROS_H_
#define VOBLA_MACROS_H_

#define DISABLE_COPY_AND_ASSIGN(ClassName) \
  ClassName(const ClassName&); \
  ClassName& operator=(const ClassName&);

#define DISABLE_IMPLICIT_CONSTRUCTORS(ClassName) \
  ClassName(); \
  DISABLE_COPY_AND_ASSIGN(ClassName)

// Refer to facebook's folly library.
#define ONE_OR_NONE(a, ...) THIRD(a, ## __VA_ARGS__, a)
#define THIRD(a, b, ...) __VA_ARGS__

#endif  // VOBLA_MACROS_H_
