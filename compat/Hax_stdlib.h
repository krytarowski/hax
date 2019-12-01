#ifndef HAX_FREESTANDING
#include <stdlib.h>
#else
/*	NetBSD: stdlib.h,v 1.121 2019/01/05 09:16:46 maya Exp 	*/

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)stdlib.h	8.5 (Berkeley) 5/19/95
 */

#ifndef _HAX_STDLIB_H_
#define _HAX_STDLIB_H_

#include "Hax_stddef.h"

#define atoi(x) Hax_atoi(x)
#define calloc(x, y) Hax_calloc((x), (y))
#define free(x) Hax_free(x)
#define malloc(x) Hax_malloc(x)
#define qsort(a,b,c,d) Hax_qsort((a), (b), (c), (d))
#define realloc(x, y) Hax_realloc((x), (y))
#define strtod(erange, x, y) Hax_strtod((erange), (x), (y))
#define strtol(erange, x, y, z) Hax_strtol((erange), (x), (y), (z))
#define strtoul(ergange, x, y, z) Hax_strtoul((erange), (x), (y), (z))
#define strtoll(ergange, x, y, z) Hax_strtoll((erange), (x), (y), (z))

int	 Hax_atoi(const char *);
void	*Hax_calloc(size_t, size_t);
void	 Hax_free(void *);
void	*Hax_malloc(size_t);
void	 Hax_qsort(void *, size_t, size_t, int (*)(const void *, const void *));
void	*Hax_realloc(void *, size_t);
double	 Hax_strtod(int *erange, const char * restrict, char ** restrict);
long	 Hax_strtol(int *erange, const char * restrict, char ** restrict, int);
unsigned long
	 Hax_strtoul(int *erange, const char * restrict, char ** restrict, int);

long long int	Hax_strtoll(int *erange, const char * restrict, char ** restrict, int);

#endif /* !_HAX_STDLIB_H_ */
#endif
