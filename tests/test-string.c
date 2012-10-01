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


void MLTestStringInit() {
    var string = MLInit(MLCreate(MLString));
    MLAssertEquals(MLCount(string), N(0), "Count of a newly initialized string should be 0");
    MLAssertEquals(string, IS(""), "A newly initialized string should equal an empty string");
    MLAssertNotEquals(string, IS(" "), "A newly initialized string should not equal any non-empty string");
    MLAssertFalse(MLIsMutable(string), "A newly initialized string should not be mutable");
    MLRelease(string);
}


void MLTestStringInitWithString() {
    var string = IS("test");
    var new = MLInitWithString(MLCreate(MLString), string);
    MLAssertEquals(string, string, "A newly string initialized with string 'test' should that string");
    MLRelease(new);
}


void MLTestString() {
    MLTestStringInit();
    MLTestStringInitWithString();
}
