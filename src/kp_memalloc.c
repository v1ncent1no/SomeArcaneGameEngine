#include <assert.h>
#include "kp_memalloc.h"
#include "kp_macroutils.h"

////////////////////////////////////////////////////////////////////////////
/// Implementation of <malloc.h> memory allocator methods

#ifdef KP_MALLOC_DEFAULT_INTEFACE
#include <malloc.h>

void *__kp_std_malloc(void* _, size_t size) {
    return malloc(size);
}
void __kp_std_free(void* _, void* ptr) {
    free(ptr);
}
void *__kp_std_realloc(void* _, void* ptr, size_t size) {
    return realloc(ptr, size);
}
#endif // KP_MALLOC_DEFAULT_INTEFACE

////////////////////////////////////////////////////////////////////////////
/// `kp_arena_t` memory allocator's implementation

/**
 * @param `size` is a number of bytes to be allocated for one region
 */
static kp_memory_error_t kp_region_init(kp_allocator_t *allocator,
                                        struct __kp_arena_region *region,
                                        size_t size) {
    region->raw_memory = allocator->alloc(allocator->parent, size);
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
kp_memory_error_t kp_arena_init(kp_arena_t *arena, kp_allocator_t *parent,
                                size_t region_size) {
    arena->region_size = region_size;
    arena->root = parent->alloc(parent->parent,
                                sizeof(struct __kp_arena_region));

    kp_memory_error_t err = kp_region_init(
        parent,
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
        arena->parent.free(arena->parent.parent, region);
        region = next;
    }

    return KP_MEMORY_ERROR_OK;
}

static kp_memory_error_t kp_region_alloc_recursive(
    kp_allocator_t *allocator,
    struct __kp_arena_region* region,
    size_t min_size,
    size_t size,
    void **ptr
) {
    if (!region) return KP_MEMORY_ERROR_REGION_NOT_INITIALIZED;

    while (region->next)
        region = region->next;

    if ((region->local_size - (size_t)region->pointer) > size) {
        region = allocator->alloc(allocator->parent, MAX(min_size, size));
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
        &arena->parent,
        arena->root,
        arena->region_size,
        size,
        ptr
    );
    if (!err) return err;

    return KP_MEMORY_ERROR_OK;
}


////////////////////////////////////////////////////////////////////////////
/// TODO: To be continued later
