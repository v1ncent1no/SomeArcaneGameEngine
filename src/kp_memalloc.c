#include <assert.h>
#include "kp_memalloc.h"
#include "kp_macroutils.h"


// FIXME: rewrite it, so that custom allocators use kp_allocator_t interface
//        as a parant allocator;
#ifndef KP_MEMALLOC_CUSTOM_MALLOC
#include <malloc.h>

#define KP_MALLOC(size)          malloc(size)
#define KP_FREE(ptr)             free(ptr)
#define KP_REALLOC(ptr, newsize) realloc(ptr, newsize)
#endif

/**
 * @param `size` is a number of bytes to be allocated for one region
 */
static kp_memory_error_t kp_region_init(struct __kp_arena_region *region,
                                        size_t size) {
    region->raw_memory = KP_MALLOC(size);
    if (!region->raw_memory)
        return KP_MEMORY_ERROR_REGION_ALLOC;

    region->local_size = size;
    region->pointer = NULL;
    region->next = NULL;

    return KP_MEMORY_ERROR_OK;
}

/**
 * Initializes the arena.
 *
 * @param `arena` is a pointer to arena
 * @param `region_size`
 */
kp_memory_error_t kp_arena_init(kp_arena_t *arena, size_t region_size) {
    arena->region_size = region_size;
    arena->root = KP_MALLOC(sizeof(struct __kp_arena_region));

    kp_memory_error_t err = kp_region_init(
        arena->root,
        MAX(KP_ARENA_MIN_REGION_SIZE, region_size)
    );

    if (!err)
        return err;
    return KP_MEMORY_ERROR_OK;
}

/**
 * Deletes a bunch of memory allocated by this exact kp_arena_t
 */
kp_memory_error_t kp_arena_deinit(kp_arena_t* arena) {
    if (!arena)
        return KP_MEMORY_ERROR_ARENA_NOT_INITIALIZED;
    if (!arena->root)
        return KP_MEMORY_ERROR_REGION_NOT_INITIALIZED;

    struct __kp_arena_region *region = arena->root;
    while (region->next != NULL) {
        struct __kp_arena_region *next = region->next;
        KP_FREE(region);
        region = next;
    }

    return KP_MEMORY_ERROR_OK;
}

static kp_memory_error_t kp_region_alloc_recursive(
    struct __kp_arena_region* region,
    size_t min_size,
    size_t size,
    void **ptr
) {
    if (!region) return KP_MEMORY_ERROR_REGION_NOT_INITIALIZED;

    while (region->next)
        region = region->next;

    if ((region->local_size - (size_t)region->pointer) > size) {
        region = KP_MALLOC(MAX(min_size, size));
        if (!region) return KP_MEMORY_ERROR_REGION_ALLOC;
    }

    *ptr = region->pointer;
    region->pointer += (size_t)region->pointer + size;

    return KP_MEMORY_ERROR_OK;
}

/**
 * If there's enough space in the last region, return the address, otherwise
 * creates another and returns the address to int.
 */
kp_memory_error_t kp_arena_alloc(kp_arena_t* arena, size_t size, void** ptr) {
    if (!arena)
        return KP_MEMORY_ERROR_ARENA_NOT_INITIALIZED;

    kp_memory_error_t err = kp_region_alloc_recursive(
        arena->root,
        arena->region_size,
        size,
        ptr
    );
    if (!err) return err;

    return KP_MEMORY_ERROR_OK;
}
