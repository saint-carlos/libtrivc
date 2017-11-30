#include <cstring>
#include <regex>

#include <bsd/string.h>

#define __TRIVC_UT
#include "trivc/trivc.h"
#include "trivc/compiler.h"
#include "trivc/numeric.h"

#include "trivc/buf.h"

template <typename T>
static void assert_buf(const tvc_buf_t buf, const T* p, size_t n)
{
	TVC_ASSERT(buf.p == (void*)p);
	TVC_ASSERT(buf.n == n);
}

static void test_bufget_one(void* p, size_t n)
{
	tvc_buf_t buf = tvc_bufget(p, n);
	assert_buf(buf, p, n);
}

static void test_bufget()
{
	int array[100];

	void* p = (void*)array;
	size_t nlist[] = { 0, 12, sizeof(array) };
	for (size_t n: nlist) {
		test_bufget_one(p, n);
	}

	size_t half = TVC_ARRAY_SIZE(array) / 2;
	void* middle = (void*)(array + half);
	size_t nlist_half[] = { 0, 12, sizeof(array) / 2 };
	for (size_t n: nlist_half) {
		test_bufget_one(middle, n);
	}

	test_bufget_one(NULL, 0);
}

template <typename T>
static void fill_arr(T* array, size_t size, T value)
{
	for (size_t i = 0; i < size; i++)
		array[i] = value;
}

#define FILL_ARR(array, value) fill_arr(array, TVC_ARRAY_SIZE(array), value)

template <typename T>
static tvc_buf_t bufget_xx(T* p, size_t n)
{
	return tvc_bufget((void*)p, n);
}

#define BUFGET_ARR(array) bufget_xx(array, sizeof(array))

template <typename T>
static void prep_arr_buf(tvc_buf_t* buf, T* array, size_t n, T value)
{
	fill_arr(array, n / sizeof(T), value);
	*buf = tvc_bufget((void*)array, n);
}

#define PREP_ARR_BUF(bufp, array, value) \
		prep_arr_buf(bufp, array, sizeof(array), value)

void assert_bufcmp(tvc_buf_t a, tvc_buf_t b, int expected_sign)
{
	int sign = tvc_bufcmp(a, b);
	TVC_ASSERT(tvc_num_sign(sign) == tvc_num_sign(expected_sign));

	bool expect_equal = expected_sign == 0;
	bool equal = tvc_bufeq(a, b);
	TVC_ASSERT(equal == expect_equal);
}

static void test_bufcmp_bufeq()
{
	char array_a[100];
	FILL_ARR(array_a, '1');
	tvc_buf_t buf_a = BUFGET_ARR(array_a);

	char array_b[50];
	FILL_ARR(array_b, '2');
	tvc_buf_t buf_b = BUFGET_ARR(array_b);

	char array_c[50];
	FILL_ARR(array_c, '2');
	tvc_buf_t buf_c = BUFGET_ARR(array_c);

	// trivial
	assert_bufcmp(buf_a, buf_a, 0);
	assert_bufcmp(buf_b, buf_b, 0);
	// same buf, different ptrs
	assert_bufcmp(buf_b, buf_c, 0);

	// length based check first
	assert_bufcmp(buf_a, buf_b, +1);
	assert_bufcmp(buf_b, buf_a, -1);
	PREP_ARR_BUF(&buf_a, array_a, '2');
	PREP_ARR_BUF(&buf_b, array_b, '1');
	assert_bufcmp(buf_a, buf_b, +1);
	assert_bufcmp(buf_b, buf_a, -1);

	// content based check if lengths are equal
	PREP_ARR_BUF(&buf_b, array_b, '2');
	PREP_ARR_BUF(&buf_c, array_c, '3');
	assert_bufcmp(buf_b, buf_c, -1);
	assert_bufcmp(buf_c, buf_b, +1);

	// test 0 length
	PREP_ARR_BUF(&buf_b, array_b, '2');
	buf_c = tvc_bufget(array_c, 0);
	assert_bufcmp(buf_b, buf_c, +1);
	assert_bufcmp(buf_c, buf_b, -1);
	assert_bufcmp(buf_c, buf_c, 0);

	// test NULL
	PREP_ARR_BUF(&buf_b, array_b, '2');
	buf_c = tvc_bufget(NULL, 0);
	assert_bufcmp(buf_b, buf_c, +1);
	assert_bufcmp(buf_c, buf_b, -1);
	assert_bufcmp(buf_c, buf_c, 0);
}

