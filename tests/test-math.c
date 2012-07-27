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

#include "test.h"


void CRTestRotateLeft() {
    assert(CRRotateLeft(0x1020304050607080ull, 4) == 0x0203040506070801ull);
    assert(CRRotateLeft(0x1020304050607080ull, 16) == 0x3040506070801020ull);
    assert(CRRotateLeft(0x1020304050607080ull, 32) == 0x5060708010203040ull);
    assert(CRRotateLeft(0x1020304050607080ull, 48) == 0x7080102030405060ull);
    assert(CRRotateLeft(0x1020304050607080ull, 56) == 0x8010203040506070ull);
    assert(CRRotateLeft(0x1020304050607080ull, 60) == 0x0102030405060708ull);
}


void CRTestRotateRight() {
    assert(CRRotateRight(0x1020304050607080ull, 4) == 0x0102030405060708ull);
    assert(CRRotateRight(0x1020304050607080ull, 16) == 0x7080102030405060ull);
    assert(CRRotateRight(0x1020304050607080ull, 32) == 0x5060708010203040ull);
    assert(CRRotateRight(0x1020304050607080ull, 48) == 0x3040506070801020ull);
    assert(CRRotateRight(0x1020304050607080ull, 56) == 0x2030405060708010ull);
    assert(CRRotateRight(0x1020304050607080ull, 60) == 0x0203040506070801ull);
}


void CRTestRoundUpToPowerOfTwo() {
    assert(CRRoundUpToPowerOfTwo(0) == 0);
    assert(CRRoundUpToPowerOfTwo(1) == 1);
    assert(CRRoundUpToPowerOfTwo(2) == 2);

    assert(CRRoundUpToPowerOfTwo(3) == 4);
    assert(CRRoundUpToPowerOfTwo(4) == 4);

    assert(CRRoundUpToPowerOfTwo(5) == 8);
    assert(CRRoundUpToPowerOfTwo(8) == 8);

    assert(CRRoundUpToPowerOfTwo(9) == 16);
    assert(CRRoundUpToPowerOfTwo(16) == 16);

    assert(CRRoundUpToPowerOfTwo(33) == 64);
    assert(CRRoundUpToPowerOfTwo(64) == 64);

    assert(CRRoundUpToPowerOfTwo((1 << 15) - 1) == 1 << 15);
    assert(CRRoundUpToPowerOfTwo(1 << 15) == 1 << 15);

    assert(CRRoundUpToPowerOfTwo((1 << 16) - 1) == 1 << 16);
    assert(CRRoundUpToPowerOfTwo(1 << 16) == 1 << 16);

    assert(CRRoundUpToPowerOfTwo((1 << 30) - 1) == 1 << 30);
    assert(CRRoundUpToPowerOfTwo(1 << 30) == 1 << 30);

    assert(CRRoundUpToPowerOfTwo((1ull << 31) - 1) == 1ull << 31);
    assert(CRRoundUpToPowerOfTwo(1ull << 31) == 1ull << 31);

    assert(CRRoundUpToPowerOfTwo((1ull << 32) - 1) == 1ull << 32);
    assert(CRRoundUpToPowerOfTwo(1ull << 32) == 1ull << 32);

    assert(CRRoundUpToPowerOfTwo((1ull << 62) - 1) == 1ull << 62);
    assert(CRRoundUpToPowerOfTwo(1ull << 62) == 1ull << 62);

    assert(CRRoundUpToPowerOfTwo((1ull << 63) - 1) == 1ull << 63);
    assert(CRRoundUpToPowerOfTwo(1ull << 63) == 1ull << 63);
}


void CRTestRoundDownToPowerOfTwo() {
    assert(CRRoundDownToPowerOfTwo(0) == 0);
    assert(CRRoundDownToPowerOfTwo(1) == 1);

    assert(CRRoundDownToPowerOfTwo(2) == 2);
    assert(CRRoundDownToPowerOfTwo(3) == 2);

    assert(CRRoundDownToPowerOfTwo(4) == 4);
    assert(CRRoundDownToPowerOfTwo(5) == 4);
    assert(CRRoundDownToPowerOfTwo(7) == 4);

    assert(CRRoundDownToPowerOfTwo(8) == 8);
    assert(CRRoundDownToPowerOfTwo(9) == 8);
    assert(CRRoundDownToPowerOfTwo(15) == 8);

    assert(CRRoundDownToPowerOfTwo(16) == 16);
    assert(CRRoundDownToPowerOfTwo(17) == 16);
    assert(CRRoundDownToPowerOfTwo(31) == 16);

    assert(CRRoundDownToPowerOfTwo(32) == 32);
    assert(CRRoundDownToPowerOfTwo(33) == 32);
    assert(CRRoundDownToPowerOfTwo(63) == 32);

    assert(CRRoundDownToPowerOfTwo(64) == 64);
    assert(CRRoundDownToPowerOfTwo(65) == 64);
    assert(CRRoundDownToPowerOfTwo(127) == 64);

    assert(CRRoundDownToPowerOfTwo(1ull << 30) == 1ull << 30);
    assert(CRRoundDownToPowerOfTwo((1ull << 30) + 1) == 1ull << 30);
    assert(CRRoundDownToPowerOfTwo((1ull << 31) - 1) == 1ull << 30);

    assert(CRRoundDownToPowerOfTwo(1ull << 31) == 1ull << 31);
    assert(CRRoundDownToPowerOfTwo((1ull << 31) + 1) == 1ull << 31);
    assert(CRRoundDownToPowerOfTwo((1ull << 32) - 1) == 1ull << 31);

    assert(CRRoundDownToPowerOfTwo(1ull << 32) == 1ull << 32);
    assert(CRRoundDownToPowerOfTwo((1ull << 32) + 1) == 1ull << 32);
    assert(CRRoundDownToPowerOfTwo((1ull << 33) - 1) == 1ull << 32);

    assert(CRRoundDownToPowerOfTwo(1ull << 62) == 1ull << 62);
    assert(CRRoundDownToPowerOfTwo((1ull << 62) + 1) == 1ull << 62);
    assert(CRRoundDownToPowerOfTwo((1ull << 63) - 1) == 1ull << 62);

    assert(CRRoundDownToPowerOfTwo(1ull << 63) == 1ull << 63);
    assert(CRRoundDownToPowerOfTwo((1ull << 63) + 1) == 1ull << 63);
}


void CRTestMath() {
    CRTestRotateLeft();
    CRTestRotateRight();

    CRTestRoundUpToPowerOfTwo();
    CRTestRoundDownToPowerOfTwo();
}
