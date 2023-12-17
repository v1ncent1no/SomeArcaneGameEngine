#pragma once

#include "kp_macroutils.h"

/**
 * TODO:
 * 	- [ ] Arena Alocator (Working on it)
 * 	- [ ] Fixed Buffer Allocator
 * 	- [ ] (Safety on/off) Allocator
 */

/**
 * `kp_alloctor_t` is unified memory allocator inteface.
 *
 * It's supposed to be passed with required methods:
 *     - alloc(void *allocator, size_t size)
 *     - free(void *allocator, void* ptr)
 *     - realloc(void *allocator, void *ptr, size_t newsize)
 */
typedef void *(*kp_alloc_func_ptr)(void*, size_t);
typedef void (*kp_free_func_ptr)(void*, void*);
typedef void *(*kp_realloc_func_ptr)(void*, void*, size_t);

typedef struct kp_alloctor {
    void *parant_allocator;
    kp_alloc_func_ptr alloc;
    kp_realloc_func_ptr realloc;
    kp_free_func_ptr free;
} kp_alloctor_t;

#ifndef KP_MALLOC_DEFAULT_INTEFACE
#define KP_MALLOC_DEFAULT_INTEFACE

#include <malloc.h>

inline void *__kp_std_malloc(void* _, size_t size) {
    return malloc(size);
}
inline void __kp_std_free(void* _, void* ptr) {
    free(ptr);
}
inline void *__kp_std_realloc(void* _, void* ptr, size_t size) {
    return realloc(ptr, size);
}

const kp_alloctor_t std_malloc_alloc = {
    .parant_allocator = NULL,
    .alloc = __kp_std_malloc,
    .free = __kp_std_free,
    .realloc = __kp_std_realloc
};
#endif // KP_MALLOC_DEFAULT_INTEFACE

typedef enum kp_memory_error {
    KP_MEMORY_ERROR_OK,
    KP_MEMORY_ERROR_REGION_ALLOC,
    KP_MEMORY_ERROR_ARENA_NOT_INITIALIZED,
    KP_MEMORY_ERROR_REGION_NOT_INITIALIZED,
} kp_memory_error_t;

struct __kp_arena_region {
    void *raw_memory;
    size_t local_size;
    void *pointer;
    struct __kp_arena_region *next;
};

#ifndef KP_ARENA_MIN_REGION_SIZE
#define KP_ARENA_MIN_REGION_SIZE 1024 * 640 // 640K is Enough For Anyone
#endif

// FIXME: rewrite it, so that custom allocators use kp_allocator_t interface
//        as a parant allocator;
// NOTE: consider keeping a pointer to the last allocated region, which can give
//       some *little pieace* of perfomance, as arena's deallocated all at once
typedef struct kp_arena_allocator {
    // kp_alloctor_t parent_alloc;
    size_t region_size;
    struct __kp_arena_region* root;
} kp_arena_t;

/**
 * Initializes the arena. It's optional right now, as you may just do that
 * manually.
 */
kp_memory_error_t kp_arena_init(kp_arena_t *arena, size_t region_size);
/**
 * Deletes a bunch of memory allocated by this exact kp_arena_t
 */
kp_memory_error_t kp_arena_deinit(kp_arena_t* arena);
/**
 * If there's enough space in the last region, return the address, otherwise
 * creates another and returns the address to int.
 */
kp_memory_error_t kp_arena_alloc(kp_arena_t* arena, size_t size, void** ptr);
/**
 * No-Op for arena allocator.
 */
kp_memory_error_t kp_arena_dealloc(kp_arena_t* arena, size_t size, void** ptr);
