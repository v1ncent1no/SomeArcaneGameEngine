#pragma once

#include <stddef.h>
#include <stdio.h>

/**
 * Simple Logging *Module*
 *
 * Made just to reduce some boilerplate and make logging more comfortable and
 * unified to work with.
 *
 * There four main macros defined here:
 * 	- KP_LOG: for any kind of logging, you can choose watever log level you
 * 		  want, or make up your own
 * 	- KP_INFO: for information, things that might help keep track of the
 * 		   state of the code, and help with debugging
 * 	- KP_WARN: for warnings, things that you may want to double check
 * 	- KP_ERROR: for error, when program state is broke and unrecorable
 *
 * All these logging macros will be included only in debug builds, in release
 * builds, where NDEBUG is defined, it's a NOP that compiler will ignore.
 */

#ifdef NDEBUG
#define KP_LOG(level, func_name, line, fmt, err_code)
#else
inline static void __kp_log(const char *level, const char *func_name,
			    size_t line, const char *fmt, int err_code) {
    fprintf(stderr, "[%s] [%s] [line: %ld] %s. (%d)", level, func_name, line,
	    fmt, err_code);
}

#define KP_LOG(level, func_name, line, fmt, err_code) \
    __kp_log(level, func_name, line, fmt, err_code)
#define KP_INFO(func_name, line, fmt, err_code) \
    KP_LOG("INFO", func_name, line, fmt, err_code)
#define KP_WARN(func_name, line, fmt, err_code) \
    KP_LOG("WARN", func_name, line, fmt, err_code)
#define KP_ERROR(func_name, line, fmt, err_code) \
    KP_LOG("ERROR", func_name, line, fmt, err_code)
#endif
