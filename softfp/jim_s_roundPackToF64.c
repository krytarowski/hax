
/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3e, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014, 2015, 2017 The Regents of the University of
California.  All rights reserved.

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

#include "jim.h"   
#include "jim-floats.h"
#include "jim-softfloat-internals.h"

jim_double
 jim_softfloat_roundPackToF64( jim_bool sign, jim_int_fast16_t exp, jim_uint_fast64_t sig )
{
    jim_uint_fast8_t roundingMode;
    jim_bool roundNearEven;
    jim_uint_fast16_t roundIncrement, roundBits;
    jim_bool isTiny;
    jim_uint_fast64_t uiZ;
    union jim_ui64_f64 uZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    roundingMode = jim_softfloat_roundingMode;
    roundNearEven = (roundingMode == jim_softfloat_round_near_even);
    roundIncrement = 0x200;
    if ( ! roundNearEven && (roundingMode != jim_softfloat_round_near_maxMag) ) {
        roundIncrement =
            (roundingMode
                 == (sign ? jim_softfloat_round_min : jim_softfloat_round_max))
                ? 0x3FF
                : 0;
    }
    roundBits = sig & 0x3FF;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( 0x7FD <= (uint16_t) exp ) {
        if ( exp < 0 ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            isTiny =
                (jim_softfloat_detectTininess == jim_softfloat_tininess_beforeRounding)
                    || (exp < -1)
                    || (sig + roundIncrement < JIM_UINT64_C( 0x8000000000000000 ));
            sig = jim_softfloat_shiftRightJam64( sig, -exp );
            exp = 0;
            roundBits = sig & 0x3FF;
            if ( isTiny && roundBits ) {
                jim_softfloat_raiseFlags( jim_softfloat_flag_underflow );
            }
        } else if (
            (0x7FD < exp)
                || (JIM_UINT64_C( 0x8000000000000000 ) <= sig + roundIncrement)
        ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            jim_softfloat_raiseFlags(
                jim_softfloat_flag_overflow | jim_softfloat_flag_inexact );
            uiZ = jim_packToF64UI( sign, 0x7FF, 0 ) - ! roundIncrement;
            goto uiZ;
        }
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sig = (sig + roundIncrement)>>10;
    if ( roundBits ) {
        jim_softfloat_exceptionFlags |= jim_softfloat_flag_inexact;
#ifdef SOFTFLOAT_ROUND_ODD
        if ( roundingMode == jim_softfloat_round_odd ) {
            sig |= 1;
            goto packReturn;
        }
#endif
    }
    sig &= ~(jim_uint_fast64_t) (! (roundBits ^ 0x200) & roundNearEven);
    if ( ! sig ) exp = 0;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 packReturn:
    uiZ = jim_packToF64UI( sign, exp, sig );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}
