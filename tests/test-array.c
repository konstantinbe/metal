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
    var array = IA(N(4), N(5), N(6));
    MLAssertEquals(MLAtCount(array, N(0), N(0)), IA(), "[4, 5, 6] at 0 count 0 should return []");
    MLAssertEquals(MLAtCount(array, N(0), N(1)), IA(N(4)), "[4, 5, 6] at 0 count 1 should return [4]");
    MLAssertEquals(MLAtCount(array, N(0), N(3)), array, "[4, 5, 6] at 0 count 3 should return the same array");
    MLAssertEquals(MLAtCount(array, N(0), N(9)), array, "[4, 5, 6] at 0 count 9 (more than array contains) should return the same array");
    MLAssertEquals(MLAtCount(array, N(1), N(2)), IA(N(5), N(6)), "[4, 5, 6] at 1 count 2 should return [5, 6]");
    MLAssertEquals(MLAtCount(array, N(2), N(2)), IA(N(6)), "[4, 5, 6] at 2 count 2 should return [6]");
    MLAssertEquals(MLAtCount(array, N(9), N(0)), IA(), "[4, 5, 6] at 9 count 0 should return []");
    MLAssertEquals(MLAtCount(array, N(9), N(8)), IA(), "[4, 5, 6] at 9 count 8 should return []");
}


void MLTestArrayIndexOf() {
    var array = IA(N(4), N(5), N(6), N(4), N(6));
    MLAssertEquals(MLIndexOf(array, N(4)), N(0), "[4, 5, 6, 4, 6] index-of 4 should be 0");
    MLAssertEquals(MLIndexOf(array, N(5)), N(1), "[4, 5, 6, 4, 6] index-of 5 should be 1");
    MLAssertEquals(MLIndexOf(array, N(6)), N(2), "[4, 5, 6, 4, 6] index-of 6 should be 2");
    MLAssertEquals(MLIndexOf(array, N(7)), N(-1), "[4, 5, 6, 4, 6] index-of 7 should be -1");
}


void MLTestArrayLastIndexOf() {
    // TODO: implement.
}


void MLTestArrayIndexesOf() {
    // TODO: implement.
}


void MLTestArrayFirst() {
    // TODO: implement.
}


void MLTestArrayFirstCount() {
    // TODO: implement.
}


void MLTestArrayLast() {
    // TODO: implement.
}


void MLTestArrayLastCount() {
    // TODO: implement.
}


void MLTestArraySecond() {
    // TODO: implement.
}


void MLTestArrayThird() {
    // TODO: implement.
}


void MLTestArrayRest() {
    // TODO: implement.
}


void MLTestArrayWith() {
    // TODO: implement.
}


void MLTestArrayWithMany() {
    // TODO: implement.
}


void MLTestArrayWithAt() {
    // TODO: implement.
}


void MLTestArrayWithManyAt() {
    // TODO: implement.
}


void MLTestArrayWithBefore() {
    // TODO: implement.
}


void MLTestArrayWithManyBefore() {
    // TODO: implement.
}


void MLTestArrayWithAfter() {
    // TODO: implement.
}


void MLTestArrayWithManyAfter() {
    // TODO: implement.
}


void MLTestArrayWithReplacing() {
    // TODO: implement.
}


void MLTestArrayWithManyReplacing() {
    // TODO: implement.
}


void MLTestArrayWithReplacingAt() {
    // TODO: implement.
}


void MLTestArrayWithManyReplacingAt() {
    // TODO: implement.
}


void MLTestArrayWithReplacingAtCount() {
    // TODO: implement.
}


void MLTestArrayWithManyReplacingAtCount() {
    // TODO: implement.
}


void MLTestArrayWithout() {
    // TODO: implement.
}


void MLTestArrayWithoutMany() {
    // TODO: implement.
}


void MLTestArrayWithoutAt() {
    // TODO: implement.
}


void MLTestArrayWithoutAtMany() {
    // TODO: implement.
}


void MLTestArrayWithoutAtCount() {
    // TODO: implement.
}


void MLTestArrayReversed() {
    // TODO: implement.
}


void MLTestArraySorted() {
    // TODO: implement.
}


void MLTestArrayPluck() {
    // TODO: implement.
}


void MLTestArrayMin() {
    // TODO: implement.
}


void MLTestArrayMax() {
    // TODO: implement.
}


void MLTestMutableArrayAdd() {
    // TODO: implement.
}