static void assert_bufcpy(tvc_buf_t* dst, tvc_buf_t* src, bool expected_ret)
{
	tvc_buf_t src_orig = *src;
	tvc_buf_t dst_orig = *dst;
	uint8_t src_array[src->n];
	bool ret;

	memcpy(src_array, src->p, src->n);

	ret = tvc_bufcpy(dst, src);
	TVC_ASSERT(ret == expected_ret);

	TVC_ASSERT(src->p == src_orig.p);
	TVC_ASSERT(src->n == src_orig.n);
	TVC_ASSERT(memcmp(src->p, src_array, src->n) == 0);

	TVC_ASSERT(dst->p == dst_orig.p);
	if (ret == true) {
		TVC_ASSERT(tvc_bufeq(*dst, *src));
	} else {
		TVC_ASSERT(dst->n == dst_orig.n);
		TVC_ASSERT(memcmp(dst->p, src->p, dst->n) == 0);
	}
}

static void test_bufcpy()
{
#define SIZE 100
	int src_array[SIZE];
	int dst_array[SIZE];

	tvc_buf_t srcbuf;
	tvc_buf_t dstbuf;

	// test full copy
	PREP_ARR_BUF(&srcbuf, src_array, 0x10);
	PREP_ARR_BUF(&dstbuf, dst_array, 0x20);
	assert_bufcpy(&dstbuf, &srcbuf, true);

	// test different sizes
	prep_arr_buf(&srcbuf, src_array, SIZE, 0x10);
	prep_arr_buf(&dstbuf, dst_array, SIZE / 2, 0x20);
	assert_bufcpy(&dstbuf, &srcbuf, false);
	prep_arr_buf(&srcbuf, src_array, SIZE / 2, 0x10);
	prep_arr_buf(&dstbuf, dst_array, SIZE, 0x20);
	assert_bufcpy(&dstbuf, &srcbuf, true);

	// test buffers of size 0
	prep_arr_buf(&srcbuf, src_array, SIZE, 0x10);
	dstbuf = tvc_bufget(dst_array, 0);
	assert_bufcpy(&dstbuf, &srcbuf, false);
	srcbuf = tvc_bufget(dst_array, 0);
	prep_arr_buf(&dstbuf, dst_array, SIZE, 0x20);
	assert_bufcpy(&dstbuf, &srcbuf, true);
	srcbuf = tvc_bufget(dst_array, 0);
	dstbuf = tvc_bufget(dst_array, 0);
	assert_bufcpy(&dstbuf, &srcbuf, true);

	// test same pointer
	prep_arr_buf(&srcbuf, dst_array, SIZE, 0x20);
	prep_arr_buf(&dstbuf, dst_array, SIZE, 0x20);
	assert_bufcpy(&dstbuf, &srcbuf, true);
	prep_arr_buf(&srcbuf, dst_array, SIZE, 0x20);
	dstbuf = tvc_bufget(dst_array, 0);
	assert_bufcpy(&dstbuf, &srcbuf, false);
	srcbuf = tvc_bufget(dst_array, 0);
	prep_arr_buf(&dstbuf, dst_array, SIZE, 0x20);
	assert_bufcpy(&dstbuf, &srcbuf, true);
	srcbuf = tvc_bufget(dst_array, 0);
	dstbuf = tvc_bufget(dst_array, 0);
	assert_bufcpy(&dstbuf, &srcbuf, true);

	// test NULL <-> 0
	srcbuf = tvc_bufget(src_array, 0);
	dstbuf = tvc_bufget(NULL, 0);
	assert_bufcpy(&dstbuf, &srcbuf, true);
	srcbuf = tvc_bufget(NULL, 0);
	dstbuf = tvc_bufget(dst_array, 0);
	assert_bufcpy(&dstbuf, &srcbuf, true);
	srcbuf = tvc_bufget(NULL, 0);
	dstbuf = tvc_bufget(NULL, 0);
	assert_bufcpy(&dstbuf, &srcbuf, true);

	// test NULL <-> nonzero
	PREP_ARR_BUF(&srcbuf, src_array, 0x10);
	dstbuf = tvc_bufget(NULL, 0);
	assert_bufcpy(&dstbuf, &srcbuf, false);
	srcbuf = tvc_bufget(NULL, 0);
	PREP_ARR_BUF(&dstbuf, dst_array, 0x20);
	assert_bufcpy(&dstbuf, &srcbuf, true);
#undef SIZE
}

