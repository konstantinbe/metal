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


void MLTestArrayCount() {
    var array = IA(N(1), N(2), N(3));
    var count = MLCount(array);
    MLExpectToEqual(count, N(3), "[1, 2, 3] should have count 3");
}


void MLTestArrayContains() {
    var array = IA(N(1), N(2), N(3));
    MLExpectNotTo(MLContains(array, N(0)), "[1, 2, 3] should not contain 0");
    MLExpectTo(MLContains(array, N(1)), "[1, 2, 3] should contain 1");
    MLExpectTo(MLContains(array, N(2)), "[1, 2, 3] should contain 2");
    MLExpectTo(MLContains(array, N(3)), "[1, 2, 3] should contain 3");
    MLExpectNotTo(MLContains(array, N(4)), "[1, 2, 3] should not contain 4");
}


void MLTestArrayContainsAny() {
    var array = IA(N(1), N(2), N(3));
    MLExpectNotTo(MLContainsAny(array, IA()), "[1, 2, 3] should not contain any of []");
    MLExpectNotTo(MLContainsAny(array, IA(N(0), N(4))), "[1, 2, 3] should not contain any of [0, 4]");
    MLExpectTo(MLContainsAny(array, IA(N(0), N(3))), "[1, 2, 3] should contain any of [0, 3]");
    MLExpectTo(MLContainsAny(array, IA(N(1), N(3))), "[1, 2, 3] should contain any of [1, 3]");
}


void MLTestArrayContainsAll() {
    var array = IA(N(1), N(2), N(3));
    MLExpectNotTo(MLContainsAll(array, IA(N(0), N(4))), "[1, 2, 3] should not contain all of [0, 4]");
    MLExpectNotTo(MLContainsAll(array, IA(N(1), N(4))), "[1, 2, 3] should not contain all of [1, 4]");
    MLExpectTo(MLContainsAll(array, IA(N(1), N(3))), "[1, 2, 3] should contain all of [1, 3]");
    MLExpectTo(MLContainsAll(array, IA()), "[1, 2, 3] should contain all of []");
}


void MLTestArrayIsEmpty() {
    var arrayWithNoObjects = IA();
    var arrayWithOneObject = IA(N(1));
    var arrayWithManyObjects = IA(N(1), N(2), N(3));
    MLExpectTo(MLIsEmpty(arrayWithNoObjects), "[] should be empty");
    MLExpectNotTo(MLIsEmpty(arrayWithOneObject), "[1] should not be empty");
    MLExpectNotTo(MLIsEmpty(arrayWithManyObjects), "[1, 2, 3] should not be empty");
}


void MLTestArrayIsInline() {
    // TODO: implement.
}


void MLTestArrayIsMutable() {
    // TODO: implement.
}


void MLTestArray() {
    printf("\nTesting MLArray ...\n");

    MLTestArrayCount();
    MLTestArrayContains();
    MLTestArrayContainsAny();
    MLTestArrayContainsAll();
    MLTestArrayIsEmpty();
    MLTestArrayIsInline();
    MLTestArrayIsMutable();

}
