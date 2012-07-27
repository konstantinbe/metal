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


void CRTestNumberMake() {
    CRAssert(CRNumber(123.4).payload.decimal == 123.4, "A number should store its value in payload as decimal");
}


// -----------------------------------------------------------------------------


void CRTestNumberHash() {
    var number = CRNumber(123.4);
    CRAssert(CRHash(number) == number.payload.natural, "The hash of a number should be its bitpattern");

    number.payload.natural = 0;
    CRAssert(CRHash(number) == 0, "The hash of a number should be its bitpattern");
}


void CRTestNumberEquals() {
    var number1 = CRNumber(123.4);
    var number2 = CRNumber(123.4);
    var number3 = CRNumber(123.5);
    CRAssert(CREquals(number1, number2), "Two equal numbers should be equal");
    CRAssert(!CREquals(number1, number3), "Two not equal numbers should be not equal");
}


void CRTestNumberCopy() {
    var number = CRNumber(123.4);
    CRAssert(CREquals(number, CRCopy(number)), "A copy of a number should be equal to its original");
}


void CRTestNumberDescription() {
    const var number = CRNumber(123.4);
    const var description = CRDescription(number);
    CRAssert(CREquals(description, CRString("123.4")), "Description for number created with 0x123abcdf '0x123abcdf', but is: '%s'", CRStringCharacters(description));
}


void CRTestNumber() {
    CRTestNumberMake();

    CRTestNumberHash();
    CRTestNumberEquals();
    CRTestNumberCopy();
    CRTestNumberDescription();
}
