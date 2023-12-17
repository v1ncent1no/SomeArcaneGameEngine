#pragma once

////////////////////////////////////////////////////////////////////////////////
/// Short Primitive Types Definitions

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

////////////////////////////////////////////////////////////////////////////////
/// Helper Macros

#include "kp_log.h"

#define KP_STATEMENT(S) \
    do { S; } while (0);

#ifdef _STDLIB_H
#define KP_ABORT \
    (*((char*)0) = 0)
#else
#define KP_ABORT \
    abort()
#endif

#ifdef NDEBUG
#define KP_ASSERT(C)
#else

#ifdef _ASSERT_H
#define KP_ASSERT(C) \
    assert(C);
#else
#define KP_ASSERT(C) \
    KP_STATEMENT(if (!(C)) { KP_ABORT; })
#endif

#endif

#define KP_ARRAY_LENGTH(A) (sizeof(A) / sizeof(A[0]))

#define __KP_STRINGIFY(S) #S
#define KP_STRINGIFY(S) __STRINGIFY(S)

#define __KP_STRING_CONCAT(A, B) A ## B
#define KP_STRING_CONCAT(A, B) __KP_STRING_CONCAT(A, B)

#define KP_MEMBER_OF(T, m) (size_t) (((T*)0)->m)
#define KP_OFFSET_OF(T, m) (size_t) ((char*) KP_MEMBER_OF(T, m) - (char*)0)
