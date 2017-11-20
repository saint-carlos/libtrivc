#pragma once

#include <trivc/compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NDEBUG) && !defined(__TRIVC_UT)
#define TVC_NDEBUG
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

/**
 * tvc_gettid() - returns the thread ID of the calling thread
 */
static inline pid_t tvc_gettid(void)
{
	return syscall(SYS_gettid);
}

/**
 * TVC_TRACE() - emit a trace with metadata
 *
 * metadata e.g. timestamp, thread ID etc.
 * by default, trace is emitted to stderr. to override the default, define
 * TVC_TPRINTF to something that has the same signature as printf(3).
 */
#ifdef TVC_NDEBUG
#define TVC_TRACE(fmt, args...) do { } while (0)
#else
#ifndef TVC_TPRINTF
#include <stdio.h>
#define TVC_TPRINTF(fmt, args...) fprintf(stderr, fmt, ##args)
#endif
#include <time.h>
#define TVC_TRACE(fmt, args...) do { \
		/* TODO optimize time handling */ \
		struct timespec __tvc_ts; \
		char __buf[32]; \
		clock_gettime(CLOCK_REALTIME, &__tvc_ts); \
		strftime(__buf, sizeof(__buf), "%Y-%m-%d %H:%M:%S", \
				gmtime(&__tvc_ts.tv_sec)); \
		TVC_TPRINTF("%s.%09ld %d: %s:%d: %s(): " fmt "\n", \
			__buf, __tvc_ts.tv_nsec, tvc_gettid(), \
			__TVC_FILEBASE__, __LINE__, __func__, ##args); \
	} while (0)
#endif

/**
 * TVC_ASSERT() - just a regular assert
 *
 * mainly for use within libtrvic itself.
 */
#ifdef TVC_NDEBUG
#define TVC_ASSERT(cond) do { (void)(cond); } while (0)
#else
#include <assert.h>
#define TVC_ASSERT(cond) do { assert(cond); } while (0)
#endif

/**
 * TVC_CMP() - building block for safe *cmp() functions
 *
 * TVC_CMP() helps building functions similar to memcmp(3) and strcmp(3).
 * it has generic *cmp() semantics that returns as follows:
 * -1 if @a < @b
 * 0 if @a == @b
 * +1 if @a > @b
 *
 * safety dictates that simple subtraction cannot be used for the following
 * reasons:
 * - subtraction may underflow, which will make it return positive instead of
 *   negative.
 * - most *cmp() functions return an int. if @a and @b are both of long types,
 *   and @a == 4G and @b == 0, then @a - @b = 4G, which is 0 when converted to
 *   int.
 * - subtraction preserves @a's type, which may be unsigned, however the result
 *   needs to be signed, or gcc will be unhappy.
 */
#define TVC_CMP(a, b) ((a) < (b) ? -1 : (a) > (b) ? +1 : 0)

#ifdef __cplusplus
}
#endif
