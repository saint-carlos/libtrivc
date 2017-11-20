#include <cstring>
#include <cassert>

#include <vector>

#define __TRIVC_UT
#include "trivc/trivc.h"

#include "trivc/string.h"

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

int main(int argc, char** argv)
{
	test_str0();
	test_streq();
}
