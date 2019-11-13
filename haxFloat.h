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
typedef float Float;
typedef double Double;

/*----------------------------------------------------------------------------
| Integer-to-floating-point conversion routines.
*----------------------------------------------------------------------------*/
static inline Float Hax_LongLong_to_float( long long int w )
{
    return (Float)w;
}
static inline Double Hax_LongLong_to_double( long long int w )
{
    return (Double)w;
}

/*----------------------------------------------------------------------------
| 32-bit (single-precision) floating-point operations.
*----------------------------------------------------------------------------*/
static inline long long int Hax_float_to_LongLong( Float f )
{
    return (long long int)f;
}
static inline Double hax_float_to_double( Float f )
{
    return (Double)f;
}

static inline Float hax_float_add( Float f1, Float f2 )
{
    return f1 + f2;
}
static inline Float hax_float_sub( Float f1, Float f2 )
{
    return f1 - f2;
}
static inline Float hax_float_mul( Float f1, Float f2 )
{
    return f1 * f2;
}
static inline Float hax_float_div( Float f1, Float f2 )
{
    return f1 / f2;
}

static inline int hax_float_eq( Float f1, Float f2 )
{
    return f1 == f2;
}
static inline int hax_float_le( Float f1, Float f2 )
{
    return f1 <= f2;
}
static inline int hax_float_lt( Float f1, Float f2 )
{
    return f1 < f2;
}
static inline int hax_float_neq( Float f1, Float f2 )
{
    return f1 != f2;
}
static inline int hax_float_ge( Float f1, Float f2 )
{
    return f1 >= f2;
}
static inline int hax_float_gt( Float f1, Float f2 )
{
    return f1 > f2;
}

hax_float hax_strtof( const char *, char ** );
int hax_stof( hax_float, const char *, char *, unsigned int );

/*----------------------------------------------------------------------------
| 64-bit (double-precision) floating-point operations.
*----------------------------------------------------------------------------*/
static inline long long int hax_double_to_LongLong( Double f )
{
    return (long long int)f;
}
static inline hax_float hax_double_to_float( Double f )
{
    return (hax_float)f;
}

static inline Double hax_double_add( Double f1, Double f2 )
{
    return f1 + f2;
}
static inline Double hax_double_sub( Double f1, Double f2 )
{
    return f1 - f2;
}
static inline Double hax_double_mul( Double f1, Double f2 )
{
    return f1 * f2;
}
static inline Double hax_double_div( Double f1, Double f2 )
{
    return f1 / f2;
}

static inline int hax_double_eq( Double f1, Double f2 )
{
    return f1 == f2;
}
static inline int hax_double_le( Double f1, Double f2 )
{
    return f1 <= f2;
}
static inline int hax_double_lt( Double f1, Double f2 )
{
    return f1 < f2;
}
static inline int hax_double_neq( Double f1, Double f2 )
{
    return f1 != f2;
}
static inline int hax_double_ge( Double f1, Double f2 )
{
    return f1 >= f2;
}
static inline int hax_double_gt( Double f1, Double f2 )
{
    return f1 > f2;
}

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
