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


void CRTestWordMake() {
    CRAssert(CRWord(0x123).payload.natural == 0x123, "A word should store its value in payload as natural");
}


// -----------------------------------------------------------------------------


void CRTestWordHash() {
    var word = CRWord(0x1234);
    CRAssert(CRHash(word) == 0x1234, "The hash of a word should be its bitpattern");

    word.payload.natural = 0;
    CRAssert(CRHash(word) == 0, "The hash of a word should be its bitpattern");
}


void CRTestWordEquals() {
    var word1 = CRWord(0x1234);
    var word2 = CRWord(0x1234);
    var word3 = CRWord(0x123A);
    CRAssert(CREquals(word1, word2), "Two equal words should be equal");
    CRAssert(!CREquals(word1, word3), "Two not equal words should be not equal");
}


void CRTestWordCopy() {
    var word = CRWord(0x123);
    CRAssert(CREquals(word, CRCopy(word)), "A copy of a word should be equal to its original");
}


void CRTestWordDescription() {
    const var word = CRWord(0x123abcdf);
    const var description = CRDescription(word);
    CRAssert(CREquals(description, CRString("0x123abcdf")), "Description for word created with 0x123abcdf '0x123abcdf', but is: '%s'", CRStringCharacters(description));
}


void CRTestWord() {
    CRTestWordMake();

    CRTestWordHash();
    CRTestWordEquals();
    CRTestWordCopy();
    CRTestWordDescription();
}
