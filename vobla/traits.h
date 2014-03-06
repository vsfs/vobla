/*
 * Copyright 2014 (c) Lei Xu <eddyxu@gmail.com>
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

#ifndef VOBLA_TRAITS_H_
#define VOBLA_TRAITS_H_

#include <cstdint>
#include <string>

/// The integer ID for basic C++ types.
enum TypeIDs {
  UNKNOWN, CHAR, UINT8, INT8, INT16, UINT16, INT32, UINT32, INT64,
  UINT64, FLOAT, DOUBLE, STRING
};

/**
 * \class TypeToInt vobla/traits.h
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
 * \class IntToType vobla/traits.h
 * \brief A mapping from int to C++ types.
 * \tparam I an integer.
 *
 * To get the type represented by the given integer.
 * \code
 *   IntToType<STRING>::type a_str = "Hello World";
 * \endcode
 *
 * \see TypeIDs for the available integers.
 */
template<TypeIDs I>
struct IntToType {
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
#define DEFINE_TYPE_WITH_INT(typeval, intval) \
    template <> struct TypeToInt<typeval> { \
      enum { value_ = intval }; \
      static int value() { return value_; } \
    }; \
    template <> struct IntToType<intval> { \
      typedef typeval type; \
    }


DEFINE_TYPE_WITH_INT(char, CHAR);
DEFINE_TYPE_WITH_INT(int8_t, INT8);
DEFINE_TYPE_WITH_INT(uint8_t, UINT8);
DEFINE_TYPE_WITH_INT(int16_t, INT16);
DEFINE_TYPE_WITH_INT(uint16_t, UINT16);
DEFINE_TYPE_WITH_INT(int32_t, INT32);
DEFINE_TYPE_WITH_INT(uint32_t, UINT32);
DEFINE_TYPE_WITH_INT(int64_t, INT64);
DEFINE_TYPE_WITH_INT(uint64_t, UINT64);
DEFINE_TYPE_WITH_INT(float, FLOAT);
DEFINE_TYPE_WITH_INT(double, DOUBLE);
DEFINE_TYPE_WITH_INT(std::string, STRING);

/// \endcond

#endif  // VOBLA_TRAITS_H_
