
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

#define _HAXSOFTFLOAT_INTERNAL
#include "haxSoftFloat.h"
#include "haxSoftFloatInternals.h"

#ifndef Hax_softfloat_shortShiftRightJamM

void
 Hax_softfloat_shortShiftRightJamM(
     Hax_uint_fast8_t size_words,
     const Hax_uint32_t *aPtr,
     Hax_uint_fast8_t dist,
     Hax_uint32_t *zPtr
 )
{
    Hax_uint_fast8_t uNegDist;
    unsigned int index, lastIndex;
    Hax_uint32_t partWordZ, wordA;

    uNegDist = -dist;
    index = Hax_indexWordLo( size_words );
    lastIndex = Hax_indexWordHi( size_words );
    wordA = aPtr[index];
    partWordZ = wordA>>dist;
    if ( partWordZ<<dist != wordA ) partWordZ |= 1;
    while ( index != lastIndex ) {
        wordA = aPtr[index + Hax_wordIncr];
        zPtr[index] = wordA<<(uNegDist & 31) | partWordZ;
        index += Hax_wordIncr;
        partWordZ = wordA>>dist;
    }
    zPtr[index] = partWordZ;

}

#endif