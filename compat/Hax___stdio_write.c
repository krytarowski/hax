#include "Hax_stdio_impl.h"

#ifdef __cplusplus
#   define HAX_EXTERN extern "C"
#else
#   define HAX_EXTERN extern
#endif
 
HAX_EXTERN int Hax_PutChar(int c);

ssize_t
Hax_fake_writev(int d, const struct iovec *iov, int iovcnt)
{
	ssize_t printed;
	int i;
	size_t n;

	printed = 0;
	for (i = 0; i < iovcnt; i++) {
		for (n = 0; n < iov[i].iov_len; n++) {
			Hax_PutChar(((char *)iov[i].iov_base)[n]);
			printed++;
		}
	}

	return printed;
}

size_t Hax___stdio_write(FILE *f, const unsigned char *buf, size_t len)
{
	struct iovec iovs[2] = {
		{ .iov_base = f->wbase, .iov_len = f->wpos-f->wbase },
		{ .iov_base = (void *)buf, .iov_len = len }
	};
	struct iovec *iov = iovs;
	size_t rem = iov[0].iov_len + iov[1].iov_len;
	int iovcnt = 2;
	ssize_t cnt;
	for (;;) {
		cnt = Hax_fake_writev(f->fd, iov, iovcnt);
		if (cnt == rem) {
			f->wend = f->buf + f->buf_size;
			f->wpos = f->wbase = f->buf;
			return len;
		}
		if (cnt < 0) {
			f->wpos = f->wbase = f->wend = 0;
			f->flags |= F_ERR;
			return iovcnt == 2 ? 0 : len-iov[0].iov_len;
		}
		rem -= cnt;
		if (cnt > iov[0].iov_len) {
			cnt -= iov[0].iov_len;
			iov++; iovcnt--;
		}
		iov[0].iov_base = (char *)iov[0].iov_base + cnt;
		iov[0].iov_len -= cnt;
	}
}
