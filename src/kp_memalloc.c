#include "kp_memalloc.h"

/**
 * Initializes the arena. It's optional right now, as you may just do that
 * manually.
 */
KP_MEMORY_ERROR kp_arena_init(kp_arena_t *arena, size_t regions_size) {
    return KP_ERROR_OK;
}

/**
 * Deletes a bunch of memory allocated by this exact kp_arena_t
 */
KP_MEMORY_ERROR kp_arena_deinit(kp_arena_t* arena) {
    return KP_ERROR_OK;
}

/**
 * If there's enough space in the last region, return the address, otherwise
 * creates another and returns the address to int.
 */
KP_MEMORY_ERROR kp_arena_alloc(kp_arena_t* arena, size_t size, void** ptr) {
    return KP_ERROR_OK;
}

