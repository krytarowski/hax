#include "Hax_stdio_impl.h"

static size_t Hax_do_read(FILE *f, unsigned char *buf, size_t len)
{
	return Hax___string_read(f, buf, len);
}

int Hax_vsscanf(const char *restrict s, const char *restrict fmt, va_list ap)
{
	FILE f = {
		.buf = (void *)s, .cookie = (void *)s,
		.read = Hax_do_read, .lock = -1
	};
	return Hax_vfscanf(&f, fmt, ap);
}