void MLTestMutableArrayAddMany() {
    // TODO: implement.
}


void MLTestMutableArrayInsert() {
    // TODO: implement.
}


void MLTestMutableArrayInsertMany() {
    // TODO: implement.
}


void MLTestMutableArrayInsertAt() {
    // TODO: implement.
}


void MLTestMutableArrayInsertManyAt() {
    // TODO: implement.
}


void MLTestMutableArrayInsertBefore() {
    // TODO: implement.
}


void MLTestMutableArrayInsertManyBefore() {
    // TODO: implement.
}


void MLTestMutableArrayInsertAfter() {
    // TODO: implement.
}


void MLTestMutableArrayInsertManyAfter() {
    // TODO: implement.
}


void MLTestMutableArrayReplaceWith() {
    // TODO: implement.
}


void MLTestMutableArrayReplaceWithMany() {
    // TODO: implement.
}


void MLTestMutableArrayReplaceAtWith() {
    // TODO: implement.
}


void MLTestMutableArrayReplaceAtWithMany() {
    // TODO: implement.
}


void MLTestMutableArrayReplaceAtCountWith() {
    // TODO: implement.
}


void MLTestMutableArrayReplaceAtCountWithMany() {
    // TODO: implement.
}


void MLTestMutableArrayRemove() {
    // TODO: implement.
}


void MLTestMutableArrayRemoveMany() {
    // TODO: implement.
}


void MLTestMutableArrayRemoveAt() {
    // TODO: implement.
}


void MLTestMutableArrayRemoveAtMany() {
    // TODO: implement.
}


void MLTestMutableArrayRemoveAtCount() {
    // TODO: implement.
}


void MLTestMutableArrayRemoveAll() {
    // TODO: implement.
}


void MLTestMutableArrayReverse() {
    // TODO: implement.
}


void MLTestMutableArraySort() {
    // TODO: implement.
}


void MLTestMutableArrayIsMutable() {
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
    MLTestArrayIndexOf();
    MLTestArrayLastIndexOf();
    MLTestArrayIndexesOf();
    MLTestArrayFirst();
    MLTestArrayFirstCount();
    MLTestArrayLast();
    MLTestArrayLastCount();
    MLTestArraySecond();
    MLTestArrayThird();
    MLTestArrayRest();
    MLTestArrayWith();
    MLTestArrayWithMany();
    MLTestArrayWithAt();
    MLTestArrayWithManyAt();
    MLTestArrayWithBefore();
    MLTestArrayWithManyBefore();
    MLTestArrayWithAfter();
    MLTestArrayWithManyAfter();
    MLTestArrayWithReplacing();
    MLTestArrayWithManyReplacing();
    MLTestArrayWithReplacingAt();
    MLTestArrayWithManyReplacingAt();
    MLTestArrayWithReplacingAtCount();
    MLTestArrayWithManyReplacingAtCount();
    MLTestArrayWithout();
    MLTestArrayWithoutMany();
    MLTestArrayWithoutAt();
    MLTestArrayWithoutAtMany();
    MLTestArrayWithoutAtCount();
    MLTestArrayReversed();
    MLTestArraySorted();
    MLTestArrayPluck();
    MLTestArrayMin();
    MLTestArrayMax();
    MLTestMutableArrayAdd();
    MLTestMutableArrayAddMany();
    MLTestMutableArrayInsert();
    MLTestMutableArrayInsertMany();
    MLTestMutableArrayInsertAt();
    MLTestMutableArrayInsertManyAt();
    MLTestMutableArrayInsertBefore();
    MLTestMutableArrayInsertManyBefore();
    MLTestMutableArrayInsertAfter();
    MLTestMutableArrayInsertManyAfter();
    MLTestMutableArrayReplaceWith();
    MLTestMutableArrayReplaceWithMany();
    MLTestMutableArrayReplaceAtWith();
    MLTestMutableArrayReplaceAtWithMany();
    MLTestMutableArrayReplaceAtCountWith();
    MLTestMutableArrayReplaceAtCountWithMany();
    MLTestMutableArrayRemove();
    MLTestMutableArrayRemoveMany();
    MLTestMutableArrayRemoveAt();
    MLTestMutableArrayRemoveAtMany();
    MLTestMutableArrayRemoveAtCount();
    MLTestMutableArrayRemoveAll();
    MLTestMutableArrayReverse();
    MLTestMutableArraySort();
    MLTestMutableArrayIsMutable();
}
