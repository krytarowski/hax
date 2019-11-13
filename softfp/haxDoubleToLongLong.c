
/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
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

#include "jim.h"
#include "jim-floats.h"
#include "jim-softfloat-internals.h"

#include "jim-softfloat-specialize.h"

jim_wide jim_double_to_wide( jim_double a )
{
    const jim_uint_fast8_t roundingMode = jim_softfloat_round_minMag;
    const jim_bool exact = 0;

    union jim_ui64_f64 uA;
    jim_uint_fast64_t uiA;
    jim_bool sign;
    jim_int_fast16_t exp;
    jim_uint_fast64_t sig;
    jim_int_fast16_t shiftDist;
#ifdef SOFTFLOAT_FAST_INT64
    struct jim_uint64_extra sigExtra;
#else
    jim_uint32_t extSig[3];
#endif

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uA.f = a;
    uiA = uA.ui;
    sign = jim_signF64UI( uiA );
    exp  = jim_expF64UI( uiA );
    sig  = jim_fracF64UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( exp ) sig |= JIM_UINT64_C( 0x0010000000000000 );
    shiftDist = 0x433 - exp;
#ifdef SOFTFLOAT_FAST_INT64
    if ( shiftDist <= 0 ) {
        if ( shiftDist < -11 ) goto invalid;
        sigExtra.v = sig<<-shiftDist;
        sigExtra.extra = 0;
    } else {
        sigExtra = jim_softfloat_shiftRightJam64Extra( sig, 0, shiftDist );
    }
    return
        jim_softfloat_roundToI64(
            sign, sigExtra.v, sigExtra.extra, roundingMode, exact );
#else
    extSig[jim_indexWord( 3, 0 )] = 0;
    if ( shiftDist <= 0 ) {
        if ( shiftDist < -11 ) goto invalid;
        sig <<= -shiftDist;
        extSig[jim_indexWord( 3, 2 )] = sig>>32;
        extSig[jim_indexWord( 3, 1 )] = sig;
    } else {
        extSig[jim_indexWord( 3, 2 )] = sig>>32;
        extSig[jim_indexWord( 3, 1 )] = sig;
        jim_softfloat_shiftRightJam96M( extSig, shiftDist, extSig );
    }
    return jim_softfloat_roundMToI64( sign, extSig, roundingMode, exact );
#endif
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 invalid:
    jim_softfloat_raiseFlags( jim_softfloat_flag_invalid );
    return
        (exp == 0x7FF) && jim_fracF64UI( uiA ) ? jim_i64_fromNaN
            : sign ? jim_i64_fromNegOverflow : jim_i64_fromPosOverflow;

}
