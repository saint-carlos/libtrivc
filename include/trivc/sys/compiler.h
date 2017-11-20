#pragma once

#include <trivc/compiler.h>

#define __printf	__tvc_printf
#define __packed	__tvc_packed
#define __unused	__tvc_unused
#define __noreturn	__tvc_noreturn
#define __deprecated	__tvc_deprecated

#define ARRAY_SIZE	TVC_ARRAY_SIZE
#define container_of	tvc_container_of
#define __FILEBASE__	__TVC_FILEBASE__
