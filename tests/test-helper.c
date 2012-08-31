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


void MLTestHelperRotateLeft() {
    assert(MLHelperRotateLeft(0x1020304050607080ull, 4) == 0x0203040506070801ull);
    assert(MLHelperRotateLeft(0x1020304050607080ull, 16) == 0x3040506070801020ull);
    assert(MLHelperRotateLeft(0x1020304050607080ull, 32) == 0x5060708010203040ull);
    assert(MLHelperRotateLeft(0x1020304050607080ull, 48) == 0x7080102030405060ull);
    assert(MLHelperRotateLeft(0x1020304050607080ull, 56) == 0x8010203040506070ull);
    assert(MLHelperRotateLeft(0x1020304050607080ull, 60) == 0x0102030405060708ull);
}


void MLTestHelperRotateRight() {
    assert(MLHelperRotateRight(0x1020304050607080ull, 4) == 0x0102030405060708ull);
    assert(MLHelperRotateRight(0x1020304050607080ull, 16) == 0x7080102030405060ull);
    assert(MLHelperRotateRight(0x1020304050607080ull, 32) == 0x5060708010203040ull);
    assert(MLHelperRotateRight(0x1020304050607080ull, 48) == 0x3040506070801020ull);
    assert(MLHelperRotateRight(0x1020304050607080ull, 56) == 0x2030405060708010ull);
    assert(MLHelperRotateRight(0x1020304050607080ull, 60) == 0x0203040506070801ull);
}


void MLTestHelperRoundUpToPowerOfTwo() {
    assert(MLHelperRoundUpToPowerOfTwo(0) == 0);
    assert(MLHelperRoundUpToPowerOfTwo(1) == 1);
    assert(MLHelperRoundUpToPowerOfTwo(2) == 2);

    assert(MLHelperRoundUpToPowerOfTwo(3) == 4);
    assert(MLHelperRoundUpToPowerOfTwo(4) == 4);

    assert(MLHelperRoundUpToPowerOfTwo(5) == 8);
    assert(MLHelperRoundUpToPowerOfTwo(8) == 8);

    assert(MLHelperRoundUpToPowerOfTwo(9) == 16);
    assert(MLHelperRoundUpToPowerOfTwo(16) == 16);

    assert(MLHelperRoundUpToPowerOfTwo(33) == 64);
    assert(MLHelperRoundUpToPowerOfTwo(64) == 64);

    assert(MLHelperRoundUpToPowerOfTwo((1 << 15) - 1) == 1 << 15);
    assert(MLHelperRoundUpToPowerOfTwo(1 << 15) == 1 << 15);

    assert(MLHelperRoundUpToPowerOfTwo((1 << 16) - 1) == 1 << 16);
    assert(MLHelperRoundUpToPowerOfTwo(1 << 16) == 1 << 16);

    assert(MLHelperRoundUpToPowerOfTwo((1 << 30) - 1) == 1 << 30);
    assert(MLHelperRoundUpToPowerOfTwo(1 << 30) == 1 << 30);

    assert(MLHelperRoundUpToPowerOfTwo((1ull << 31) - 1) == 1ull << 31);
    assert(MLHelperRoundUpToPowerOfTwo(1ull << 31) == 1ull << 31);

    assert(MLHelperRoundUpToPowerOfTwo((1ull << 32) - 1) == 1ull << 32);
    assert(MLHelperRoundUpToPowerOfTwo(1ull << 32) == 1ull << 32);

    assert(MLHelperRoundUpToPowerOfTwo((1ull << 62) - 1) == 1ull << 62);
    assert(MLHelperRoundUpToPowerOfTwo(1ull << 62) == 1ull << 62);

    assert(MLHelperRoundUpToPowerOfTwo((1ull << 63) - 1) == 1ull << 63);
    assert(MLHelperRoundUpToPowerOfTwo(1ull << 63) == 1ull << 63);
}


