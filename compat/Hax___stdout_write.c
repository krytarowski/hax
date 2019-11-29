#ifdef HAX_FREESTANDING
#include "Hax_stdio_impl.h"

size_t Hax___stdout_write(FILE *f, const unsigned char *buf, size_t len)
{
	f->write = Hax___stdio_write;
	return Hax___stdio_write(f, buf, len);
}
#endif
