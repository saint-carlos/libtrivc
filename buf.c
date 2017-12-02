#include "trivc/buf.h"

#include <zlib.h>
#include <string.h>

#include <trivc/trivc.h>
#include <trivc/numeric.h>
#include <trivc/string.h>

#ifdef __cplusplus
extern "C" {
#endif

static void tvc_buf_check_sane(tvc_buf_t buf)
{
	TVC_ASSERT((buf.p != NULL) || (buf.n == 0));
}

tvc_buf_t tvc_bufget(void *p, size_t n)
{
	tvc_buf_t buf = {
		.p = p,
		.n = n,
	};
	tvc_buf_check_sane(buf);
	return buf;
}

bool tvc_bufcpy(tvc_buf_t *dst, const tvc_buf_t *src)
{
	bool ret = dst->n >= src->n;

	tvc_buf_check_sane(*src);
	tvc_buf_check_sane(*dst);
	dst->n = tvc_umin(src->n, dst->n);
	memmove(dst->p, src->p, dst->n);
	return ret;
}

int tvc_bufcmp(tvc_buf_t a, tvc_buf_t b)
{
	int diff;

	tvc_buf_check_sane(a);
	tvc_buf_check_sane(b);

	diff = TVC_CMP(a.n, b.n);
	if (diff)
		return diff;
	/* if a.p or b.p are NULL, a.n == b.n == 0 */
	return memcmp(a.p, b.p, a.n);
}

bool tvc_bufeq(tvc_buf_t a, tvc_buf_t b)
{
	return tvc_bufcmp(a, b) == 0;
}

uint32_t tvc_bufcrc32(tvc_buf_t buf)
{
	uLong initial;

	tvc_buf_check_sane(buf);

	initial = adler32(0, (const Bytef *)&buf.n, sizeof(buf.n));
	if (!buf.p)
		return initial;
	return adler32(initial, (const Bytef *)buf.p, (uInt)buf.n);
}

size_t tvc_buffmt(char *p, size_t n, tvc_buf_t buf)
{
	tvc_buf_check_sane(buf);
	if (buf.p) {
		return tvc_scnprintf(p, n, "{n=%zu p=%p crc=0x%x}",
				buf.n, buf.p, tvc_bufcrc32(buf));
	} else {
		return tvc_scnprintf(p, n, "{n=%zu p=%p}", buf.n, buf.p);
	}
}

bool tvc_memeq(const void *a, const void *b, size_t len)
{
	return memcmp(a, b, len) == 0;
}
