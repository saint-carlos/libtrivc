#include "trivc/atomic.h"

#include "trivc/trivc.h"

#define MEMORDER __ATOMIC_SEQ_CST

void tvc_memory_barrier(void)
{
	__atomic_thread_fence(MEMORDER);
}

void tvc_atomic_init(tvc_atomic_t *a, int64_t v)
{
	a->val = v;
}

void tvc_atomic_uinit(tvc_atomic_t *a, uint64_t v)
{
	a->uval = v;
}

int64_t tvc_atomic_get(const tvc_atomic_t *a)
{
	return __atomic_load_n(&a->val, MEMORDER);
}

int64_t tvc_atomic_set(tvc_atomic_t *a, int64_t v)
{
	__atomic_store_n(&a->val, v, MEMORDER);
	return v;
}

uint64_t tvc_atomic_uget(const tvc_atomic_t *a)
{
	return __atomic_load_n(&a->uval, MEMORDER);
}

uint64_t tvc_atomic_uset(tvc_atomic_t *a, int64_t v)
{
	__atomic_store_n(&a->uval, v, MEMORDER);
	return v;
}

int64_t tvc_atomic_add(tvc_atomic_t *a, int64_t v)
{
	return __atomic_add_fetch(&a->val, v, MEMORDER);
}

int64_t tvc_atomic_sub(tvc_atomic_t *a, int64_t v)
{
	return __atomic_sub_fetch(&a->val, v, MEMORDER);
}

int64_t tvc_atomic_inc(tvc_atomic_t *a)
{
	return tvc_atomic_add(a, 1);
}

int64_t tvc_atomic_dec(tvc_atomic_t *a)
{
	return tvc_atomic_sub(a, 1);
}

uint64_t tvc_atomic_bit_and(tvc_atomic_t *a, uint64_t bits)
{
	return __atomic_and_fetch(&a->uval, bits, MEMORDER);
}

uint64_t tvc_atomic_bit_or(tvc_atomic_t *a, uint64_t bits)
{
	return __atomic_or_fetch(&a->uval, bits, MEMORDER);
}

uint64_t tvc_atomic_bit_xor(tvc_atomic_t *a, uint64_t bits)
{
	return __atomic_xor_fetch(&a->uval, bits, MEMORDER);
}

bool tvc_atomic_test_and_set(tvc_atomic_t *a, int64_t prev, int64_t next)
{
	int64_t ret = tvc_atomic_compare_and_swap(a, prev, next);
	return ret == prev;
}

bool tvc_atomic_utest_and_set(tvc_atomic_t *a, uint64_t prev, uint64_t next)
{
	uint64_t ret = tvc_atomic_ucompare_and_swap(a, prev, next);
	return ret == prev;
}

int64_t tvc_atomic_compare_and_swap(tvc_atomic_t *a, int64_t prev, int64_t next)
{
	int64_t expected = prev;
	bool ret;
	ret = __atomic_compare_exchange_n(&a->val, &expected, next,
			false, MEMORDER, MEMORDER);
	TVC_ASSERT((ret == true) == (expected == prev));
	return expected;
}

uint64_t tvc_atomic_ucompare_and_swap(tvc_atomic_t *a, uint64_t prev, uint64_t next)
{
	uint64_t expected = prev;
	bool ret;
	ret = __atomic_compare_exchange_n(&a->uval, &expected, next,
			false, MEMORDER, MEMORDER);
	TVC_ASSERT((ret == true) == (expected == prev));
	return expected;
}
