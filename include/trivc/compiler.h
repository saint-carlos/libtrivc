#pragma once

#define __tvc_printf(fmt, var)	__attribute__((format(printf, fmt, var)))
#define __tvc_packed		__attribute__((packed))
#define __tvc_unused		__attribute__((unused))
#define __tvc_noreturn		__attribute__((noreturn))
#define __tvc_deprecated	__attribute__((deprecated))

#include <assert.h>
#define __TVC_IS_ARRAY(a) ((void *)(a) == (void *)&(a))

/**
 * TVC_ARRAY_SIZE() - compute size of static array
 *
 * @a must be an array type (as opposed to pointer).
 * @a must not be dynamically sized.
 */
#define TVC_ARRAY_SIZE(a) ({ \
		static_assert(__TVC_IS_ARRAY(a), #a "is not an array"); \
		static_assert(sizeof(a) > 0, #a "is a variable length array"); \
		(sizeof(a) / sizeof((a)[0])); \
	})

#include <stddef.h>

/**
 * tvc_container_of() - obtain the containing struct of a pointer
 * @ptr:	pointer to a struct member.
 * @container:	type of the requested struct.
 * @member:	name of the field in @container which has the address @ptr.
 *
 * for example:
 * struct mystruct {
 * 	int a;
 * 	int b;
 * };
 * struct mystruct m;
 * int *bp = &m->b;
 * struct mystruct mp = tvc_container_of(bp, struct mystruct, b);
 * assert(mp == &m);
 */
#define tvc_container_of(ptr, container, member) \
	((container *)((char *)(ptr) - offsetof(container, member)))

/**
 * TVC_STR() - stringify token
 *
 * returns the token, as is, in string form.
 * examples:
 * #define AA aa
 * TVC_STR(aa) returns "aa"
 * TVC_STR(AA) returns "AA"
 */
#define TVC_STR(__x) #__x

/**
 * TVC_PSTR() - stringify expanded token
 *
 * the given token undergoes macro expansion, and only then stringified using
 * TVC_STR().
 *
 * examples:
 * #define AA aa
 * TVC_STR(aa) returns "aa"
 * TVC_STR(AA) returns "aa"
 */
#define TVC_PSTR(__x) TVC_STR(__x)

static const char *__tvc_file = NULL;

static const char *__tvc_set_filebase(const char *__file__) __tvc_unused;

#include <string.h>
static const char *__tvc_set_filebase(const char *__file__)
{
	const char *tvc_file_local;
	const char *slash;

	slash = strrchr(__file__, '/');
	if (slash)
		tvc_file_local = slash + 1;
	else
		tvc_file_local = __file__;
	__atomic_store_n(&__tvc_file, tvc_file_local, __ATOMIC_SEQ_CST);
	return tvc_file_local;
}

/**
 * __TVC_FILEBASE__ - returns basename of file
 *
 * this is useful for building a poor man's logger.
 */
#define __TVC_FILEBASE__ (__tvc_file ? __tvc_file : __tvc_set_filebase(__FILE__))
