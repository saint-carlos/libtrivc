#include <cstring>
#include <cassert>

#include <vector>

#define __TRIVC_UT
#include "trivc/trivc.h"

#include "trivc/string.h"
#include "trivc/numeric.h"

void assert_str0eq(const char* s, const char* expected)
{
	const char* s0 = tvc_str0(s);
	TVC_ASSERT(tvc_streq(s0, expected));
}

void test_str0()
{
	assert_str0eq(NULL, "");
	assert_str0eq("", "");
	assert_str0eq("test", "test");
	const char str_stack[] = "stack";
	assert_str0eq(str_stack, "stack");
}

char large1[4 * 1024 * 1024];
char large2[sizeof(large1)];
char alarge1[4 * 1024 * 1024];
char alarge2[sizeof(alarge1)];

static void init_large_str(char* s, size_t len, const char* contents)
{
	const size_t clen = strlen(contents);
	size_t cur = 0;

	TVC_ASSERT(len % clen == 0);

	while (cur < len) {
		memcpy(s + cur, contents, clen);
		cur += clen;
	}
	s[len - 1] = '\0';
}

static void init_allchar_str(char* all)
{
	for (unsigned c = 0; c < 256; c++)
		all[c] = c;
	all[0] = 128;
	all[256] = '\0';
}

void assert_streq(const char* s1, const char* s2, bool expected)
{
	TVC_ASSERT(tvc_streq(s1, s2) == expected);
}

void test_streq()
{
	const char empty1[] = "";
	const char empty2[] = "";
	const char abc2[] = "abc";
	const char def2[] = "def";
	const char defg2[] = "defg";
	const char efg2[] = "efg";
	char allchars1[257];
	char allchars2[257];

	init_large_str(large1, sizeof(large1), "0123456789abcdef");
	init_large_str(large2, sizeof(large2), "0123456789abcdef");
	init_allchar_str(allchars1);
	strcpy(allchars2, allchars1);
	init_large_str(alarge1, sizeof(alarge1), allchars1);
	init_large_str(alarge2, sizeof(alarge2), allchars2);

	/*
	 * we want different memory locations to make sure tvc_streq() isn't
	 * being sneaky.
	 */
	const char* strings1[] = {
		NULL,
		empty1,
		"abc",
		"def", // make sure it doesn't just rely on length
		"defg", // test prefix inequality
		"efg", // test suffix inequality
		large1,
		allchars1, // test all possible characters
		alarge1, // test difference of large strings
	};
	const char* strings2[] = {
		NULL,
		empty2,
		abc2,
		def2,
		defg2,
		efg2,
		large2,
		allchars2,
		alarge2,
	};
	static_assert(TVC_ARRAY_SIZE(strings1) == TVC_ARRAY_SIZE(strings2),
			"arrays must be of same size");

	for (size_t i = 0; i < TVC_ARRAY_SIZE(strings1); i++) {
		assert_streq(strings1[i], strings1[i], true);
		assert_streq(strings1[i], strings2[i], true);

		/* start from 0 rather than i to test symmetry */
		for (size_t j = 0; j < TVC_ARRAY_SIZE(strings2); j++) {
			bool equal = i == j;
			assert_streq(strings1[i], strings1[j], equal);
			assert_streq(strings1[i], strings2[j], equal);
		}
	}
}

void assert_strpbrk0_notfound(const char* s, const char* accept)
{
	size_t len = strlen(tvc_str0(s));
	const char* res = tvc_strpbrk0(s, accept);
	if (s)
		TVC_ASSERT(res == s + len);
	TVC_ASSERT(*res == '\0');
}

void assert_strpbrk0_found(const char* s, const char* accept,
		const char* expected)
{
	const char* res = tvc_strpbrk0(s, accept);
	TVC_ASSERT(res == expected);
}

void test_strpbrk()
{
	char allchars[257];
	init_allchar_str(allchars);

	std::vector<const char*> strings = {
		NULL,
		"",
		"a",
		"aaa",
		"abc",
		"bac",
		"aaabc",
		"baaac",
		"bcaaa",
		"xyz",
		"wxyz",
		"the quick brown fox jumped over the lazy dog",
		allchars,
	};

	for (const char* s: strings) {
		for (const char* accept: strings) {
			const char* res;
			bool should_have = true;
			if (!s || *s == '\0')
				should_have = false;
			if (!accept || *accept == '\0')
				should_have = false;
			if (should_have) {
				res = strpbrk(s, accept);
				if (!res)
					should_have = false;
			}

			if (should_have)
				assert_strpbrk0_found(s, accept, res);
			else
				assert_strpbrk0_notfound(s, accept);
		}
	}
}

