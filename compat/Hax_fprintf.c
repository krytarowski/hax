#ifdef HAX_FREESTANDING
#include "Hax_stdio.h"
#include "Hax_stdarg.h"

#include "Hax_stdio_impl.h"

int Hax_fprintf(FILE *restrict f, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = Hax_vfprintf(f, fmt, ap);
	va_end(ap);
	return ret;
}
#endif
