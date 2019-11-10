/*
 * haxFloats.h --
 *
 *	This header file maps floating point operations into native
 *	toolchain support for floats.
 *
 * Copyright 2019 Kamil Rytarowski
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 */

#ifndef _HAXFLOATS
#define _HAXFLOATS

#if !defined(_HAX) || !defined(_HAXINT)
#error This header shall not be included directly
#endif

#ifdef HAX_SOFTFLOAT
#error SoftFloat IEEE emulation detected
#endif

/*----------------------------------------------------------------------------
| Hax float/double
*----------------------------------------------------------------------------*/
typedef float hax_float;
typedef double hax_double;

/*----------------------------------------------------------------------------
| Integer-to-floating-point conversion routines.
*----------------------------------------------------------------------------*/
static inline hax_float hax_wide_to_float( hax_wide w )
{
    return (hax_float)w;
}
static inline hax_double hax_wide_to_double( hax_wide w )
{
    return (hax_double)w;
}

/*----------------------------------------------------------------------------
| 32-bit (single-precision) floating-point operations.
*----------------------------------------------------------------------------*/
static inline hax_wide hax_float_to_wide( hax_float f )
{
    return (hax_wide)f;
}
static inline hax_double hax_float_to_double( hax_float f )
{
    return (hax_double)f;
}

static inline hax_float hax_float_add( hax_float f1, hax_float f2 )
{
    return f1 + f2;
}
static inline hax_float hax_float_sub( hax_float f1, hax_float f2 )
{
    return f1 - f2;
}
static inline hax_float hax_float_mul( hax_float f1, hax_float f2 )
{
    return f1 * f2;
}
static inline hax_float hax_float_div( hax_float f1, hax_float f2 )
{
    return f1 / f2;
}

static inline int hax_float_eq( hax_float f1, hax_float f2 )
{
    return f1 == f2;
}
static inline int hax_float_le( hax_float f1, hax_float f2 )
{
    return f1 <= f2;
}
static inline int hax_float_lt( hax_float f1, hax_float f2 )
{
    return f1 < f2;
}
static inline int hax_float_neq( hax_float f1, hax_float f2 )
{
    return f1 != f2;
}
static inline int hax_float_ge( hax_float f1, hax_float f2 )
{
    return f1 >= f2;
}
static inline int hax_float_gt( hax_float f1, hax_float f2 )
{
    return f1 > f2;
}

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
static inline hax_wide hax_double_to_wide( hax_double f )
{
    return (hax_wide)f;
}
static inline hax_float hax_double_to_float( hax_double f )
{
    return (hax_float)f;
}

static inline hax_double hax_double_add( hax_double f1, hax_double f2 )
{
    return f1 + f2;
}
static inline hax_double hax_double_sub( hax_double f1, hax_double f2 )
{
    return f1 - f2;
}
static inline hax_double hax_double_mul( hax_double f1, hax_double f2 )
{
    return f1 * f2;
}
static inline hax_double hax_double_div( hax_double f1, hax_double f2 )
{
    return f1 / f2;
}

static inline int hax_double_eq( hax_double f1, hax_double f2 )
{
    return f1 == f2;
}
static inline int hax_double_le( hax_double f1, hax_double f2 )
{
    return f1 <= f2;
}
static inline int hax_double_lt( hax_double f1, hax_double f2 )
{
    return f1 < f2;
}
static inline int hax_double_neq( hax_double f1, hax_double f2 )
{
    return f1 != f2;
}
static inline int hax_double_ge( hax_double f1, hax_double f2 )
{
    return f1 >= f2;
}
static inline int hax_double_gt( hax_double f1, hax_double f2 )
{
    return f1 > f2;
}

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

#define HAX_DOUBLE_ZERO 0.0
#define HAX_DOUBLE_ONE 1.0
#define HAX_DOUBLE_MINUSONE -1.0
#define HAX_DOUBLE_HALF 0.5
#define HAX_DOUBLE_THOUSAND 1e3
#define HAX_DOUBLE_MILLION 1e6

#else
#include <jim-softfloat.h>
#endif

#endif
