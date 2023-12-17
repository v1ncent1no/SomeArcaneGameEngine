#pragma once

#include "kp_macroutils.h"

/**
 * TODO:
 * 	- [ ] Arena Alocator (Working on it)
 * 	- [ ] Fixed Buffer Allocator
 * 	- [ ] (Safety on/off) Allocator
 *
 */

typedef enum KP_MEMORY_ERROR {
    KP_ERROR_OK
} KP_MEMORY_ERROR;

struct __kp_arena_region {
    u8 *raw_memory;
    size_t local_size;
    size_t pointer;
    struct __kp_arena_region *next;
};

typedef struct kp_arena_allocator {
    size_t region_size;
    struct __kp_arena_region* root;
} kp_arena_t;

/**
 * Initializes the arena. It's optional right now, as you may just do that
 * manually.
 */
KP_MEMORY_ERROR kp_arena_init(kp_arena_t *arena, size_t regions_size);
/**
 * Deletes a bunch of memory allocated by this exact kp_arena_t
 */
KP_MEMORY_ERROR kp_arena_deinit(kp_arena_t* arena);

/**
 * If there's enough space in the last region, return the address, otherwise
 * creates another and returns the address to int.
 */
KP_MEMORY_ERROR kp_arena_alloc(kp_arena_t* arena, size_t size, void** ptr);
