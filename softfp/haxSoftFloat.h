
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

#ifndef _HAXSOFTFLOAT
#define _HAXSOFTFLOAT

#if !defined(_HAX) || !defined(_HAXINT)
#error This header shall not be included directly
#endif

#ifndef HAX_SOFTFLOAT
#error SoftFloat IEEE emulation not found
#endif

/*----------------------------------------------------------------------------
| SoftFloat implementation assumptions
*----------------------------------------------------------------------------*/

HAX_CTASSERT(sizeof(hax_wide) == 8);

/*----------------------------------------------------------------------------
| Hax soft float/double
*----------------------------------------------------------------------------*/

typedef struct { unsigned int v; } hax_float;
typedef struct { unsigned long long int v; } hax_double;

HAX_CTASSERT(sizeof(hax_float) == 4);
HAX_CTASSERT(sizeof(hax_double) == 8);

/*----------------------------------------------------------------------------
| Integer-to-floating-point conversion routines.
*----------------------------------------------------------------------------*/
hax_float hax_wide_to_float( hax_wide );
hax_double hax_wide_to_double( hax_wide );

/*----------------------------------------------------------------------------
| 32-bit (single-precision) floating-point operations.
*----------------------------------------------------------------------------*/
hax_wide hax_float_to_wide( hax_float );
hax_double hax_float_to_double( hax_float );

hax_float hax_float_add( hax_float, hax_float );
hax_float hax_float_sub( hax_float, hax_float );
hax_float hax_float_mul( hax_float, hax_float );
hax_float hax_float_div( hax_float, hax_float );

int hax_float_eq( hax_float, hax_float );
int hax_float_le( hax_float, hax_float );
int hax_float_lt( hax_float, hax_float );
int hax_float_neq( hax_float, hax_float );
int hax_float_ge( hax_float, hax_float );
int hax_float_gt( hax_float, hax_float );

int hax_float_isnan( hax_float );
int hax_float_isinf( hax_float );

hax_float hax_float_sin( hax_float );
hax_float hax_float_cos( hax_float );
hax_float hax_float_tan( hax_float );
hax_float hax_float_asin( hax_float );
hax_float hax_float_acos( hax_float );
hax_float hax_float_atan( hax_float );
hax_float hax_float_atan2( hax_float );
hax_float hax_float_sinh( hax_float );
hax_float hax_float_cosh( hax_float );
hax_float hax_float_tanh( hax_float );
hax_float hax_float_ceil( hax_float );
hax_float hax_float_floor( hax_float );
hax_float hax_float_exp( hax_float );
hax_float hax_float_log( hax_float );
hax_float hax_float_log10( hax_float );
hax_float hax_float_sqrt( hax_float );
hax_float hax_float_pow( hax_float );
hax_float hax_float_hypot( hax_float );
hax_float hax_float_fmod( hax_float );

hax_float hax_strtof( const char *, char ** );
int hax_stof( hax_float, const char *, char *, unsigned int );

/*----------------------------------------------------------------------------
| 64-bit (double-precision) floating-point operations.
*----------------------------------------------------------------------------*/

hax_wide hax_double_to_wide( hax_double );
hax_float hax_double_to_float( hax_double );

hax_double hax_double_add( hax_double, hax_double );
hax_double hax_double_sub( hax_double, hax_double );
hax_double hax_double_mul( hax_double, hax_double );
hax_double hax_double_div( hax_double, hax_double );

int hax_double_eq( hax_double, hax_double );
int hax_double_le( hax_double, hax_double );
int hax_double_lt( hax_double, hax_double );
int hax_double_neq( hax_double, hax_double );
int hax_double_ge( hax_double, hax_double );
int hax_double_gt( hax_double, hax_double );

int hax_double_isnan( hax_double );
int hax_double_isinf( hax_double );

hax_double hax_double_sin( hax_double );
hax_double hax_double_cos( hax_double );
hax_double hax_double_tan( hax_double );
hax_double hax_double_asin( hax_double );
hax_double hax_double_acos( hax_double );
hax_double hax_double_atan( hax_double );
hax_double hax_double_atan2( hax_double );
hax_double hax_double_sinh( hax_double );
hax_double hax_double_cosh( hax_double );
hax_double hax_double_tanh( hax_double );
hax_double hax_double_ceil( hax_double );
hax_double hax_double_floor( hax_double );
hax_double hax_double_exp( hax_double );
hax_double hax_double_log( hax_double );
hax_double hax_double_log10( hax_double );
hax_double hax_double_sqrt( hax_double );
hax_double hax_double_pow( hax_double );
hax_double hax_double_hypot( hax_double );
hax_double hax_double_fmod( hax_double );

hax_double hax_strtod( const char *, char ** );
int hax_stod( hax_double, const char *, char *, unsigned int );

/*----------------------------------------------------------------------------
| Hax soft float/double commonly used constants
*----------------------------------------------------------------------------*/

#define hax_DOUBLE_ZERO hax_wide_to_double(0)
#define hax_DOUBLE_ONE hax_wide_to_double(1)
#define hax_DOUBLE_MINUSONE hax_wide_to_double(-1)
#define hax_DOUBLE_HALF hax_double_div(hax_wide_to_double(1), hax_wide_to_double(2))
#define hax_DOUBLE_THOUSAND hax_wide_to_double(1000)
#define hax_DOUBLE_MILLION hax_wide_to_double(1000000)

#endif