void assert_str_foreach(const char* str, const char* delim)
{
	const char* current = str;
	const char* begin;
	const char* end;

	TVC_STR_FOREACH(str, delim, &begin, &end) {
		TVC_ASSERT(begin == current);
		TVC_ASSERT(tvc_strpbrk0(begin, delim) == end);
		current = end + 1;
	}
}

void test_str_foreach()
{
	const char* begin;
	const char* end;

	char allchars[257];
	init_allchar_str(allchars);

	init_large_str(large1, sizeof(large1), "0123456789abcdef");

	std::vector<const char*> strings = {
		NULL,
		"",
		"a",
		"aaa",
		"abc",
		"bac",
		"aaabc",
		"baaac",
		"bcaaa",
		"xyz",
		"wxyz",
		allchars,
		" ",
		",",
		", ",
		" ,",
		",,",
		"  ",
		" ,      ,,,, , , , ,,",
		"a ",
		"a,",
		" a",
		",a",
		"a,a,a",
		"qw rb asf 3w[pwrb adf/	r",
		"-092 t4mb2 -24 t9mnb",
		"a a a",
		"aa,aa aa,aa ,a,  ,   ,,,, ",
	};

	TVC_STR_FOREACH("test,test,test", ",", &begin, &end) {
		TVC_ASSERT(strncmp(begin, "test", end - begin) == 0);
	}

	for (const char* delims: strings) {
		TVC_STR_FOREACH(NULL, delims, &begin, &end) {
			TVC_ASSERT(false);
		}
		TVC_STR_FOREACH("", delims, &begin, &end) {
			TVC_ASSERT(false);
		}
		TVC_STR_FOREACH(delims, delims, &begin, &end) {
			TVC_ASSERT(begin == end);
		}
	}
	for (const char* s: strings) {
		for (const char* delims: strings) {
			assert_str_foreach(s, delims);
		}
	}
	assert_str_foreach(large1, NULL);
	assert_str_foreach(large1, "");
	assert_str_foreach(large1, allchars);
	assert_str_foreach(large1, " ,");
	assert_str_foreach(large1, "abc");
	assert_str_foreach(NULL, large1);
	assert_str_foreach("", large1);
	assert_str_foreach(allchars, large1);
	assert_str_foreach(", ", large1);
	assert_str_foreach("abc", large1);
}

size_t guarded_bufsz(size_t sz)
{
	return sizeof(unsigned) + sz + sizeof(unsigned);
}

void guarded_bufinit(char* buf, size_t sz)
{
	*(unsigned*)buf = 0xcafe;
	*(unsigned*)(buf + sizeof(unsigned) + sz) = 0xbabe;
}

void guarded_bufcheck(const char* buf, size_t sz)
{
	TVC_ASSERT(*(unsigned*)buf == 0xcafe);
	TVC_ASSERT(*(unsigned*)(buf + sizeof(unsigned) + sz) == 0xbabe);
}

char* guarded_bufextract(char* buf)
{
	return buf + sizeof(unsigned);
}

size_t guarded_bufszextract(size_t bufsz)
{
	return bufsz - 2 * sizeof(unsigned);
}

void __assert_formatted(const char* expect, size_t expect_len,
		char* real_guarded, size_t real_guarded_sz, size_t real_len)
{
	char* real = guarded_bufextract(real_guarded);
	size_t real_sz = guarded_bufszextract(real_guarded_sz);

	if (real_sz == 0) {
		TVC_ASSERT(real_len == 0);
	} else {
		size_t real_expect_len = tvc_umin(real_sz - 1, expect_len);
		TVC_ASSERT(real_len == real_expect_len);
		TVC_ASSERT(strncmp(real, expect, real_expect_len) == 0);
	}

	/* test buffer overflow / underflow */
	guarded_bufcheck(real_guarded, real_guarded_sz);
}

