
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


#ifndef JIM_SOFTFLOAT_INTERNALS_H
#define JIM_SOFTFLOAT_INTERNALS_H

#include "jim.h"
#include "jim-softfloat-primitives.h"
#include "jim-softfloat-types.h"

/*----------------------------------------------------------------------------
| Software floating-point underflow tininess-detection mode.
*----------------------------------------------------------------------------*/
extern JIM_THREAD_LOCAL jim_uint_fast8_t jim_softfloat_detectTininess;
enum {
    jim_softfloat_tininess_beforeRounding = 0,
    jim_softfloat_tininess_afterRounding  = 1
};

/*----------------------------------------------------------------------------
| Software floating-point rounding mode.  (Mode "odd" is supported only if
| SoftFloat is compiled with macro 'SOFTFLOAT_ROUND_ODD' defined.)
*----------------------------------------------------------------------------*/
extern JIM_THREAD_LOCAL jim_uint_fast8_t jim_softfloat_roundingMode;
enum {
    jim_softfloat_round_near_even   = 0,
    jim_softfloat_round_minMag      = 1,
    jim_softfloat_round_min         = 2,
    jim_softfloat_round_max         = 3,
    jim_softfloat_round_near_maxMag = 4,
    jim_softfloat_round_odd         = 6
};

/*----------------------------------------------------------------------------
| Software floating-point exception flags.
*----------------------------------------------------------------------------*/
extern JIM_THREAD_LOCAL jim_uint_fast8_t jim_softfloat_exceptionFlags;
enum {
    jim_softfloat_flag_inexact   =  1,
    jim_softfloat_flag_underflow =  2,
    jim_softfloat_flag_overflow  =  4,
    jim_softfloat_flag_infinite  =  8,
    jim_softfloat_flag_invalid   = 16
};

/*----------------------------------------------------------------------------
| Routine to raise any or all of the software floating-point exception flags.
*----------------------------------------------------------------------------*/
void jim_softfloat_raiseFlags( jim_uint_fast8_t );

/*----------------------------------------------------------------------------
| Misc
*----------------------------------------------------------------------------*/

union jim_ui16_f16 { jim_uint16_t ui; jim_float16_t f; };
union jim_ui32_f32 { jim_uint32_t ui; jim_float32_t f; };
union jim_ui64_f64 { jim_uint64_t ui; jim_float64_t f; };
 
#ifdef SOFTFLOAT_FAST_INT64
union extF80M_extF80 { struct extFloat80M fM; extFloat80_t f; };
union ui128_f128 { struct uint128 ui; jim_float128_t f; };
#endif

enum {
    jim_softfloat_mulAdd_subC    = 1,
    jim_softfloat_mulAdd_subProd = 2
};

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
jim_uint_fast32_t jim_softfloat_roundToUI32( jim_bool, uint_fast64_t, jim_uint_fast8_t, jim_bool );

#ifdef SOFTFLOAT_FAST_INT64
uint_fast64_t
 jim_softfloat_roundToUI64(
     jim_bool, jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast8_t, jim_bool );
#else
uint_fast64_t jim_softfloat_roundMToUI64( jim_bool, jim_uint32_t *, jim_uint_fast8_t, jim_bool );
#endif

jim_int_fast32_t jim_softfloat_roundToI32( jim_bool, jim_uint_fast64_t, jim_uint_fast8_t, jim_bool );

#ifdef SOFTFLOAT_FAST_INT64
jim_int_fast64_t
 jim_softfloat_roundToI64(
     jim_bool, jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast8_t, jim_bool );
#else
jim_int_fast64_t jim_softfloat_roundMToI64( jim_bool, jim_uint32_t *, jim_uint_fast8_t, jim_bool );
#endif

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
#define jim_signF16UI( a ) ((jim_bool) ((jim_uint16_t) (a)>>15))
#define jim_expF16UI( a ) ((jim_int_fast8_t) ((a)>>10) & 0x1F)
#define jim_fracF16UI( a ) ((a) & 0x03FF)
#define jim_packToF16UI( sign, exp, sig ) (((jim_uint16_t) (sign)<<15) + ((jim_uint16_t) (exp)<<10) + (sig))

#define jim_isNaNF16UI( a ) (((~(a) & 0x7C00) == 0) && ((a) & 0x03FF))

struct jim_exp8_sig16 { jim_int_fast8_t exp; jim_uint_fast16_t sig; };
struct jim_exp8_sig16 jim_softfloat_normSubnormalF16Sig( jim_uint_fast16_t );

jim_float16_t jim_softfloat_roundPackToF16( jim_bool, jim_int_fast16_t, jim_uint_fast16_t );
jim_float16_t jim_softfloat_normRoundPackToF16( jim_bool, jim_int_fast16_t, jim_uint_fast16_t );

