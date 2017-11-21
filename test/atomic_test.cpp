#include <cstdint>
#include <cstring>
#include <vector>

#define __TRIVC_UT
#include "trivc/trivc.h"

#include "trivc/atomic.h"

void test_memory_barrier()
{
	tvc_memory_barrier();
}

void assert_atomic_get(const tvc_atomic_t* a, int64_t expected_val)
{
	int64_t val = tvc_atomic_get(a);
	TVC_ASSERT(val == expected_val);
}

void assert_atomic_uget(const tvc_atomic_t* a, uint64_t expected_val)
{
	uint64_t val = tvc_atomic_uget(a);
	TVC_ASSERT(val == expected_val);
}

void assert_atomic_set(tvc_atomic_t* a, int64_t val)
{
	int64_t ret = tvc_atomic_set(a, val);
	TVC_ASSERT(ret == val);
	assert_atomic_get(a, val);
}

void assert_atomic_uset(tvc_atomic_t* a, uint64_t val)
{
	uint64_t ret = tvc_atomic_uset(a, val);
	TVC_ASSERT(ret == val);
	assert_atomic_uget(a, val);
}

void test_atomic_init()
{
	tvc_atomic_t little_boy;
	tvc_atomic_init(&little_boy, -9);
	assert_atomic_get(&little_boy, -9);

	tvc_atomic_t davy_crockett;
	tvc_atomic_init(&davy_crockett, 5203049);
	assert_atomic_get(&davy_crockett, 5203049);

	tvc_atomic_t tatyana;
	tvc_atomic_uinit(&tatyana, 30);
	assert_atomic_uget(&tatyana, 30);

	tvc_atomic_t fat_man = TVC_ATOMIC_INIT(-67324);
	assert_atomic_get(&fat_man, -67324);

	tvc_atomic_t tsar_bomba = TVC_ATOMIC_INIT(245084643);
	assert_atomic_get(&tsar_bomba, 245084643);
}

void test_atomic_get_set()
{
	tvc_atomic_t little_boy = TVC_ATOMIC_INIT(0);
	assert_atomic_get(&little_boy, 0);
	assert_atomic_uget(&little_boy, 0);

	assert_atomic_set(&little_boy, 100);
	assert_atomic_uget(&little_boy, 100);
	assert_atomic_uset(&little_boy, 100);
	assert_atomic_get(&little_boy, 100);

	assert_atomic_set(&little_boy, -100);

	assert_atomic_uset(&little_boy, UINT64_MAX);

	assert_atomic_uset(&little_boy, INT64_MAX);
	assert_atomic_get(&little_boy, INT64_MAX);
	assert_atomic_set(&little_boy, INT64_MAX);
	assert_atomic_uget(&little_boy, INT64_MAX);

	assert_atomic_set(&little_boy, INT64_MIN);
}

void assert_atomic_add(tvc_atomic_t* a, int64_t v)
{
	int64_t current = tvc_atomic_get(a);
	int64_t expected = current + v;
	int64_t ret = tvc_atomic_add(a, v);
	TVC_ASSERT(ret == expected);
	assert_atomic_get(a, expected);
}

void assert_atomic_sub(tvc_atomic_t* a, int64_t v)
{
	int64_t current = tvc_atomic_get(a);
	int64_t expected = current - v;
	int64_t ret = tvc_atomic_sub(a, v);
	TVC_ASSERT(ret == expected);
	assert_atomic_get(a, expected);
}

void assert_atomic_inc(tvc_atomic_t* a)
{
	int64_t current = tvc_atomic_get(a);
	int64_t expected = current + 1;
	int64_t ret = tvc_atomic_inc(a);
	TVC_ASSERT(ret == expected);
	assert_atomic_get(a, expected);
}

void assert_atomic_dec(tvc_atomic_t* a)
{
	int64_t current = tvc_atomic_get(a);
	int64_t expected = current - 1;
	int64_t ret = tvc_atomic_dec(a);
	TVC_ASSERT(ret == expected);
	assert_atomic_get(a, expected);
}

void test_atomic_arithmetic()
{
	std::vector<int64_t> vals = { 0, INT64_MIN, INT64_MAX, -20, +30 };
	tvc_atomic_t fat_man;

	for (int64_t initial: vals) {
		for (int64_t delta: vals) {
			if (initial == INT64_MIN && delta < 0)
				continue;
			if (initial == INT64_MAX && delta > 0)
				continue;
			if (initial < 0 && delta == INT64_MIN)
				continue;
			if (initial > 0 && delta == INT64_MAX)
				continue;
			tvc_atomic_init(&fat_man, initial);
			assert_atomic_add(&fat_man, delta);
		}

		for (int64_t delta: vals) {
			if (initial == INT64_MIN && delta > 0)
				continue;
			if (initial == INT64_MAX && delta < 0)
				continue;
			if (initial > 0 && delta == INT64_MIN)
				continue;
			if (initial < 0 && delta == INT64_MAX)
				continue;
			tvc_atomic_init(&fat_man, initial);
			assert_atomic_sub(&fat_man, delta);
		}

		if (initial != INT64_MAX) {
			tvc_atomic_init(&fat_man, initial);
			assert_atomic_inc(&fat_man);
		}
		if (initial != INT64_MIN) {
			tvc_atomic_init(&fat_man, initial);
			assert_atomic_dec(&fat_man);
		}
	}
}

void assert_atomic_and(tvc_atomic_t* a, uint64_t bits)
{
	uint64_t current = tvc_atomic_get(a);
	uint64_t expected = current & bits;
	uint64_t ret = tvc_atomic_bit_and(a, bits);
	TVC_ASSERT(ret == expected);
	assert_atomic_get(a, expected);
}

void assert_atomic_or(tvc_atomic_t* a, uint64_t bits)
{
	uint64_t current = tvc_atomic_get(a);
	uint64_t expected = current | bits;
	uint64_t ret = tvc_atomic_bit_or(a, bits);
	TVC_ASSERT(ret == expected);
	assert_atomic_get(a, expected);
}

void assert_atomic_xor(tvc_atomic_t* a, uint64_t bits)
{
	uint64_t current = tvc_atomic_get(a);
	uint64_t expected = current ^ bits;
	uint64_t ret = tvc_atomic_bit_xor(a, bits);
	TVC_ASSERT(ret == expected);
	assert_atomic_get(a, expected);
}

void test_atomic_bits()
{
	std::vector<uint64_t> vals = {
		0x0000000000000000,
		0xffffffffffffffff,
		0xaaaaaaaaaaaaaaaa,
		0x123456789abcdef0,
		0x00000000ffffffff,
		0x80000000ffffffff,
		0xffffffff00000000,
		0xffffffff80000000,
	};
	for (size_t b = 0; b < sizeof(uint64_t) * 8; b++)
		vals.push_back(1UL << b);

	tvc_atomic_t davy_crockett;

	for (uint64_t initial: vals) {
		for (uint64_t bits: vals) {
			tvc_atomic_uinit(&davy_crockett, initial);
			assert_atomic_and(&davy_crockett, bits);

			tvc_atomic_uinit(&davy_crockett, initial);
			assert_atomic_or(&davy_crockett, bits);

			tvc_atomic_uinit(&davy_crockett, initial);
			assert_atomic_xor(&davy_crockett, bits);
		}
	}
}

void test_atomic()
{
	test_atomic_init();
	test_atomic_get_set();
	test_atomic_arithmetic();
	test_atomic_bits();
}

int main(int argc, char** argv)
{
	/*
	 * as a unit test, this just tests semantics, we don't actually
	 * test atomicity.
	 */
	test_memory_barrier();
	test_atomic();
}
