#pragma once

#include "kp_macroutils.h"

/**
 * This is dynamic array implementation that should imitate c++'s std::vector.
 *
 * It's supposed to be dependant on kp_memalloc.h's allocator, to give user much
 * more control of the memory.
 */
