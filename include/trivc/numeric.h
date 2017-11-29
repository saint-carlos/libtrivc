#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * tvc_num_sign() - returns the sign of a number: +1, -1, or 0.
 */
static inline int tvc_num_sign(intmax_t num)
{
	return (num > 0) - (num < 0);
}

/* min and max function implemented as non-macros for single evaluation */

static inline intmax_t tvc_max(intmax_t a, intmax_t b)
{
	return (a > b) ? a : b;
}

static inline uintmax_t tvc_umax(uintmax_t a, uintmax_t b)
{
	return (a > b) ? a : b;
}

static inline intmax_t tvc_min(intmax_t a, intmax_t b)
{
	return (a < b) ? a : b;
}

static inline uintmax_t tvc_umin(uintmax_t a, uintmax_t b)
{
	return (a < b) ? a : b;
}

#ifdef __cplusplus
}
#endif
