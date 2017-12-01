#pragma once

#include <stdarg.h>
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

/**
 * tvc_scnprintf() - format unimportant strings
 * @buf:	buffer to write to.
 * @len:	number of bytes allocated in @buf.
 * @fmt:	format string.
 *
 * tvc_scnprintf() is like snprintf(), except the returned number of bytes is
 * the number of bytes actually formatted.
 * this number does NOT include the terminating null.
 * there is no way to determine whether or not the output was truncated.
 * this is useful when formatting something for humans e.g. logging.
 *
 * tvc_scnprintf() exists because snprintf() has an inconvenient interface:
 * if the buffer size is n and the formatted result is n + m bytes, snprintf
 * will return n + m. but callers should not have to bother to check that
 * _every time_, and so they just do:
 * 	res = 0;
 * 	res += snprintf(p + res, n - res, ...);
 * 	res += snprintf(p + res, n - res, ...);
 * 	...
 * which could cause "n - sres" to go negative at some point.
 * that negative, when passed back to the next snprintf, turns into a huge
 * positive, which causes memory stomp.
 * this is quite silly, since printf returns int (typeically 32 bit), so it
 * can't actually write a full size_t (typically 64 bit).
 */
size_t tvc_scnprintf(char *buf, size_t len, const char *fmt, ...)
		__tvc_printf(3, 4);

/**
 * tvc_vscnprintf() - va_list version of tvc_scnprintf()
 */
size_t tvc_vscnprintf(char *buf, size_t len, const char *fmt, va_list vl)
		__tvc_printf(3, 0);

#ifdef __cplusplus
}
#endif
