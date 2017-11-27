#include "trivc/string.h"

#include <string.h>
#include <stdbool.h>

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

const char *tvc_strpbrk0(const char *s, const char *accept)
{
	const char *s0 = tvc_str0(s);
	const char *res = strpbrk(s0, tvc_str0(accept));
	if (!res)
		res = s + strlen(s0); /* TODO optimize strpbrk()+strlen() */
	return tvc_str0(res);
}

void __tvc_str_foreach_init(const char **beginp, const char**endp,
		const char *s, const char *delims)
{
	*beginp = s;
	*endp = tvc_strpbrk0(*beginp, delims);
}

bool __tvc_str_foreach_valid(const char **beginp, const char** endp)
{
	return **beginp != '\0';
}

void __tvc_str_foreach_advance( const char **beginp, const char **endp,
		const char *delims)
{
	if (**endp == '\0') {
		*beginp = *endp;
		*endp = NULL;
	} else {
		*beginp = *endp + 1;
		*endp = tvc_strpbrk0(*beginp, delims);
	}
}
