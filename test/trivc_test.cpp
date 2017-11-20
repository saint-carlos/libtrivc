#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <sys/types.h>

#include <vector>

#include "trivc/string.h"

static char trcbuf[512];
#define TVC_TPRINTF(fmt, args...) snprintf(trcbuf, sizeof(trcbuf), fmt, ##args)

#define __TRIVC_UT
#include "trivc/trivc.h"

void test_gettid()
{
	pid_t pid = getpid();
	pid_t tid = tvc_gettid();

	TVC_ASSERT(tid == pid);
}

static void assert_str_contains(const char* haystack, const char* needle)
{
	TVC_ASSERT(strstr(haystack, needle));
}

void test_trace()
{
	TVC_TRACE("fmtstr %lu '%s'", 12UL, "arg");

	// this is about as much as we can reasonably test
	assert_str_contains(trcbuf, "fmtstr 12 'arg'");
	assert_str_contains(trcbuf, __TVC_FILEBASE__);
	assert_str_contains(trcbuf, __func__);
}

template <typename scalar_t>
void __test_cmp(const std::vector<scalar_t>& vals)
{
	for (scalar_t a: vals) {
		for (scalar_t b: vals) {
			int res;
			if (a < b)
				res = -1;
			else if (a > b)
				res = +1;
			else
				res = 0;
			TVC_ASSERT(TVC_CMP(a, b) == res);
		}
	}
}

void test_cmp()
{
	const int64_t G = 1024 * 1024 * 1024;

	std::vector<int64_t> svals = {
		INT64_MIN,
		INT64_MIN + 1,
		-4 * G - 1,
		-4 * G,
		-4 * G + 1,
		-1024,
		-100,
		-2,
		-1,
		0,
		+1,
		+2,
		+100,
		+1024,
		+4 * G - 1,
		+4 * G,
		+4 * G + 1,
		INT64_MAX - 1,
		INT64_MAX,
	};
	__test_cmp(svals);

	std::vector<uint64_t> uvals = {
		0,
		1,
		2,
		100,
		1024,
		4 * G - 1,
		4 * G,
		4 * G + 1,
		UINT64_MAX - 1,
		UINT64_MAX,
	};
	__test_cmp(uvals);
}

int main(int argc, char** argv)
{
	test_gettid();
	test_trace();
	test_cmp();
}
