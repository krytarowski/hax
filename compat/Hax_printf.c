#include "Hax_stdio.h"
#include "Hax_stdarg.h"

#include "Hax_stdio_impl.h"

int Hax_printf(const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = Hax_vfprintf(stdout, fmt, ap);
	va_end(ap);
	return ret;
}
