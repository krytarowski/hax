
/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3e, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014, 2015, 2016 The Regents of the University of
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

#include "jim-softfloat-specialize.h"

jim_float64_t
 jim_softfloat_subMagsF64( jim_uint_fast64_t uiA, jim_uint_fast64_t uiB, jim_bool signZ )
{
    jim_int_fast16_t expA;
    jim_uint_fast64_t sigA;
    jim_int_fast16_t expB;
    jim_uint_fast64_t sigB;
    jim_int_fast16_t expDiff;
    jim_uint_fast64_t uiZ;
    jim_int_fast64_t sigDiff;
    jim_int_fast8_t shiftDist;
    jim_int_fast16_t expZ;
    jim_uint_fast64_t sigZ;
    union jim_ui64_f64 uZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expA = jim_expF64UI( uiA );
    sigA = jim_fracF64UI( uiA );
    expB = jim_expF64UI( uiB );
    sigB = jim_fracF64UI( uiB );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expA - expB;
    if ( ! expDiff ) {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        if ( expA == 0x7FF ) {
            if ( sigA | sigB ) goto propagateNaN;
            jim_softfloat_raiseFlags( jim_softfloat_flag_invalid );
            uiZ = jim_defaultNaNF64UI;
            goto uiZ;
        }
        sigDiff = sigA - sigB;
        if ( ! sigDiff ) {
            uiZ =
                jim_packToF64UI(
                    (jim_softfloat_roundingMode == jim_softfloat_round_min), 0, 0 );
            goto uiZ;
        }
        if ( expA ) --expA;
        if ( sigDiff < 0 ) {
            signZ = ! signZ;
            sigDiff = -sigDiff;
        }
        shiftDist = jim_softfloat_countLeadingZeros64( sigDiff ) - 11;
        expZ = expA - shiftDist;
        if ( expZ < 0 ) {
            shiftDist = expA;
            expZ = 0;
        }
        uiZ = jim_packToF64UI( signZ, expZ, sigDiff<<shiftDist );
        goto uiZ;
    } else {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        sigA <<= 10;
        sigB <<= 10;
        if ( expDiff < 0 ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            signZ = ! signZ;
            if ( expB == 0x7FF ) {
                if ( sigB ) goto propagateNaN;
                uiZ = jim_packToF64UI( signZ, 0x7FF, 0 );
                goto uiZ;
            }
            sigA += expA ? JIM_UINT64_C( 0x4000000000000000 ) : sigA;
            sigA = jim_softfloat_shiftRightJam64( sigA, -expDiff );
            sigB |= JIM_UINT64_C( 0x4000000000000000 );
            expZ = expB;
            sigZ = sigB - sigA;
        } else {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            if ( expA == 0x7FF ) {
                if ( sigA ) goto propagateNaN;
                uiZ = uiA;
                goto uiZ;
            }
            sigB += expB ? JIM_UINT64_C( 0x4000000000000000 ) : sigB;
            sigB = jim_softfloat_shiftRightJam64( sigB, expDiff );
            sigA |= JIM_UINT64_C( 0x4000000000000000 );
            expZ = expA;
            sigZ = sigA - sigB;
        }
        return jim_softfloat_normRoundPackToF64( signZ, expZ - 1, sigZ );
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = jim_softfloat_propagateNaNF64UI( uiA, uiB );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}
