
/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3e, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014 The Regents of the University of California.
All rights reserved.

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

jim_double jim_double_div( jim_double a, jim_double b )
{
    union jim_ui64_f64 uA;
    jim_uint_fast64_t uiA;
    jim_bool signA;
    jim_int_fast16_t expA;
    jim_uint_fast64_t sigA;
    union jim_ui64_f64 uB;
    jim_uint_fast64_t uiB;
    jim_bool signB;
    jim_int_fast16_t expB;
    jim_uint_fast64_t sigB;
    jim_bool signZ;
    struct jim_exp16_sig64 normExpSig;
    jim_int_fast16_t expZ;
    jim_uint32_t recip32, sig32Z, doubleTerm;
    jim_uint_fast64_t rem;
    jim_uint32_t q;
    jim_uint_fast64_t sigZ;
    jim_uint_fast64_t uiZ;
    union jim_ui64_f64 uZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uA.f = a;
    uiA = uA.ui;
    signA = jim_signF64UI( uiA );
    expA  = jim_expF64UI( uiA );
    sigA  = jim_fracF64UI( uiA );
    uB.f = b;
    uiB = uB.ui;
    signB = jim_signF64UI( uiB );
    expB  = jim_expF64UI( uiB );
    sigB  = jim_fracF64UI( uiB );
    signZ = signA ^ signB;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( expA == 0x7FF ) {
        if ( sigA ) goto propagateNaN;
        if ( expB == 0x7FF ) {
            if ( sigB ) goto propagateNaN;
            goto invalid;
        }
        goto infinity;
    }
    if ( expB == 0x7FF ) {
        if ( sigB ) goto propagateNaN;
        goto zero;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( ! expB ) {
        if ( ! sigB ) {
            if ( ! (expA | sigA) ) goto invalid;
            jim_softfloat_raiseFlags( jim_softfloat_flag_infinite );
            goto infinity;
        }
        normExpSig = jim_softfloat_normSubnormalF64Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    if ( ! expA ) {
        if ( ! sigA ) goto zero;
        normExpSig = jim_softfloat_normSubnormalF64Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expZ = expA - expB + 0x3FE;
    sigA |= JIM_UINT64_C( 0x0010000000000000 );
    sigB |= JIM_UINT64_C( 0x0010000000000000 );
    if ( sigA < sigB ) {
        --expZ;
        sigA <<= 11;
    } else {
        sigA <<= 10;
    }
    sigB <<= 11;
    recip32 = jim_softfloat_approxRecip32_1( sigB>>32 ) - 2;
    sig32Z = ((jim_uint32_t) (sigA>>32) * (jim_uint_fast64_t) recip32)>>32;
    doubleTerm = sig32Z<<1;
    rem =
        ((sigA - (jim_uint_fast64_t) doubleTerm * (jim_uint32_t) (sigB>>32))<<28)
            - (jim_uint_fast64_t) doubleTerm * ((jim_uint32_t) sigB>>4);
    q = (((jim_uint32_t) (rem>>32) * (jim_uint_fast64_t) recip32)>>32) + 4;
    sigZ = ((jim_uint_fast64_t) sig32Z<<32) + ((jim_uint_fast64_t) q<<4);
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( (sigZ & 0x1FF) < 4<<4 ) {
        q &= ~7;
        sigZ &= ~(jim_uint_fast64_t) 0x7F;
        doubleTerm = q<<1;
        rem =
            ((rem - (jim_uint_fast64_t) doubleTerm * (jim_uint32_t) (sigB>>32))<<28)
                - (jim_uint_fast64_t) doubleTerm * ((jim_uint32_t) sigB>>4);
        if ( rem & JIM_UINT64_C( 0x8000000000000000 ) ) {
            sigZ -= 1<<7;
        } else {
            if ( rem ) sigZ |= 1;
        }
    }
    return jim_softfloat_roundPackToF64( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = jim_softfloat_propagateNaNF64UI( uiA, uiB );
    goto uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 invalid:
    jim_softfloat_raiseFlags( jim_softfloat_flag_invalid );
    uiZ = jim_defaultNaNF64UI;
    goto uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 infinity:
    uiZ = jim_packToF64UI( signZ, 0x7FF, 0 );
    goto uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 zero:
    uiZ = jim_packToF64UI( signZ, 0, 0 );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}
