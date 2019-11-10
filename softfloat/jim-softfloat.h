
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

#ifndef JIM_SOFTFLOAT_H
#define JIM_SOFTFLOAT_H

#include <jim.h>

#ifndef JIM_FLOATS_H
#error This header shall not be included directly
#endif

#ifndef HAVE_SOFTFLOAT
#error SoftFloat IEEE emulation not found
#endif

#ifndef HAVE_LONG_LONG
#error Compiler configuration not supported
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
| SoftFloat implementation assumptions
*----------------------------------------------------------------------------*/

JIM_CTASSERT(sizeof(jim_wide) == 8);

/*----------------------------------------------------------------------------
| jimtcl soft float/double
*----------------------------------------------------------------------------*/

typedef struct { unsigned int v; } jim_float;
typedef struct { unsigned long long int v; } jim_double;

JIM_CTASSERT(sizeof(jim_float) == 4);
JIM_CTASSERT(sizeof(jim_double) == 8);

/*----------------------------------------------------------------------------
| Integer-to-floating-point conversion routines.
*----------------------------------------------------------------------------*/
jim_float jim_wide_to_float( jim_wide );
jim_double jim_wide_to_double( jim_wide );

/*----------------------------------------------------------------------------
| 32-bit (single-precision) floating-point operations.
*----------------------------------------------------------------------------*/
jim_wide jim_float_to_wide( jim_float );
jim_double jim_float_to_double( jim_float );

jim_float jim_float_add( jim_float, jim_float );
jim_float jim_float_sub( jim_float, jim_float );
jim_float jim_float_mul( jim_float, jim_float );
jim_float jim_float_div( jim_float, jim_float );

int jim_float_eq( jim_float, jim_float );
int jim_float_le( jim_float, jim_float );
int jim_float_lt( jim_float, jim_float );
int jim_float_neq( jim_float, jim_float );
int jim_float_ge( jim_float, jim_float );
int jim_float_gt( jim_float, jim_float );

int jim_float_isnan( jim_float );
int jim_float_isinf( jim_float );

jim_float jim_float_sin( jim_float );
jim_float jim_float_cos( jim_float );
jim_float jim_float_tan( jim_float );
jim_float jim_float_asin( jim_float );
jim_float jim_float_acos( jim_float );
jim_float jim_float_atan( jim_float );
jim_float jim_float_atan2( jim_float );
jim_float jim_float_sinh( jim_float );
jim_float jim_float_cosh( jim_float );
jim_float jim_float_tanh( jim_float );
jim_float jim_float_ceil( jim_float );
jim_float jim_float_floor( jim_float );
jim_float jim_float_exp( jim_float );
jim_float jim_float_log( jim_float );
jim_float jim_float_log10( jim_float );
jim_float jim_float_sqrt( jim_float );
jim_float jim_float_pow( jim_float );
jim_float jim_float_hypot( jim_float );
jim_float jim_float_fmod( jim_float );

jim_float jim_strtof( const char *, char ** );
int jim_stof( jim_float, const char *, char *, unsigned int );

/*----------------------------------------------------------------------------
| 64-bit (double-precision) floating-point operations.
*----------------------------------------------------------------------------*/

jim_wide jim_double_to_wide( jim_double );
jim_float jim_double_to_float( jim_double );

jim_double jim_double_add( jim_double, jim_double );
jim_double jim_double_sub( jim_double, jim_double );
jim_double jim_double_mul( jim_double, jim_double );
jim_double jim_double_div( jim_double, jim_double );

int jim_double_eq( jim_double, jim_double );
int jim_double_le( jim_double, jim_double );
int jim_double_lt( jim_double, jim_double );
int jim_double_neq( jim_double, jim_double );
int jim_double_ge( jim_double, jim_double );
int jim_double_gt( jim_double, jim_double );

int jim_double_isnan( jim_double );
int jim_double_isinf( jim_double );

jim_double jim_double_sin( jim_double );
jim_double jim_double_cos( jim_double );
jim_double jim_double_tan( jim_double );
jim_double jim_double_asin( jim_double );
jim_double jim_double_acos( jim_double );
jim_double jim_double_atan( jim_double );
jim_double jim_double_atan2( jim_double );
jim_double jim_double_sinh( jim_double );
jim_double jim_double_cosh( jim_double );
jim_double jim_double_tanh( jim_double );
jim_double jim_double_ceil( jim_double );
jim_double jim_double_floor( jim_double );
jim_double jim_double_exp( jim_double );
jim_double jim_double_log( jim_double );
jim_double jim_double_log10( jim_double );
jim_double jim_double_sqrt( jim_double );
jim_double jim_double_pow( jim_double );
jim_double jim_double_hypot( jim_double );
jim_double jim_double_fmod( jim_double );

jim_double jim_strtod( const char *, char ** );
int jim_stod( jim_double, const char *, char *, unsigned int );

/*----------------------------------------------------------------------------
| jimtcl soft float/double commonly used constants
*----------------------------------------------------------------------------*/

#define JIM_DOUBLE_ZERO jim_wide_to_double(0)
#define JIM_DOUBLE_ONE jim_wide_to_double(1)
#define JIM_DOUBLE_MINUSONE jim_wide_to_double(-1)
#define JIM_DOUBLE_HALF jim_double_div(jim_wide_to_double(1), jim_wide_to_double(2))
#define JIM_DOUBLE_THOUSAND jim_wide_to_double(1000)
#define JIM_DOUBLE_MILLION jim_wide_to_double(1000000)

#ifdef __cplusplus
}
#endif

#endif
