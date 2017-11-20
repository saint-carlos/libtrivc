#pragma once

#include <stdbool.h>

#include <trivc/compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * tvc_str0() - return a non-null string
 *
 * if @s is NULL, the empty string is returned. otherwise, @s is returned.
 */
const char *tvc_str0(const char *s);

/**
 * tvc_streq() - compare 2 strings
 *
 * if @a == NULL and @b == NULL, true is returned.
 * if @a has the same content as @b, true is returned.
 * otherwise, false is returned.
 */
bool tvc_streq(const char *a, const char *b);

#ifdef __cplusplus
}
#endif
