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

/*
 * Hax float/double
 */
typedef float Float;
typedef double Double;

/*
 * 32-bit (single-precision) floating-point operations.
 */

static inline Double
Hax_FloatToDouble(Float f)
{
    return (Double)f;
}

Float hax_strtof( const char *, char ** );
int hax_stof( hax_float, const char *, char *, unsigned int );

/*
 * 64-bit (double-precision) floating-point operations.
 */

static inline long long int
Hax_DoubleToLongLong(Double f)
{
    return (long long int)f;
}

static inline Float
Hax_DoubleToFloat(Double f)
{
    return (Float)f;
}

static inline Double
Hax_DoubleAdd(Double f1, Double f2)
{
    return f1 + f2;
}

static inline Double
Hax_DoubleSub(Double f1, Double f2)
{
    return f1 - f2;
}

static inline Double
Hax_DoubleMul(Double f1, Double f2)
{
    return f1 * f2;
}

static inline Double
Hax_DoubleDiv(Double f1, Double f2)
{
    return f1 / f2;
}

static inline int
Hax_doubleEq(Double f1, Double f2)
{
    return f1 == f2;
}

static inline int
Hax_doubleLe(Double f1, Double f2)
{
    return f1 <= f2;
}

static inline int
Hax_doubleLt(Double f1, Double f2)
{
    return f1 < f2;
}

static inline int
Hax_doubleNeq(Double f1, Double f2)
{
    return f1 != f2;
}

static inline int
Hax_doubleGe(Double f1, Double f2)
{
    return f1 >= f2;
}

static inline int
Hax_DoubleGt(Double f1, Double f2)
{
    return f1 > f2;
}

Double hax_strtod(const char *, char **);
int hax_stod(Double, const char *, char *, unsigned int);

/*
 * Hax soft float/double commonly used constants
 */

#define HAX_DOUBLE_ZERO 0.0
#define HAX_DOUBLE_ONE 1.0
#define HAX_DOUBLE_MINUSONE -1.0
#define HAX_DOUBLE_HALF 0.5
#define HAX_DOUBLE_THOUSAND 1e3
#define HAX_DOUBLE_MILLION 1e6

#endif /* _HAXFLOAT */
