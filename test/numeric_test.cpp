#include <vector>

#define __TRIVC_UT
#include "trivc/trivc.h"

#include "trivc/numeric.h"

void test_sign()
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

	for (int64_t n: svals) {
		int sign = tvc_num_sign(n);
		if (n < 0)
			TVC_ASSERT(sign == -1);
		else if (n > 0)
			TVC_ASSERT(sign == +1);
		else
			TVC_ASSERT(sign == 0);
	}
}

template <typename scalar_t,
	scalar_t minfunc(scalar_t, scalar_t),
	scalar_t maxfunc(scalar_t, scalar_t)>
void __test_min_max(const std::vector<scalar_t>& vals)
{
	for (scalar_t a: vals) {
		for (scalar_t b: vals) {
			if (a < b) {
				TVC_ASSERT(minfunc(a, b) == a);
				TVC_ASSERT(maxfunc(a, b) == b);
			} else {
				TVC_ASSERT(minfunc(a, b) == b);
				TVC_ASSERT(maxfunc(a, b) == a);
			}
		}
	}
}

void test_min_max()
{
	const int64_t G = 1024 * 1024 * 1024;

	std::vector<intmax_t> svals = {
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
	__test_min_max<intmax_t, tvc_min, tvc_max>(svals);

	std::vector<uintmax_t> uvals = {
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
	__test_min_max<uintmax_t, tvc_umin, tvc_umax>(uvals);
}

int main(int argc, char** argv)
{
	test_sign();
	test_min_max();
}
