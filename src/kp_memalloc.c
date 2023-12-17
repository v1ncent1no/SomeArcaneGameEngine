#include <assert.h>
#include "kp_memalloc.h"

/**
 * Initializes the arena. It's optional right now, as you may just do that
 * manually.
 */
kp_memory_error_t kp_arena_init(kp_arena_t *arena, size_t regions_size) {
    return KP_MEMORY_OK;
}

/**
 * Deletes a bunch of memory allocated by this exact kp_arena_t
 */
kp_memory_error_t kp_arena_deinit(kp_arena_t* arena) {
    return KP_MEMORY_OK;
}

/**
 * If there's enough space in the last region, return the address, otherwise
 * creates another and returns the address to int.
 */
kp_memory_error_t kp_arena_alloc(kp_arena_t* arena, size_t size, void** ptr) {
    return KP_MEMORY_OK;
}
