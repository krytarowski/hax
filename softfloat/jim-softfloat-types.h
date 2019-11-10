
/*============================================================================

This C header file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3e, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014, 2015, 2016, 2017 The Regents of the
University of California.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions, and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions, and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. Neither the name of the University nor the names of its contributors may
    be used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS", AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ARE
DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#ifndef JIM_SOFTFLOAT_TYPES_H
#define JIM_SOFTFLOAT_TYPES_H

#include "jim.h"

/*----------------------------------------------------------------------------
| Auxiliary types.
*----------------------------------------------------------------------------*/

typedef unsigned char jim_uint8_t;
typedef unsigned short jim_uint16_t;
typedef unsigned int jim_uint32_t;
typedef unsigned long long jim_uint64_t;

typedef signed char jim_int8_t;
typedef signed short jim_int16_t;
typedef signed int jim_int32_t;
typedef signed long long jim_int64_t;

JIM_CTASSERT(sizeof(jim_uint8_t) == 1);
JIM_CTASSERT(sizeof(jim_uint16_t) == 2);
JIM_CTASSERT(sizeof(jim_uint32_t) == 4);
JIM_CTASSERT(sizeof(jim_uint64_t) == 8);

JIM_CTASSERT(sizeof(jim_int8_t) == 1);
JIM_CTASSERT(sizeof(jim_int16_t) == 2);
JIM_CTASSERT(sizeof(jim_int32_t) == 4);
JIM_CTASSERT(sizeof(jim_int64_t) == 8);

/*----------------------------------------------------------------------------
| Compat types
*----------------------------------------------------------------------------*/

typedef unsigned char jim_uint_fast8_t;
typedef unsigned short jim_uint_fast16_t;
typedef unsigned int jim_uint_fast32_t;
typedef unsigned long long jim_uint_fast64_t;

typedef signed char jim_int_fast8_t;
typedef signed short jim_int_fast16_t;
typedef signed int jim_int_fast32_t;
typedef signed long long jim_int_fast64_t;

typedef unsigned char jim_uint_least8_t;
typedef unsigned short jim_uint_least16_t;
typedef unsigned int jim_uint_least32_t;
typedef unsigned long long jim_uint_least64_t;

typedef signed char jim_int_least8_t;
typedef signed short jim_int_least16_t;
typedef signed int jim_int_least32_t;
typedef signed long long jim_int_least64_t;

typedef int jim_bool;

typedef struct { unsigned short v; } jim_float16_t;
typedef jim_float jim_float32_t;
typedef jim_double jim_float64_t;
typedef struct { uint64_t v[2]; } jim_float128_t;

JIM_CTASSERT(sizeof(jim_float16_t) == 2);
JIM_CTASSERT(sizeof(jim_float32_t) == 4);
JIM_CTASSERT(sizeof(jim_float64_t) == 8);
JIM_CTASSERT(sizeof(jim_float128_t) == 16);

struct jim_extFloat80M { jim_uint64_t signif; jim_uint16_t signExp; };

/*----------------------------------------------------------------------------
| Compat macros
*----------------------------------------------------------------------------*/

#define JIM_INT8_C(c)       c
#define JIM_INT16_C(c)      c
#define JIM_INT32_C(c)      c
#define JIM_INT64_C(c)      c##LL

#define JIM_UINT8_C(c)      c
#define JIM_UINT16_C(c)     c
#define JIM_UINT32_C(c)     c
#define JIM_UINT64_C(c)     c##ULL

#define JIM_INTMAX_C(c)     c##L
#define JIM_UINTMAX_C(c)    c##UL

JIM_CTASSERT(sizeof(long) == sizeof(void *));
JIM_CTASSERT(sizeof(unsigned long) == sizeof(void *));

#define JIM_INLINE static inline
#define JIM_THREAD_LOCAL

#define INLINE_LEVEL 5

/*----------------------------------------------------------------------------
| These macros are used to isolate the differences in word order between big-
| endian and little-endian platforms.
*----------------------------------------------------------------------------*/

#define jim_wordIncr 1
#define jim_indexWord( total, n ) (n)
#define jim_indexWordHi( total ) ((total) - 1)
#define jim_indexWordLo( total ) 0
#define jim_indexMultiword( total, m, n ) (n)
#define jim_indexMultiwordHi( total, n ) ((total) - (n))
#define jim_indexMultiwordLo( total, n ) 0
#define jim_indexMultiwordHiBut( total, n ) (n)
#define jim_indexMultiwordLoBut( total, n ) 0
#define JIM_INIT_UINTM4( v3, v2, v1, v0 ) { v0, v1, v2, v3 }

#endif