static void run_bufcrc32(char* p, size_t n, uint32_t expected)
{
	tvc_buf_t b = tvc_bufget((void*)p, n);
	uint32_t crc = tvc_bufcrc32(b);
	TVC_TRACE("crc 0x%x", crc);
	TVC_ASSERT(crc == expected);
}

static void test_bufcrc()
{
	// null ptr
	run_bufcrc32(NULL, 0, 0x0);

	// all zeroes in buf
	char array_0[100];
	FILL_ARR(array_0, '\0');
	size_t size_0 = sizeof(array_0);
	run_bufcrc32(array_0, 0, 0);
	run_bufcrc32(array_0, 1, 0x90001);
	run_bufcrc32(array_0, size_0 / 2, 0xb540032);
	run_bufcrc32(array_0, size_0, 0x2a300064);

	// non-zeroes in buf
	char array_n[100];
	for (size_t i = 0; i < TVC_ARRAY_SIZE(array_n); i++) {
		array_n[i] = i * 2 + 7;
	}
	size_t size_n = sizeof(array_0);
	run_bufcrc32(array_n, 0, 0);
	run_bufcrc32(array_n, 1, 0x100008);
	run_bufcrc32(array_n, size_n / 2, 0xd0e30b22);
	run_bufcrc32(array_n, size_n, 0xca8529cc);
}

#define BUF_TEST_STR "test"

static void test_string_one(size_t sz)
{
	char a[sizeof(BUF_TEST_STR) + 1];
	static_assert(TVC_ARRAY_SIZE(a) == 6, "invalid str buf size");
	memset(a, 0xaf, sizeof(a));
	tvc_buf_t sbuf = tvc_bufget(sz ? a : NULL, sz);

	if (sbuf.s) {
		strlcpy(sbuf.s, BUF_TEST_STR, sbuf.n);
		TVC_ASSERT(strncmp(sbuf.s, BUF_TEST_STR, sbuf.n - 1) == 0);
	}
}

static void test_string()
{
	test_string_one(sizeof(BUF_TEST_STR) + 2);
	test_string_one(sizeof(BUF_TEST_STR) + 1);
	test_string_one(sizeof(BUF_TEST_STR));
	test_string_one(sizeof(BUF_TEST_STR) - 1);
	test_string_one(sizeof(BUF_TEST_STR) / 2);
	test_string_one(0);
}

static void test_memeq()
{
	char a[100];
	FILL_ARR(a, '1');
	char b[50];
	FILL_ARR(b, '2');
	char c[50];
	FILL_ARR(c, '2');
	char d[100];
	fill_arr(d, 50, '1');
	fill_arr(d + 50, 50, '2');

	TVC_ASSERT(tvc_memeq(NULL, NULL, 0));
	TVC_ASSERT(tvc_memeq(a, NULL, 0));
	TVC_ASSERT(tvc_memeq(NULL, a, 0));
	TVC_ASSERT(tvc_memeq(a, a, 0));
	TVC_ASSERT(tvc_memeq(a, b, 0));
	TVC_ASSERT(tvc_memeq(b, b, 0));

	TVC_ASSERT(tvc_memeq(a, a, sizeof(a)));
	TVC_ASSERT(tvc_memeq(b, b, sizeof(b)));
	TVC_ASSERT(tvc_memeq(c, c, sizeof(c)));
	TVC_ASSERT(tvc_memeq(d, d, sizeof(d)));

	TVC_ASSERT(!tvc_memeq(a, b, sizeof(b)));
	TVC_ASSERT(!tvc_memeq(a, c, sizeof(c)));
	TVC_ASSERT(tvc_memeq(b, c, sizeof(b)));
	TVC_ASSERT(tvc_memeq(a, d, 50));
	TVC_ASSERT(!tvc_memeq(a, d, sizeof(a)));
}

int main(int argc, char** argv)
{
	test_bufget();
	test_bufcmp_bufeq();
	test_bufcpy();
	test_bufcrc();
	test_string();
	test_memeq();
}
