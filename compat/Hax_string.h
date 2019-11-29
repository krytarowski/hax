#ifndef HAX_FREESTANDING
#include <string.h>
#else
/*	NetBSD: string.h,v 1.52 2018/02/20 02:35:24 kamil Exp 	*/

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
 *	@(#)string.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _STRING_H_
#define	_STRING_H_

#include "Hax_stddef.h"

void	*memcpy(void * restrict, const void * restrict, size_t);
void	*memset(void *, int, size_t);
char	*strcat(char * restrict, const char * restrict);
char	*strchr(const char *, int);
int	 strcmp(const char *, const char *);
char	*strcpy(char * restrict, const char * restrict);
size_t	 strcspn(const char *, const char *);
size_t	 strlen(const char *);
size_t	 strspn(const char *, const char *);
char	*strstr(const char *, const char *);
int	strncmp(const char *, const char *, size_t);
char	*strncpy(char * restrict, const char * restrict, size_t);
#endif /* !defined(_HAX_STRING_H_) */
#endif
