#ifndef HAX_FREESTANDING
#include <stdio.h>
#else
/*	NetBSD: stdio.h,v 1.97 2016/03/17 00:42:49 christos Exp 	*/

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
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
 *	@(#)stdio.h	8.5 (Berkeley) 4/29/95
 */

#ifndef	_HAX_STDIO_H_
#define	_HAX_STDIO_H_

#ifndef NULL
#define NULL	((void *)0)
#endif

#ifndef EOF
#define	EOF	(-1)
#endif

#ifndef __printflike
#define __printflike(fmtarg, firstvararg)	\
	__attribute__((__format__ (__printf__, fmtarg, firstvararg)))
#endif

#ifndef __scanflike
#define __scanflike(fmtarg, firstvararg)	\
	__attribute__((__format__ (__scanf__, fmtarg, firstvararg)))
#endif

#define printf(x, ...) Hax_printf(x, ## __VA_ARGS__)
#define sscanf(x, y, ...) Hax_sscanf(x, y, ## __VA_ARGS__)
#define sprintf(x, y, ...) Hax_sprintf(x, y, ## __VA_ARGS__)

int	 Hax_printf(const char * restrict, ...)
		__printflike(1, 2);
int	 Hax_sscanf(const char * restrict, const char * restrict, ...)
		__scanflike(2, 3);
int	 Hax_sprintf(char * restrict, const char * restrict, ...)
		__printflike(2, 3);

#endif /* _HAX_STDIO_H_ */
#endif
