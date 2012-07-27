//
// Copyright (c) 2010 Konstantin Bender.
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


static void CRTestIsInstanceOf() {
    CRAssert(CRIsInstanceOf(null, CRObject) == false, "null should not be an instance of CRObject");
    CRAssert(CRIsInstanceOf(CRTrue, CRBoolean), "CRTrue should be an instance of CRBoolean");
    CRAssert(CRIsInstanceOf(CRNumber(-123.4), CRNumber), "-123.4 should be an instance of CRNumber");
    CRAssert(CRIsInstanceOf(CRWord(0xFF), CRWord), "0xFF should be an instance of CRWord");

    CRAssert(CRIsInstanceOf(CRString("Hello World!"), CRString), "'Hello World' should be an instance of CRString");
    CRAssert(CRIsInstanceOf(CRArray(), CRArray), "[] should be an instance of CRArray");
    CRAssert(CRIsInstanceOf(CRDictionary(), CRDictionary), "{} should be an instance of CRDictionary");
}


void CRTestCore() {
    CRTestIsInstanceOf();
}