jim_float16_t jim_softfloat_addMagsF16( jim_uint_fast16_t, jim_uint_fast16_t );
jim_float16_t jim_softfloat_subMagsF16( jim_uint_fast16_t, jim_uint_fast16_t );
jim_float16_t
 jim_softfloat_mulAddF16(
     jim_uint_fast16_t, jim_uint_fast16_t, jim_uint_fast16_t, jim_uint_fast8_t );

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
#define jim_signF32UI( a ) ((jim_bool) ((jim_uint32_t) (a)>>31))
#define jim_expF32UI( a ) ((jim_int_fast16_t) ((a)>>23) & 0xFF)
#define jim_fracF32UI( a ) ((a) & 0x007FFFFF)
#define jim_packToF32UI( sign, exp, sig ) (((jim_uint32_t) (sign)<<31) + ((jim_uint32_t) (exp)<<23) + (sig))

#define jim_isNaNF32UI( a ) (((~(a) & 0x7F800000) == 0) && ((a) & 0x007FFFFF))

struct jim_exp16_sig32 { jim_int_fast16_t exp; jim_uint_fast32_t sig; };
struct jim_exp16_sig32 jim_softfloat_normSubnormalF32Sig( jim_uint_fast32_t );

jim_float32_t jim_softfloat_roundPackToF32( jim_bool, jim_int_fast16_t, jim_uint_fast32_t );
jim_float32_t jim_softfloat_normRoundPackToF32( jim_bool, jim_int_fast16_t, jim_uint_fast32_t );

jim_float32_t jim_softfloat_addMagsF32( jim_uint_fast32_t, jim_uint_fast32_t );
jim_float32_t jim_softfloat_subMagsF32( jim_uint_fast32_t, jim_uint_fast32_t );
jim_float32_t
 jim_softfloat_mulAddF32(
     jim_uint_fast32_t, jim_uint_fast32_t, jim_uint_fast32_t, jim_uint_fast8_t );

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
#define jim_signF64UI( a ) ((jim_bool) ((jim_uint64_t) (a)>>63))
#define jim_expF64UI( a ) ((jim_int_fast16_t) ((a)>>52) & 0x7FF)
#define jim_fracF64UI( a ) ((a) & UINT64_C( 0x000FFFFFFFFFFFFF ))
#define jim_packToF64UI( sign, exp, sig ) ((jim_uint64_t) (((jim_uint_fast64_t) (sign)<<63) + ((jim_uint_fast64_t) (exp)<<52) + (sig)))

#define jim_isNaNF64UI( a ) (((~(a) & JIM_UINT64_C( 0x7FF0000000000000 )) == 0) && ((a) & JIM_UINT64_C( 0x000FFFFFFFFFFFFF )))

struct jim_exp16_sig64 { jim_int_fast16_t exp; jim_uint_fast64_t sig; };
struct jim_exp16_sig64 jim_softfloat_normSubnormalF64Sig( jim_uint_fast64_t );

jim_float64_t jim_softfloat_roundPackToF64( jim_bool, jim_int_fast16_t, jim_uint_fast64_t );
jim_float64_t jim_softfloat_normRoundPackToF64( jim_bool, jim_int_fast16_t, jim_uint_fast64_t );

jim_float64_t jim_softfloat_addMagsF64( jim_uint_fast64_t, jim_uint_fast64_t, jim_bool );
jim_float64_t jim_softfloat_subMagsF64( jim_uint_fast64_t, jim_uint_fast64_t, jim_bool );
jim_float64_t
 jim_softfloat_mulAddF64(
     jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast8_t );

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
#define jim_signExtF80UI64( a64 ) ((jim_bool) ((jim_uint16_t) (a64)>>15))
#define jim_expExtF80UI64( a64 ) ((a64) & 0x7FFF)
#define jim_packToExtF80UI64( sign, exp ) ((jim_uint_fast16_t) (sign)<<15 | (exp))

#define jim_isNaNExtF80UI( a64, a0 ) ((((a64) & 0x7FFF) == 0x7FFF) && ((a0) & JIM_UINT64_C( 0x7FFFFFFFFFFFFFFF )))

#ifdef SOFTFLOAT_FAST_INT64

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/

struct jim_exp32_sig64 { jim_int_fast32_t exp; jim_uint64_t sig; };
struct jim_exp32_sig64 jim_softfloat_normSubnormalExtF80Sig( jim_uint_fast64_t );

jim_extFloat80_t
 jim_softfloat_roundPackToExtF80(
     jim_bool, int_fast32_t, jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast8_t );
jim_extFloat80_t
 jim_softfloat_normRoundPackToExtF80(
     jim_bool, int_fast32_t, jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast8_t );

jim_extFloat80_t
 jim_softfloat_addMagsExtF80(
     jim_uint_fast16_t, jim_uint_fast64_t, uint_fast16_t, jim_uint_fast64_t, jim_bool );
