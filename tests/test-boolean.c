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


void CRTestBooleanMake() {
    CRAssert(CRBooleanMake(true).payload.boolean == true, "Makes true from native C constant 'true'");
    CRAssert(CRBooleanMake(false).payload.boolean == false, "Makes false from native C constant 'false'");
    CRAssert(CRBooleanMake(123).payload.boolean == true, "Makes true from native C number literal '123'");
    CRAssert(CRBooleanMake(0).payload.boolean == false, "Makes true from native C number literal '0'");
}


void CRTestBooleanHash() {
    CRAssert(CRHash(CRBoolean(true)) == 1, "Hash of true should be 1");
    CRAssert(CRHash(CRBoolean(false)) == 0, "Hash of false should be 0");
}


void CRTestBooleanEquals() {
    CRAssert(CREquals(CRBoolean(true), CRBoolean(true)), "true should equal true");
    CRAssert(CREquals(CRBoolean(false), CRBoolean(false)), "false should equal false");
    CRAssert(!CREquals(CRBoolean(false), CRBoolean(true)), "false should not equal true");
    CRAssert(!CREquals(CRBoolean(true), CRBoolean(false)), "true should not equal false");
}


void CRTestBooleanCopy() {
    CRAssert(CRCopy(CRBoolean(true)).payload.boolean == true, "Copying true should return true");
    CRAssert(CRCopy(CRBoolean(false)).payload.boolean == false, "Copying false should return false");
}


void CRTestBooleanDescription() {
    CRAssert(CREquals(CRDescription(CRTrue), CRString("true")), "Description of true should be 'true'");
    CRAssert(CREquals(CRDescription(CRFalse), CRString("false")), "Description of false should be 'false'");
}


void CRTestBoolean() {
    CRTestBooleanMake();

    CRTestBooleanHash();
    CRTestBooleanEquals();
    CRTestBooleanCopy();
    CRTestBooleanDescription();
}
