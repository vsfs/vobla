/*
 * Copyright 2012 (c) Lei Xu <eddyxu@gmail.com>
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
 * \file traits.h
 *
 * \brief Provide an convinient way to do type->int and int->type conversions.
 */

#ifndef VOBLA_TRAITS_H_
#define VOBLA_TRAITS_H_

#include <cstdint>  // NOLINT
#include <string>

/// The integer ID for basic C++ types.
enum TypeIDs {
  UNKNOWN, CHAR, UINT8, INT8, INT16, UINT16, INT32, UINT32, INT64,
  UINT64, FLOAT, DOUBLE, STRING
};

/**
 * \class TypeToInt
 * \brief A mapping between C++ type to int.
 * \tparam T A C++ type.
 *
 * To get the int value (a.k.a ID) of a type:
 * \code
 *    int type_id = TypeToInt<int32_t>::value;
 * \endcode
 *
 * \see TypeIDs
 */
template<typename T>
struct TypeToInt {
  enum { value_ = 0 };
};

/**
 * \class Int2Type
 * \brief A mapping from int to C++ types.
 * \tparam I an integer.
 *
 * To get the type represented by the given integer.
 * \code
 *   Int2Type<STRING>::type a_str = "Hello World";
 * \endcode
 *
 * \see TypeIDs for the available integers.
 */
template<TypeIDs I>
struct Int2Type {
  enum { value_ = I };
  typedef int type;

  static int value() { return value_; }
};

/**
 * \brief Parses the type string to integer.
 * \see TypeIDs
 */
int parse_type_string_to_int(const std::string &str);

/**
 * \brief Returns a string representation for a type.
 * \param type_id the integer ID of the type.
 */
std::string type_int_to_string(int type_id);

/// \cond

/**
 * \brief Defines the mapping from C++ type to its integer ID.
 */
#define DEFINE_INT2TYPE(type, intval) \
    template <> struct TypeToInt<type> { \
      enum { value_ = intval }; \
      static int value() { return value_; } \
    }

DEFINE_INT2TYPE(char, CHAR);
DEFINE_INT2TYPE(int8_t, INT8);
DEFINE_INT2TYPE(uint8_t, UINT8);
DEFINE_INT2TYPE(int16_t, INT16);
DEFINE_INT2TYPE(uint16_t, UINT16);
DEFINE_INT2TYPE(int32_t, INT32);
DEFINE_INT2TYPE(uint32_t, UINT32);
DEFINE_INT2TYPE(int64_t, INT64);
DEFINE_INT2TYPE(uint64_t, UINT64);
DEFINE_INT2TYPE(float, FLOAT);
DEFINE_INT2TYPE(double, DOUBLE);
DEFINE_INT2TYPE(std::string, STRING);

template<> struct Int2Type<CHAR> {
  typedef char type;
};

template<> struct Int2Type<INT8> {
  typedef int8_t type;
};

template<> struct Int2Type<UINT8> {
  typedef uint8_t type;
};

template<> struct Int2Type<INT16> {
  typedef int16_t type;
};

template<> struct Int2Type<UINT16> {
  typedef uint16_t type;
};

template<> struct Int2Type<INT32> {
  typedef int32_t type;
};

template<> struct Int2Type<UINT32> {
  typedef uint32_t type;
};

template<> struct Int2Type<INT64> {
  typedef int64_t type;
};

template<> struct Int2Type<UINT64> {
  typedef uint64_t type;
};

template<> struct Int2Type<FLOAT> {
  typedef float type;
};

template<> struct Int2Type<DOUBLE> {
  typedef double type;
};

template<> struct Int2Type<STRING> {
  typedef typename std::string type;
};
/// \endcond

#endif  // VOBLA_TRAITS_H_
