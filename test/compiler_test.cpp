#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cstdalign>

#define __TRIVC_UT
#include "trivc/trivc.h"

#include "trivc/compiler.h"

static void test_array_size()
{
	uint8_t a1[100];
	TVC_ASSERT(TVC_ARRAY_SIZE(a1) == 100);

	uint32_t a2[12];
	TVC_ASSERT(TVC_ARRAY_SIZE(a2) == 12);

	struct {
		int x;
		char y;
	} a3[5];
	TVC_ASSERT(TVC_ARRAY_SIZE(a3) == 5);

	int a4[1];
	TVC_ASSERT(TVC_ARRAY_SIZE(a4) == 1);
}

static void test_trivc_container_of()
{
	typedef struct cot {
		char x;
		int y;
	} cot_t;

	struct cot c;
	TVC_ASSERT(tvc_container_of(&c.x, cot_t, x));
	TVC_ASSERT(tvc_container_of(&c.y, cot_t, y));
}

static void test_str()
{
	TVC_ASSERT(strcmp(TVC_STR(abc), "abc") == 0);
	TVC_ASSERT(strcmp(TVC_PSTR(abc), "abc") == 0);
#define DEF def
	TVC_ASSERT(strcmp(TVC_STR(DEF), "DEF") == 0);
	TVC_ASSERT(strcmp(TVC_PSTR(DEF), "def") == 0);
#undef DEF
}

static void test_filebase()
{
	/* we want it from two different calls */
	TVC_ASSERT(strcmp(__TVC_FILEBASE__, "compiler_test.cpp") == 0);
	TVC_ASSERT(strcmp(__TVC_FILEBASE__, "compiler_test.cpp") == 0);
}

static void __trivc_format_test(const char* fmt, void* lie, ...)
	__tvc_printf(1, 3);
static void __trivc_format_test(const char* fmt, void* lie, ...)
{
	va_list vl;
	char buf[100];
	va_start(vl, lie);
	vsprintf(buf, fmt, vl);
	va_end(vl);
	assert(strcmp(buf, "testing") == 0);
}

typedef struct __tvc_packed mystruct {
	uint8_t		u8_1;
	uint64_t	u64_2;
	uint8_t		u8_3;
} mystruct_t;

static void test_unused_deprecated() __tvc_unused __tvc_deprecated;
static void test_unused_deprecated()
{
	TVC_ASSERT(false);
}

static void test_unused_used() __tvc_unused;
static void test_unused_used()
{
	/* no-op */
}

static void test_noreturn() __tvc_noreturn;
static void test_noreturn()
{
	TVC_ASSERT(false);
}

static void test_compiler_attrs()
{
	size_t sz = 0;
	TVC_ASSERT(offsetof(mystruct_t, u8_1) == sz);
	sz += sizeof(uint8_t);
	TVC_ASSERT(offsetof(mystruct_t, u64_2) == sz);
	sz += sizeof(uint64_t);
	TVC_ASSERT(offsetof(mystruct_t, u8_3) == sz);

	__trivc_format_test("%s", NULL, "testing");

	test_unused_used();

	TVC_ASSERT(sz > 0);
	if (sz == 0)
		test_noreturn();
}

int main(int argc, char** argv)
{
	test_array_size();
	test_trivc_container_of();
	test_str();
	test_filebase();
	test_compiler_attrs();
}
