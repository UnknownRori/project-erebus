/**
 * @file types.hpp
 * @author UnknownRori (68576836+UnknownRori@users.noreply.github.com)
 * @brief header file that include custom types
 * @version 0.1
 * @date 2023-04-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef UNKNOWNRORI_PRIMITIVES_TYPES_HPP
#define UNKNOWNRORI_PRIMITIVES_TYPES_HPP

#include <tuple>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef double f32;
typedef long double f64;

template <typename T, typename Y>
using Result = std::tuple<T, Y>;

#endif