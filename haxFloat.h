/*
 * haxFloat.h --
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

#ifndef _HAXFLOAT
#define _HAXFLOAT

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
static inline hax_float hax_longlong_to_float( long long int w )
{
    return (hax_float)w;
}
static inline hax_double hax_longlong_to_double( long long int w )
{
    return (hax_double)w;
}

/*----------------------------------------------------------------------------
| 32-bit (single-precision) floating-point operations.
*----------------------------------------------------------------------------*/
static inline long long int hax_float_to_longlong( hax_float f )
{
    return (long long int)f;
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

hax_float hax_strtof( const char *, char ** );
int hax_stof( hax_float, const char *, char *, unsigned int );

/*----------------------------------------------------------------------------
| 64-bit (double-precision) floating-point operations.
*----------------------------------------------------------------------------*/
static inline long long int hax_double_to_longlong( hax_double f )
{
    return (long long int)f;
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

#endif /* _HAXFLOAT */
