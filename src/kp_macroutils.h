#pragma once

////////////////////////////////////////////////////////////////////////////////
/// Short Primitive Types Definitions

#include <stdint.h>
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

#include <stdio.h>
#include <stdlib.h>

#define KP_STATEMENT(S) \
    do { S; } while (0);

#ifdef NDEBUG
    #define KP_ASSERT(C)
    #define KP_LOG(MSG)
    #define KP_LOG_ASSERT(C, MSG)
#else
    #define KP_LOG(MSG) \
        fprintf(stderr, "[%s:%s:%d] %s", __FILE_NAME__, __FUNCTION__, \
                __LINE__, MSG);

    #ifdef KP_USE_LIBC_ASSERT
        #define KP_ASSERT(C) \
            assert(C);
        #define KP_LOG_ASSERT(C, MSG) \
            KP_STATEMENT(if (!(C)) { KP_LOG(MSG); abort(); })

    #else
        #define KP_ASSERT_BREAK() \
            *(int*)0 = 0;
        #define KP_ASSERT(C) \
            KP_STATEMENT(if (!(C)) { KP_ASSERT_BREAK(); })
        #define KP_LOG_ASSERT(C, MSG) \
            KP_STATEMENT(if (!(C)) { KP_LOG(MSG); KP_ASSERT_BREAK(); })
    #endif
#endif

#define KP_ARRAY_LENGTH(A) (sizeof(A) / sizeof(A[0]))

#define __KP_STRINGIFY(S) #S
#define KP_STRINGIFY(S) __STRINGIFY(S)

#define __KP_STRING_CONCAT(A, B) A ## B
#define KP_STRING_CONCAT(A, B) __KP_STRING_CONCAT(A, B)

#define KP_OFFSET_OF(T, m) (size_t) ((char*)(((T*)0)->m) - (char*)0)

#define KP_TODO \
    KP_LOG_ASSERT(false, "UNIMPLEMENTED")
