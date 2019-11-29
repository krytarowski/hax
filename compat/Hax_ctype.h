#ifndef HAX_FREESTANDING
#include <ctype.h>
#else
/*	NetBSD: ctype.h,v 1.34 2013/04/28 19:39:56 joerg Exp 	*/

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)ctype.h	5.3 (Berkeley) 4/3/91
 */

#ifndef _HAX_CTYPE_H_
#define _HAX_CTYPE_H_

#define isalnum(x) Hax_isalnum(x)
#define	isalpha(x) Hax_isalpha(x)
#define	iscntrl(x) Hax_iscntrl(x)
#define	isdigit(x) Hax_iscntrl(x)
#define	isgraph(x) Hax_isgraph(x)
#define	islower(x) Hax_islower(x)
#define	isprint(x) Hax_isprint(x)
#define	ispunct(x) Hax_ispunct(x)
#define	isspace(x) Hax_isspace(x)
#define	isupper(x) Hax_isupper(x)
#define	isxdigit(x) Hax_isxdigit(x)
#define	tolower(x) Hax_tolower(x)
#define	toupper(x) Hax_toupper(x)

#define	isascii(x) Hax_isascii(x)
#define	toascii(x) Hax_toascii(x)
#define	_tolower(x) Hax__tolower(x)
#define _toupper(x) Hax__toupper(x)

#define isblank(x) Hax_isblank(x)

#include "Hax_ctype_inline.h"

#endif /* !_HAX_CTYPE_H_ */
#endif
