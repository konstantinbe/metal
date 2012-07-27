//
// Copyright (c) 2012 Konstantin Bender.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "digest.h"


static const CRNatural64 CRDefaultSeed = 0;


CRNatural64 CRDigest(CRInteger count, const void* bytes) {
    const CRNatural64 seed = CRDefaultSeed;
    const CRNatural64 m = 0xc6a4a7935bd1e995;
    const CRInteger r = 47;

    CRNatural64 h = seed ^ (count * m);

    const CRNatural64* data = (const CRNatural64 *)bytes;
    const CRNatural64* end = data + (count / 8);

    while (data != end) {
        CRNatural64 k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const CRByte* data2 = (const CRByte*)data;

    switch (count & 7 /* count % 8 */) {
        case 7: h ^= (CRNatural64)(data2[6]) << 48;
        case 6: h ^= (CRNatural64)(data2[5]) << 40;
        case 5: h ^= (CRNatural64)(data2[4]) << 32;
        case 4: h ^= (CRNatural64)(data2[3]) << 24;
        case 3: h ^= (CRNatural64)(data2[2]) << 16;
        case 2: h ^= (CRNatural64)(data2[1]) << 8;
        case 1: h ^= (CRNatural64)(data2[0]);
                h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}