jim_extFloat80_t
 jim_softfloat_subMagsExtF80(
     jim_uint_fast16_t, jim_uint_fast64_t, uint_fast16_t, jim_uint_fast64_t, jim_bool );

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
#define jim_signF128UI64( a64 ) ((jim_bool) ((jim_uint64_t) (a64)>>63))
#define jim_expF128UI64( a64 ) ((jim_int_fast32_t) ((a64)>>48) & 0x7FFF)
#define jim_fracF128UI64( a64 ) ((a64) & UINT64_C( 0x0000FFFFFFFFFFFF ))
#define jim_packToF128UI64( sign, exp, sig64 ) (((jim_uint_fast64_t) (sign)<<63) + ((jim_uint_fast64_t) (exp)<<48) + (sig64))

#define jim_isNaNF128UI( a64, a0 ) (((~(a64) & JIM_UINT64_C( 0x7FFF000000000000 )) == 0) && (a0 || ((a64) & JIM_UINT64_C( 0x0000FFFFFFFFFFFF ))))

struct exp32_sig128 { jim_int_fast32_t exp; struct jim_uint128 sig; };
struct exp32_sig128
 softfloat_normSubnormalF128Sig( jim_uint_fast64_t, jim_uint_fast64_t );

jim_float128_t
 jim_softfloat_roundPackToF128(
     jim_bool, jim_int_fast32_t, jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast64_t );
jim_float128_t
 jim_softfloat_normRoundPackToF128(
     jim_bool, jim_int_fast32_t, jim_uint_fast64_t, jim_uint_fast64_t );

jim_float128_t
 softfloat_addMagsF128(
     jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast64_t, jim_bool );
jim_float128_t
 softfloat_subMagsF128(
     jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast64_t, jim_uint_fast64_t, jim_bool );
jim_float128_t
 softfloat_mulAddF128(
     jim_uint_fast64_t,
     jim_uint_fast64_t,
     jim_uint_fast64_t,
     jim_uint_fast64_t,
     jim_uint_fast64_t,
     jim_uint_fast64_t,
     jim_uint_fast8_t
 );

#else

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/

jim_bool
 jim_softfloat_tryPropagateNaNExtF80M(
     const struct jim_extFloat80M *,
     const struct jim_extFloat80M *,
     struct jim_extFloat80M *
 );
void jim_softfloat_invalidExtF80M( struct jim_extFloat80M * );

int jim_softfloat_normExtF80SigM( jim_uint64_t * );

void
 jim_softfloat_roundPackMToExtF80M(
     jim_bool, jim_int32_t, jim_uint32_t *, jim_uint_fast8_t, struct jim_extFloat80M * );
void
 jim_softfloat_normRoundPackMToExtF80M(
     jim_bool, jim_int32_t, jim_uint32_t *, jim_uint_fast8_t, struct jim_extFloat80M * );

void
 jim_softfloat_addExtF80M(
     const struct jim_extFloat80M *,
     const struct jim_extFloat80M *,
     struct jim_extFloat80M *,
     jim_bool
 );

int
 jim_softfloat_compareNonnormExtF80M(
     const struct jim_extFloat80M *, const struct jim_extFloat80M * );

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
#define jim_signF128UI96( a96 ) ((jim_bool) ((jim_uint32_t) (a96)>>31))
#define jim_expF128UI96( a96 ) ((jim_int32_t) ((a96)>>16) & 0x7FFF)
#define jim_fracF128UI96( a96 ) ((a96) & 0x0000FFFF)
#define jim_packToF128UI96( sign, exp, sig96 ) (((jim_uint32_t) (sign)<<31) + ((jim_uint32_t) (exp)<<16) + (sig96))

jim_bool jim_softfloat_isNaNF128M( const jim_uint32_t * );

jim_bool
 jim_softfloat_tryPropagateNaNF128M(
     const jim_uint32_t *, const jim_uint32_t *, jim_uint32_t * );
void jim_softfloat_invalidF128M( jim_uint32_t * );

int jim_softfloat_shiftNormSigF128M( const jim_uint32_t *, jim_uint_fast8_t, jim_uint32_t * );

void jim_softfloat_roundPackMToF128M( jim_bool, int32_t, jim_uint32_t *, jim_uint32_t * );
void jim_softfloat_normRoundPackMToF128M( jim_bool, int32_t, jim_uint32_t *, jim_uint32_t * );

void
 jim_softfloat_addF128M( const jim_uint32_t *, const jim_uint32_t *, jim_uint32_t *, jim_bool );
void
 jim_softfloat_mulAddF128M(
     const jim_uint32_t *,
     const jim_uint32_t *,
     const jim_uint32_t *,
     jim_uint32_t *,
     jim_uint_fast8_t
 );

#endif

#endif
