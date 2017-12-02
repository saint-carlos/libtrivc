#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <trivc/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * struct tvc_buf - everything you've ever dreamed of
 * @p:	bytes of the buffer.
 * @n:	number of bytes in @p which are valid to access.
 * @s:	buffer as string type, for convenience only.
 *
 * the invariant @p != NULL || @n == 0 must always hold.
 *
 * the whole point of this struct is to save people from spelling out two
 * arguments, so do yourself a favor and pass this by value unless you
 * absolutely can't.
 *
 * @s exists as a matter of convenience. it can be used for interfaces which
 * need to fill in a string. none of the tvc_buf interfaces assigns any meaning
 * to @s, and no handling of null termination is done.
 */
typedef struct tvc_buf {
	union {
		void *p;
		char *s;
	};
	size_t n;
} tvc_buf_t;

/**
 * tvc_bufget() - return an on-stack buffer structure
 */
tvc_buf_t tvc_bufget(void *p, size_t n);

/**
 * tvc_bufcpy() - deep-copy a buffer
 *
 * copies at most @dst->n bytes from @src. the previous contents of @dst->p
 * is assumed to be meaningless.
 *
 * returns 'true' if all of @src could be copied into @dst and 'false'
 * otherwise.
 *
 * if 'true' is returned, tvc_bufeq(*@dst, *@src) == true.
 */
bool tvc_bufcpy(tvc_buf_t *dst, const tvc_buf_t *src);

/**
 * tvc_bufcmp() - compare buffers, similar to memcmp(3)
 *
 * if @a and @b are of different lengths, a negative number is returned if @a is
 * shorter than @b, and a positive number is returned if @a is longer than @b.
 *
 * if @a and @b are of the same length, the return value is the same as
 * memcmp(3).
 */
int tvc_bufcmp(tvc_buf_t a, tvc_buf_t b);

/**
 * tvc_bufeq() - compare 2 buffers
 *
 * returns true if @a has the same content as @b, or false otherwise.
 * this is equivalent to "tvc_bufcmp(a, b) == 0".
 */
bool tvc_bufeq(tvc_buf_t a, tvc_buf_t b);

/**
 * tvc_bufcrc32() - crc32 of a buffer.
 *
 * @buf.n counts as input for crc32.
 *
 * this uses the adler32 crc from zlib.
 */
uint32_t tvc_bufcrc32(tvc_buf_t buf);

/**
 * tvc_buffmt() - format a buffer
 *
 * formatting logic and guarantees are same as for tvc_scnprintf().
 */
size_t tvc_buffmt(char *p, size_t n, tvc_buf_t buf);

/**
 * TVC_BUFSTR() - syntactic sugar for buffer printing
 *
 * used similarly to TVC_STKSTR().
 */
#define TVC_BUFSTR(buf) TVC_STKSTRN(48, tvc_buffmt, buf)

/**
 * tvc_memeq() - compare 2 pieces of memory
 *
 * returns true if @a has the same content as @b, or false otherwise.
 * this is equivalent to "memcmp(a, b) == 0".
 */
bool tvc_memeq(const void *a, const void *b, size_t len);

#ifdef __cplusplus
}
#endif
