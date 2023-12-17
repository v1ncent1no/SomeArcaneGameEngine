#pragma once

#include "kp_macroutils.h"

/**
 * This is dynamic array implementation that should imitate c++'s std::vector.
 *
 * It's supposed to be dependant on kp_memalloc.h's allocator, to give user much
 * more control of the memory.
 */

typedef enum kp_dynlist_error {
    KP_DYNLIST_OK
} kp_dynlist_error_t;

typedef struct kp_dynlist_t {
    void *ptr;
    size_t elem_size;
    size_t length;
} kp_dynlist_t;
