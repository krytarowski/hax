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
typedef __INT64_TYPE__ off_t;
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

#ifndef CHAR_BIT
#define CHAR_BIT	__CHAR_BIT__ 
#endif

#ifndef SCHAR_MIN
#define SCHAR_MIN	(- __SCHAR_MAX__ - 1)
#endif

#ifndef SCHAR_MAX
#define SCHAR_MAX	__SCHAR_MAX__
#endif

#ifndef UCHAR_MAX
#define UCHAR_MAX	(2 * __SCHAR_MAX__ + 1)
#endif

#ifndef INT_MIN
#define INT_MIN		(- __INT_MAX__ - 1)
#endif

#ifndef INT_MAX
#define INT_MAX		__INT_MAX__
#endif

#ifndef UINT_MAX
#define UINT_MAX	(2U * __INT_MAX__ - 1U)
#endif

#ifndef LONG_MIN
#define LONG_MIN	(- __LONG_MAX__-1L)
#endif

#ifndef LONG_MAX
#define LONG_MAX	__LONG_MAX__
#endif

#ifndef ULONG_MAX
#define ULONG_MAX	(2UL * __LONG_MAX__ + 1UL)
#endif

#ifndef LLONG_MIN
#define LLONG_MIN	(-__LONG_LONG_MAX__ - 1)
#endif

#ifndef LLONG_MAX
#define LLONG_MAX	__LONG_LONG_MAX__
#endif

#ifndef ULLONG_MAX
#define ULLONG_MAX	(2ULL * __LONG_LONG_MAX__ + 1ULL)
#endif

#ifndef SSIZE_MAX
#define SSIZE_MAX	LONG_MAX
#endif

#ifndef SSIZE_MIN
#define SSIZE_MIN	LONG_MIN
#endif

#ifndef SIZE_T_MAX
#define SIZE_T_MAX	ULONG_MAX
#endif

#ifndef DBL_DIG
#define DBL_DIG		__DBL_DIG__
#endif

#ifndef DBL_MAX
#define DBL_MAX		__DBL_MAX__
#endif

#ifndef DBL_MIN
#define DBL_MIN		__DBL_MIN__
#endif

#ifndef FLT_DIG
#define FLT_DIG		__FLT_DIG__
#endif

#ifndef FLT_MAX
#define FLT_MAX		__FLT_MAX__
#endif

#ifndef FLT_MIN
#define FLT_MIN		__FLT_MIN__
#endif

#ifndef INTMAX_MAX
#define INTMAX_MAX	__INTMAX_MAX__
#endif

#ifndef DBL_MANT_DIG
#define DBL_MANT_DIG	__DBL_MANT_DIG__
#endif

#ifndef DBL_MIN_EXP
#define DBL_MIN_EXP	__DBL_MIN_EXP__
#endif

#ifndef DBL_MAX_EXP
#define DBL_MAX_EXP	__DBL_MAX_EXP__
#endif

#ifndef DBL_EPSILON
#define DBL_EPSILON	__DBL_EPSILON__
#endif

#ifndef FLT_MANT_DIG
#define FLT_MANT_DIG	__FLT_MANT_DIG__
#endif

#ifndef FLT_MIN_EXP
#define FLT_MIN_EXP	__FLT_MIN_EXP__
#endif

#ifndef FLT_MAX_EXP
#define FLT_MAX_EXP	__FLT_MAX_EXP__
#endif

#ifndef FLT_EPSILON
#define FLT_EPSILON	__FLT_EPSILON__
#endif

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef OFFSETOF_DEFINED
#define offsetof(type, member)  __builtin_offsetof(type, member)
#define OFFSETOF_DEFINED
#endif

#endif /* _HAX_STDDEF_H_ */
#endif
