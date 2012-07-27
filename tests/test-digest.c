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
#include <string.h>


void CRTestDigest() {
    const CRCharacter* find_method_for = "find_method*for*";
    const CRCharacter* method_not_found_for = "method*not_found_for*";

    const CRNatural find_method_for_length = strlen(find_method_for);
    const CRNatural method_not_found_for_length = strlen(method_not_found_for);

    assert(CRDigest(find_method_for_length, find_method_for) == 0xe922bccb387e52bfull);
    assert(CRDigest(method_not_found_for_length, method_not_found_for) == 0xcd42b3fddaaa6e97ull);

    assert(CRDigest(0, "") == 0x0);
    assert(CRDigest(1, "1") == 0xa5d08d6299c4888);
    assert(CRDigest(2, "12") == 0xed1ee14aa9f597b0);
    assert(CRDigest(3, "123") == 0xb26b00896960cb14);
    assert(CRDigest(4, "1234") == 0x92b7977746710c51);
    assert(CRDigest(5, "12345") == 0x2c468f7605bfecb);
    assert(CRDigest(6, "123456") == 0x99cda13344b54b1f);
    assert(CRDigest(7, "1234567") == 0xae9ebd2095279402);
    assert(CRDigest(8, "12345678") == 0x758f67d162b2d202);
    assert(CRDigest(9, "123456789") == 0x4977490251674330);
}
