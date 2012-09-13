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
    MLAssertEquals(count, N(3), "[1, 2, 3] should have count 3");
}


void MLTestArrayContains() {
    var array = IA(N(1), N(2), N(3));
    MLAssertFalse(MLContains(array, N(0)), "[1, 2, 3] should not contain 0");
    MLAssertTrue(MLContains(array, N(1)), "[1, 2, 3] should contain 1");
    MLAssertTrue(MLContains(array, N(2)), "[1, 2, 3] should contain 2");
    MLAssertTrue(MLContains(array, N(3)), "[1, 2, 3] should contain 3");
    MLAssertFalse(MLContains(array, N(4)), "[1, 2, 3] should not contain 4");
}


void MLTestArrayContainsAny() {
    var array = IA(N(1), N(2), N(3));
    MLAssertFalse(MLContainsAny(array, IA()), "[1, 2, 3] should not contain any of []");
    MLAssertFalse(MLContainsAny(array, IA(N(0), N(4))), "[1, 2, 3] should not contain any of [0, 4]");
    MLAssertTrue(MLContainsAny(array, IA(N(0), N(3))), "[1, 2, 3] should contain any of [0, 3]");
    MLAssertTrue(MLContainsAny(array, IA(N(1), N(3))), "[1, 2, 3] should contain any of [1, 3]");
}


void MLTestArrayContainsAll() {
    var array = IA(N(1), N(2), N(3));
    MLAssertFalse(MLContainsAll(array, IA(N(0), N(4))), "[1, 2, 3] should not contain all of [0, 4]");
    MLAssertFalse(MLContainsAll(array, IA(N(1), N(4))), "[1, 2, 3] should not contain all of [1, 4]");
    MLAssertTrue(MLContainsAll(array, IA(N(1), N(3))), "[1, 2, 3] should contain all of [1, 3]");
    MLAssertTrue(MLContainsAll(array, IA()), "[1, 2, 3] should contain all of []");
}


void MLTestArrayIsEmpty() {
    var arrayWithNoObjects = IA();
    var arrayWithOneObject = IA(N(1));
    var arrayWithManyObjects = IA(N(1), N(2), N(3));
    MLAssertTrue(MLIsEmpty(arrayWithNoObjects), "[] should be empty");
    MLAssertFalse(MLIsEmpty(arrayWithOneObject), "[1] should not be empty");
    MLAssertFalse(MLIsEmpty(arrayWithManyObjects), "[1, 2, 3] should not be empty");
}


void MLTestArrayIsInline() {
    var array = A();
    var inlineArray = IA();
    var mutableArray = MA();
    MLAssertFalse(MLIsInline(array), "A regular array should not be inline");
    MLAssertTrue(MLIsInline(inlineArray), "An inline array should be inline");
    MLAssertFalse(MLIsInline(mutableArray), "A mutable array should not be inline");
}


void MLTestArrayIsMutable() {
    var array = A();
    var inlineArray = IA();
    var mutableArray = MA();
    MLAssertFalse(MLIsMutable(array), "A regular array should not be mutable");
    MLAssertFalse(MLIsMutable(inlineArray), "An inline array should not be mutable");
    MLAssertTrue(MLIsMutable(mutableArray), "A mutable array should be mutable");
}


void MLTestArrayAt() {
    var array = IA(N(1), N(2), N(3));
    MLAssertNull(MLAt(array, N(-1)), "[1, 2, 3] should return null for indexes < 0");
    MLAssertEquals(MLAt(array, N(0)), N(1), "Array [1, 2, 3] should have 1 at index 0");
    MLAssertEquals(MLAt(array, N(1)), N(2), "Array [1, 2, 3] should have 2 at index 1");
    MLAssertEquals(MLAt(array, N(2)), N(3), "Array [1, 2, 3] should have 3 at index 2");
    MLAssertNull(MLAt(array, N(3)), "[1, 2, 3] should return null for indexes >= count");
}


void MLTestArrayAtMany() {
    var array = IA(N(4), N(5), N(6));
    MLAssertEquals(MLAtMany(array, IA()), IA(), "[4, 5, 6] at-many [] should return []");
    MLAssertEquals(MLAtMany(array, IA(N(-1))), IA(), "[4, 5, 6] at-many [-1] (invalid index) should return []");
    MLAssertEquals(MLAtMany(array, IA(N(9))), IA(), "[4, 5, 6] at-many [9] (invalid index) should return []");
    MLAssertEquals(MLAtMany(array, IA(N(1))), IA(N(5)), "[4, 5, 6] at-many [1] should return [5]");
    MLAssertEquals(MLAtMany(array, IA(N(0), N(1), N(2))), array, "[4, 5, 6] at-many [0, 1, 2] should return the same array");
    MLAssertEquals(MLAtMany(array, IA(N(-1), N(0), N(1), N(2))), array, "[4, 5, 6] at-many [-1, 0, 1, 2] (one invalid index) should return the same array");
    MLAssertEquals(MLAtMany(array, IA(N(0), N(1), N(2), N(3))), array, "[4, 5, 6] at-many [0, 1, 2, 3] (one invalid index) should return the same array");
}


void MLTestArrayAtCount() {
    // TODO: implement.
}


void MLTestArray() {
    MLTestArrayCount();
    MLTestArrayContains();
    MLTestArrayContainsAny();
    MLTestArrayContainsAll();
    MLTestArrayIsEmpty();
    MLTestArrayIsInline();
    MLTestArrayIsMutable();
    MLTestArrayAt();
    MLTestArrayAtMany();
    MLTestArrayAtCount();
}
