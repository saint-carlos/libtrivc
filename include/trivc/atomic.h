#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void tvc_memory_barrier(void);

/**
 * struct tvc_atomic - sequentially consistent 64-bit atomic variable
 *
 * can be used for both signed and unsigned computations.
 * unless otherwise specified, all functions below return the value after the
 * operation.
 *
 * the unsigned variants of interfaces exist for convenience of passing
 * arguments. as long as @uval, numeric function arguments, and the numeric
 * result of the operation are all in the range [0, 2^63), all signed operations
 * set and return the same things as their unsigned counterparts, and
 * @uval == @val.
 */
typedef union tvc_atomic {
	volatile int64_t val;
	volatile uint64_t uval;
} tvc_atomic_t;

#define TVC_ATOMIC_INIT(v) { v }

void tvc_atomic_init(tvc_atomic_t *a, int64_t v);
void tvc_atomic_uinit(tvc_atomic_t *a, uint64_t v);
int64_t tvc_atomic_get(const tvc_atomic_t *a);
int64_t tvc_atomic_set(tvc_atomic_t *a, int64_t v);
uint64_t tvc_atomic_uget(const tvc_atomic_t *a);
uint64_t tvc_atomic_uset(tvc_atomic_t *a, int64_t v);

int64_t tvc_atomic_add(tvc_atomic_t *a, int64_t v);
int64_t tvc_atomic_sub(tvc_atomic_t *a, int64_t v);
int64_t tvc_atomic_inc(tvc_atomic_t *a);
int64_t tvc_atomic_dec(tvc_atomic_t *a);
uint64_t tvc_atomic_bit_and(tvc_atomic_t *a, uint64_t bits);
uint64_t tvc_atomic_bit_or(tvc_atomic_t *a, uint64_t bits);
uint64_t tvc_atomic_bit_xor(tvc_atomic_t *a, uint64_t bits);

/* these operations return whether or not @a was assigned */
bool tvc_atomic_test_and_set(tvc_atomic_t *a, int64_t prev, int64_t next);
bool tvc_atomic_utest_and_set(tvc_atomic_t *a, uint64_t prev, uint64_t next);

/* these operations return the value of @a before the operation */
int64_t tvc_atomic_compare_and_swap(tvc_atomic_t *a, int64_t prev, int64_t next);
uint64_t tvc_atomic_ucompare_and_swap(tvc_atomic_t *a, uint64_t prev, uint64_t next);

/**
 * struct tvc_refcount - refcount interface for atomic variable
 *
 * when tvc_refcount_put() returns 'true', it means that the refcount went to 0
 * and the object being refcounted can be released.
 */
typedef struct tvc_refcount {
	tvc_atomic_t count;
} tvc_refcount_t;

void tvc_refcount_init(tvc_refcount_t *refcount, uint64_t val);
void tvc_refcount_get(tvc_refcount_t *refcount);
bool tvc_refcount_put(tvc_refcount_t *refcount);
uint64_t tvc_refcount_count(tvc_refcount_t *refcount);

#ifdef __cplusplus
}
#endif
