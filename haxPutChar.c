/*
 * haxPutChar.c --
 *
 *	Source code for the "Hax_PutChar" library procedure for
 *	Hax; individual applications will probably override this
 *	with an environment-specific putchar() procedure.
 *
 * Copyright 1988-1991 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appears in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#include <stdio.h>

#ifdef __cplusplus
#   define HAX_EXTERN extern "C"
#else
#   define HAX_EXTERN extern
#endif

HAX_EXTERN int Hax_PutChar(int c);


/*
 *----------------------------------------------------------------------
 *
 * Hax_PutChar --
 *
 *	Print a single character.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	A character is printed on the output of the environment.
 *
 *----------------------------------------------------------------------
 */

int
Hax_PutChar(
    int c)
{
    return putchar(c);
}
