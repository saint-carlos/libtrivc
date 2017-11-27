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

/**
 * tvc_strpbrk0() - search a string for any of a set of bytes, return search end
 *
 * like strpbrk(3), tvc_strpbrk0() locates the first occurrence in the string @s
 * of any of the bytes in the string @accept.
 *
 * the difference is that if nothing was found, tvc_strpbrk0() returns a pointer
 * to the end of the string, i.e. to the null terminator.
 *
 * NULL strings are implicitly converted to empty strings.
 */
const char *tvc_strpbrk0(const char *s, const char *accept);

void __tvc_str_foreach_init(const char **beginp, const char**endp,
		const char *s, const char *delims);
bool __tvc_str_foreach_valid(const char **beginp, const char**endp);
void __tvc_str_foreach_advance(const char **beginp, const char **endp,
		const char *delims);

/**
 * TVC_STR_FOREACH() - building block for string tokenizing
 * @str:	string to tokenize.
 * @delims:	C string functioning as array of characters by which to tokenize.
 * @beginp:	will point to the first character of each token.
 * @endp:	will point to the first character after the end of each token.
 *
 * TVC_STR_FOREACH() tokenizes a string based on delimiters, without modifying
 * the string or allocating memory.
 * instead, it just sets pointers to the start and end (@beginp and @endp) of
 * the current token, and lets the caller decide what to do with it.
 *
 * the token length will be *@endp - *@beginp.
 *
 * if two or more characters in @delims appear consecutively, empty strings will
 * be seen, i.e. @endp == @beginp.
 */
#define TVC_STR_FOREACH(str, delims, beginp, endp) \
	for (__tvc_str_foreach_init(beginp, endp, tvc_str0(str), delims); \
			__tvc_str_foreach_valid(beginp, endp); \
			__tvc_str_foreach_advance(beginp, endp, delims))

#ifdef __cplusplus
}
#endif
