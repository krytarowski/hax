#include "Hax_stdio_impl.h"

#ifdef __cplusplus
#   define HAX_EXTERN extern "C"
#else
#   define HAX_EXTERN extern
#endif
 
HAX_EXTERN void Hax_Panic(char *format, ...);

off_t Hax___stdio_seek(FILE *f, off_t off, int whence)
{
	Hax_Panic((char *) "Not supported");
	return 0;
}
