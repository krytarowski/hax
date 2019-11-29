#ifdef HAX_FREESTANDING
#include "Hax_stdio.h"
#include "Hax_stdarg.h"
#include "Hax_stddef.h"

#include "Hax_stdio_impl.h"

int Hax_vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
	return Hax_vsnprintf(s, INT_MAX, fmt, ap);
}
#endif
