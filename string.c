#include "trivc/string.h"

#include <string.h>

const char *tvc_str0(const char *s)
{
	return s ? s : "";
}

bool tvc_streq(const char *a, const char *b)
{
	if (a == b)
		return true;
	if (!a || !b)
		return false;
	return strcmp(a, b) == 0;
}

