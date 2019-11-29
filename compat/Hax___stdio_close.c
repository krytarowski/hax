#include "Hax_stdio_impl.h"

#ifdef __cplusplus
#   define HAX_EXTERN extern "C"
#else
#   define HAX_EXTERN extern
#endif
 
HAX_EXTERN void Hax_Panic(char *format, ...);

static int Hax_dummy(int fd)
{
	return fd;
}

int Hax___stdio_close(FILE *f)
{
	Hax_Panic((char *) "Not supported");
	return 0;
}
