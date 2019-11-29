#ifdef HAX_FREESTANDING
#include "Hax_stdio.h"
#include "Hax_stdarg.h"

#include "Hax_stdio_impl.h"

int Hax_sscanf(const char *restrict s, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = Hax_vsscanf(s, fmt, ap);
	va_end(ap);
	return ret;
}
#endif