void __assert_printf(const char* expect, size_t expect_len,
		char* real_guarded, size_t real_guarded_sz, size_t real_len,
		const char* fmt, ...) __tvc_printf(6, 7);

void __assert_printf(const char* expect, size_t expect_len,
		char* real_guarded, size_t real_guarded_sz, size_t real_len,
		const char* fmt, ...)
{
	char* real = guarded_bufextract(real_guarded);
	size_t real_sz = guarded_bufszextract(real_guarded_sz);

	__assert_formatted(expect, expect_len, real_guarded,
			real_guarded_sz, real_len);

	va_list vl;
	va_start(vl, fmt);
	real_len = tvc_vscnprintf(real, real_sz, fmt, vl);
	va_end(vl);
	__assert_formatted(expect, expect_len, real_guarded,
			real_guarded_sz, real_len);
}

#define ASSERT_PRINTF(expect, sz, fmt, args...) do { \
			size_t formatted_len = snprintf(NULL, 0, fmt, ##args); \
			size_t real_sz = sz >= 0 ? sz : formatted_len + 1 - sz; \
			size_t real_guarded_sz = guarded_bufsz(real_sz); \
			char real_guarded[real_guarded_sz]; \
			guarded_bufinit(real_guarded, sizeof(real_guarded)); \
			char* real = guarded_bufextract(real_guarded); \
			size_t real_len = tvc_scnprintf(\
					real, real_sz, \
					fmt, ##args); \
			__assert_printf(expect, real_len, \
					real_guarded, real_guarded_sz, \
					real_len, fmt, ##args); \
		} while (false)

void test_printf()
{
	char allchars[257];
	init_allchar_str(allchars);

	for (int n: { 0, 1, 2, 5, 10, 100, -1, -2, -5 }) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-zero-length"
		ASSERT_PRINTF("", n, "");
#pragma GCC diagnostic pop
		ASSERT_PRINTF("abc", n, "abc");
		ASSERT_PRINTF("-1234", n, "%d", -1234);
		ASSERT_PRINTF("mystring", n, "%s", "mystring");
		ASSERT_PRINTF(allchars, n, "%s", allchars);
		ASSERT_PRINTF("mixed 0x3000 testing123", n, "mixed 0x%x %s",
				0x3000, "testing123");
	}
}

size_t mode2perms(char* buf, size_t len, mode_t mode)
{
	const char* src = NULL;
	switch (mode) {
	case 0644: src = "rw-r--r--"; break;
	case 0755: src = "rwxr-xr-x"; break;
	default: TVC_ASSERT(false); return 0;
	}
	size_t ret = tvc_umin(len, strlen(src));
	memcpy(buf, src, ret);
	return ret;
}

void test_stkstr()
{
	char buf[4096];
	sprintf(buf, "perms1='%s' perms2='%s'",
			TVC_STKSTRN(16, mode2perms, 0644),
			TVC_STKSTRN(16, mode2perms, 0755));
	TVC_ASSERT(tvc_streq(buf, "perms1='rw-r--r--' perms2='rwxr-xr-x'"));

	const char* haystack = "abc 30000 def";
	const char* str300 = strstr(haystack,
			TVC_STKSTRN(32, tvc_scnprintf, "%d", 300));
	TVC_ASSERT(str300 == haystack + 4);

	/* test truncation */
	sprintf(buf, "perms1='%s' perms2='%s'",
			TVC_STKSTRN(3, mode2perms, 0644),
			TVC_STKSTRN(6, mode2perms, 0755));
	TVC_ASSERT(tvc_streq(buf, "perms1='rw-' perms2='rwxr-x'"));

	init_large_str(large1, sizeof(large1), "0123456789abcdef");
	sprintf(buf, "%s", TVC_STKSTR(tvc_scnprintf, "%s", large1));
	TVC_ASSERT(strlen(buf) == 32);
	TVC_ASSERT(strncmp(buf, large1, 32) == 0);
	sprintf(buf, "%s", TVC_STKSTRL(tvc_scnprintf, "%s", large1));
	TVC_ASSERT(strlen(buf) == 512);
	TVC_ASSERT(strncmp(buf, large1, 512) == 0);
}

int main(int argc, char** argv)
{
	test_str0();
	test_streq();
	test_strpbrk();
	test_str_foreach();
	test_printf();
	test_stkstr();
}