void MLTestHelperRoundDownToPowerOfTwo() {
    assert(MLHelperRoundDownToPowerOfTwo(0) == 0);
    assert(MLHelperRoundDownToPowerOfTwo(1) == 1);

    assert(MLHelperRoundDownToPowerOfTwo(2) == 2);
    assert(MLHelperRoundDownToPowerOfTwo(3) == 2);

    assert(MLHelperRoundDownToPowerOfTwo(4) == 4);
    assert(MLHelperRoundDownToPowerOfTwo(5) == 4);
    assert(MLHelperRoundDownToPowerOfTwo(7) == 4);

    assert(MLHelperRoundDownToPowerOfTwo(8) == 8);
    assert(MLHelperRoundDownToPowerOfTwo(9) == 8);
    assert(MLHelperRoundDownToPowerOfTwo(15) == 8);

    assert(MLHelperRoundDownToPowerOfTwo(16) == 16);
    assert(MLHelperRoundDownToPowerOfTwo(17) == 16);
    assert(MLHelperRoundDownToPowerOfTwo(31) == 16);

    assert(MLHelperRoundDownToPowerOfTwo(32) == 32);
    assert(MLHelperRoundDownToPowerOfTwo(33) == 32);
    assert(MLHelperRoundDownToPowerOfTwo(63) == 32);

    assert(MLHelperRoundDownToPowerOfTwo(64) == 64);
    assert(MLHelperRoundDownToPowerOfTwo(65) == 64);
    assert(MLHelperRoundDownToPowerOfTwo(127) == 64);

    assert(MLHelperRoundDownToPowerOfTwo(1ull << 30) == 1ull << 30);
    assert(MLHelperRoundDownToPowerOfTwo((1ull << 30) + 1) == 1ull << 30);
    assert(MLHelperRoundDownToPowerOfTwo((1ull << 31) - 1) == 1ull << 30);

    assert(MLHelperRoundDownToPowerOfTwo(1ull << 31) == 1ull << 31);
    assert(MLHelperRoundDownToPowerOfTwo((1ull << 31) + 1) == 1ull << 31);
    assert(MLHelperRoundDownToPowerOfTwo((1ull << 32) - 1) == 1ull << 31);

    assert(MLHelperRoundDownToPowerOfTwo(1ull << 32) == 1ull << 32);
    assert(MLHelperRoundDownToPowerOfTwo((1ull << 32) + 1) == 1ull << 32);
    assert(MLHelperRoundDownToPowerOfTwo((1ull << 33) - 1) == 1ull << 32);

    assert(MLHelperRoundDownToPowerOfTwo(1ull << 62) == 1ull << 62);
    assert(MLHelperRoundDownToPowerOfTwo((1ull << 62) + 1) == 1ull << 62);
    assert(MLHelperRoundDownToPowerOfTwo((1ull << 63) - 1) == 1ull << 62);

    assert(MLHelperRoundDownToPowerOfTwo(1ull << 63) == 1ull << 63);
    assert(MLHelperRoundDownToPowerOfTwo((1ull << 63) + 1) == 1ull << 63);
}


void MLTestHelperDigest() {
    const char* find_method_for = "find_method*for*";
    const char* method_not_found_for = "method*not_found_for*";

    const MLNatural find_method_for_count = strlen(find_method_for);
    const MLNatural method_not_found_for_count = strlen(method_not_found_for);

    assert(MLHelperDigest(find_method_for_count, find_method_for) == 0xe922bccb387e52bfull);
    assert(MLHelperDigest(method_not_found_for_count, method_not_found_for) == 0xcd42b3fddaaa6e97ull);

    assert(MLHelperDigest(0, "") == 0x0);
    assert(MLHelperDigest(1, "1") == 0xa5d08d6299c4888);
    assert(MLHelperDigest(2, "12") == 0xed1ee14aa9f597b0);
    assert(MLHelperDigest(3, "123") == 0xb26b00896960cb14);
    assert(MLHelperDigest(4, "1234") == 0x92b7977746710c51);
    assert(MLHelperDigest(5, "12345") == 0x2c468f7605bfecb);
    assert(MLHelperDigest(6, "123456") == 0x99cda13344b54b1f);
    assert(MLHelperDigest(7, "1234567") == 0xae9ebd2095279402);
    assert(MLHelperDigest(8, "12345678") == 0x758f67d162b2d202);
    assert(MLHelperDigest(9, "123456789") == 0x4977490251674330);
}


void MLTestHelper() {
    MLTestHelperRotateLeft();
    MLTestHelperRotateRight();

    MLTestHelperRoundUpToPowerOfTwo();
    MLTestHelperRoundDownToPowerOfTwo();

    MLTestHelperDigest();
}
