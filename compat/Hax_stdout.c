#include "Hax_stdio_impl.h"

#undef stdout

static unsigned char buf[BUFSIZ+UNGET];
FILE Hax___stdout_FILE = {
	.buf = buf+UNGET,
	.buf_size = sizeof buf-UNGET,
	.fd = 1,
	.flags = F_PERM | F_NORD,
	.lbf = '\n',
	.write = Hax___stdout_write,
	.seek = Hax___stdio_seek,
	.close = Hax___stdio_close,
	.lock = -1,
};
FILE *const Hax_stdout = &Hax___stdout_FILE;
FILE *volatile Hax___stdout_used = &Hax___stdout_FILE;
