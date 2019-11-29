#ifndef HAX_FREESTANDING
#include <stddef.h>
#else
/*	NetBSD: stddef.h,v 1.20 2016/03/20 16:26:06 christos Exp 	*/

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
 *	@(#)stddef.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _HAX_STDDEF_H_
#define _HAX_STDDEF_H_

#ifndef SIZE_T_DEFINED
typedef __SIZE_TYPE__ size_t;
#define SIZE_T_DEFINED
#endif

#ifndef SSIZE_T_DEFINED
typedef __PTRDIFF_TYPE__ ssize_t;
#define SSIZE_T_DEFINED
#endif

#ifndef OFF_T_DEFINED  
typedef __UINT64_TYPE__ off_t;   
#define OFF_T_DEFINED  
#endif

#ifndef PTRDIFF_T_DEFINED
typedef __PTRDIFF_TYPE__ ptrdiff_t;
#define PTRDIFF_T_DEFINED
#endif

#ifndef WCHAR_T_DEFINED
typedef __WCHAR_TYPE__ wchar_t;
#define WCHAR_T_DEFINED
#endif

#ifndef OFF_T_DEFINED  
typedef __UINT64_TYPE__ off_t;   
#define OFF_T_DEFINED  
#endif

#ifndef UINTMAX_T_DEFINED
typedef __UINTMAX_TYPE__ uintmax_t;
#define UINTMAX_T_DEFINED
#endif

#ifndef INTMAX_T_DEFINED
typedef __INTMAX_TYPE__ intmax_t;
#define INTMAX_T_DEFINED
#endif

#ifndef INTPTR_T_DEFINED
typedef __INTPTR_TYPE__ intptr_t;
#define INTPTR_T_DEFINED
#endif

#ifndef UINTPTR_T_DEFINED
typedef __UINTPTR_TYPE__ uintptr_t;
#define UINTPTR_T_DEFINED
#endif

#ifndef UINT8_T_DEFINED
typedef __UINT8_TYPE__ uint8_t;
#define UINT8_T_DEFINED
#endif

#ifndef UINT16_T_DEFINED
typedef __UINT16_TYPE__ uint16_t;
#define UINT16_T_DEFINED
#endif

#ifndef UINT32_T_DEFINED
typedef __UINT32_TYPE__ uint32_t;
#define UINT32_T_DEFINED
#endif

#ifndef UINT64_T_DEFINED
typedef __UINT64_TYPE__ uint64_t;
#define UINT64_T_DEFINED
#endif

#ifndef INT8_T_DEFINED
typedef __INT8_TYPE__ int8_t;
#define INT8_T_DEFINED
#endif

#ifndef INT16_T_DEFINED
typedef __INT16_TYPE__ int16_t;
#define INT16_T_DEFINED
#endif

#ifndef INT32_T_DEFINED
typedef __INT32_TYPE__ int32_t;
#define INT32_T_DEFINED
#endif

#ifndef INT64_T_DEFINED
typedef __INT64_TYPE__ int64_t;
#define INT64_T_DEFINED
#endif

#define NL_ARGMAX 9

#define ULONG_MAX (2UL*__LONG_MAX__+1UL)
#define INTMAX_MAX __INTMAX_MAX__
#define INT_MAX __INT_MAX__
#define DBL_MANT_DIG __DBL_MANT_DIG__
#define DBL_MAX_EXP __DBL_MAX_EXP__
#define DBL_EPSILON __DBL_EPSILON__

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef OFFSETOF_DEFINED
#define offsetof(type, member)  __builtin_offsetof(type, member)
#define OFFSETOF_DEFINED
#endif

#endif /* _HAX_STDDEF_H_ */
#endif
