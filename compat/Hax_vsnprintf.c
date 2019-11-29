#ifdef HAX_FREESTANDING
#include "Hax_stdio.h"
#include "Hax_stdarg.h"
#include "Hax_stddef.h"
#include "Hax_string.h"

#include "Hax_stdio_impl.h"

struct cookie {
	char *s;
	size_t n;
};

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t Hax_sn_write(FILE *f, const unsigned char *s, size_t l)
{
	struct cookie *c = f->cookie;
	size_t k = MIN(c->n, f->wpos - f->wbase);
	if (k) {
		memcpy(c->s, f->wbase, k);
		c->s += k;
		c->n -= k;
	}
	k = MIN(c->n, l);
	if (k) {
		memcpy(c->s, s, k);
		c->s += k;
		c->n -= k;
	}
	*c->s = 0;
	f->wpos = f->wbase = f->buf;
	/* pretend to succeed, even if we discarded extra data */
	return l;
}

int Hax_vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	unsigned char buf[1];
	char dummy[1];
	struct cookie c = { .s = n ? s : dummy, .n = n ? n-1 : 0 };
	FILE f = {
		.lbf = EOF,
		.write = Hax_sn_write,
		.lock = -1,
		.buf = buf,
		.cookie = &c,
	};

	if (n > INT_MAX) {
		return -1;
	}

	*c.s = 0;
	return Hax_vfprintf(&f, fmt, ap);
}
#endif
