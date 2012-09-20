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
    MLAssertEquals(MLIndexOf(array, N(4)), N(0), "[4, 5, 6, 4, 6] index-of 4 should return 0");
    MLAssertEquals(MLIndexOf(array, N(5)), N(1), "[4, 5, 6, 4, 6] index-of 5 should return 1");
    MLAssertEquals(MLIndexOf(array, N(6)), N(2), "[4, 5, 6, 4, 6] index-of 6 should return 2");
    MLAssertEquals(MLIndexOf(array, N(7)), N(-1), "[4, 5, 6, 4, 6] index-of 7 should return -1");
}


void MLTestArrayLastIndexOf() {
    var array = IA(N(4), N(5), N(6), N(4), N(6));
    MLAssertEquals(MLLastIndexOf(array, N(4)), N(3), "[4, 5, 6, 4, 6] last-index-of 4 should return 3");
    MLAssertEquals(MLLastIndexOf(array, N(5)), N(1), "[4, 5, 6, 4, 6] last-index-of 5 should return 1");
    MLAssertEquals(MLLastIndexOf(array, N(6)), N(4), "[4, 5, 6, 4, 6] last-index-of 6 should return 4");
    MLAssertEquals(MLLastIndexOf(array, N(7)), N(-1), "[4, 5, 6, 4, 6] last-index-of 7 should return -1");
}


void MLTestArrayIndexesOf() {
    var array = IA(N(4), N(5), N(6), N(4), N(6), N(4));
    MLAssertEquals(MLIndexesOf(array, N(5)), IA(N(1)), "[4, 5, 6, 4, 6, 4] indexes-of 5 should return [1]");
    MLAssertEquals(MLIndexesOf(array, N(4)), IA(N(0), N(3), N(5)), "[4, 5, 6, 4, 6, 4] indexes-of 4 should return [0, 3, 5]");
    MLAssertEquals(MLIndexesOf(array, N(9)), IA(), "[4, 5, 6, 4, 6, 4] indexes-of 9 should return []");
}


void MLTestArrayFirst() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertNull(MLFirst(array1), "[] first should be null");
    MLAssertEquals(MLFirst(array2), N(5), "[5] first should be 5");
    MLAssertEquals(MLFirst(array3), N(4), "[4, 5, 6] first should be 4");
}


void MLTestArrayFirstCount() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertEquals(MLFirstCount(array1, N(0)), IA(), "[] first 0 should be []");
    MLAssertEquals(MLFirstCount(array1, N(1)), IA(), "[] first 1 should be []");
    MLAssertEquals(MLFirstCount(array1, N(9)), IA(), "[] first 9 should be []");
    MLAssertEquals(MLFirstCount(array2, N(0)), IA(), "[5] first 0 should be []");
    MLAssertEquals(MLFirstCount(array2, N(1)), IA(N(5)), "[5] first 1 should be [5]");
    MLAssertEquals(MLFirstCount(array2, N(2)), IA(N(5)), "[5] first 2 should be [5]");
    MLAssertEquals(MLFirstCount(array2, N(9)), IA(N(5)), "[5] first 9 should be [5]");
    MLAssertEquals(MLFirstCount(array3, N(0)), IA(), "[4, 5, 6] first 0 should be []");
    MLAssertEquals(MLFirstCount(array3, N(1)), IA(N(4)), "[4, 5, 6] first 1 should be [4]");
    MLAssertEquals(MLFirstCount(array3, N(2)), IA(N(4), N(5)), "[4, 5, 6] first 2 should be [4, 5]");
    MLAssertEquals(MLFirstCount(array3, N(3)), IA(N(4), N(5), N(6)), "[4, 5, 6] first 3 should be [4, 5, 6]");
    MLAssertEquals(MLFirstCount(array3, N(4)), IA(N(4), N(5), N(6)), "[4, 5, 6] first 4 should be [4, 5, 6]");
    MLAssertEquals(MLFirstCount(array3, N(9)), IA(N(4), N(5), N(6)), "[4, 5, 6] first 9 should be [4, 5, 6]");
}


void MLTestArrayLast() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertNull(MLLast(array1), "[] last should be null");
    MLAssertEquals(MLLast(array2), N(5), "[5] last should be 5");
    MLAssertEquals(MLLast(array3), N(6), "[4, 5, 6] last should be 6");
}


void MLTestArrayLastCount() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertEquals(MLLastCount(array1, N(0)), IA(), "[] last 0 should be []");
    MLAssertEquals(MLLastCount(array1, N(1)), IA(), "[] last 1 should be []");
    MLAssertEquals(MLLastCount(array1, N(9)), IA(), "[] last 9 should be []");
    MLAssertEquals(MLLastCount(array2, N(0)), IA(), "[5] last 0 should be []");
    MLAssertEquals(MLLastCount(array2, N(1)), IA(N(5)), "[5] last 1 should be [5]");
    MLAssertEquals(MLLastCount(array2, N(2)), IA(N(5)), "[5] last 2 should be [5]");
    MLAssertEquals(MLLastCount(array2, N(9)), IA(N(5)), "[5] last 9 should be [5]");
    MLAssertEquals(MLLastCount(array3, N(0)), IA(), "[4, 5, 6] last 0 should be []");
    MLAssertEquals(MLLastCount(array3, N(1)), IA(N(6)), "[4, 5, 6] last 1 should be [6]");
    MLAssertEquals(MLLastCount(array3, N(2)), IA(N(5), N(6)), "[4, 5, 6] last 2 should be [5, 6]");
    MLAssertEquals(MLLastCount(array3, N(3)), IA(N(4), N(5), N(6)), "[4, 5, 6] last 3 should be [4, 5, 6]");
    MLAssertEquals(MLLastCount(array3, N(4)), IA(N(4), N(5), N(6)), "[4, 5, 6] last 4 should be [4, 5, 6]");
    MLAssertEquals(MLLastCount(array3, N(9)), IA(N(4), N(5), N(6)), "[4, 5, 6] last 9 should be [4, 5, 6]");
}


void MLTestArraySecond() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertNull(MLSecond(array1), "[] second should be null");
    MLAssertNull(MLSecond(array2), "[5] second should be null");
    MLAssertEquals(MLSecond(array3), N(5), "[4, 5, 6] second should be 5");
}


void MLTestArrayThird() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertNull(MLThird(array1), "[] third should be null");
    MLAssertNull(MLThird(array2), "[5] third should be null");
    MLAssertEquals(MLThird(array3), N(6), "[4, 5, 6] third should be 6");
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
    var array1 = IA(N(1));
    var array2 = IA(N(1), N(2));
    var array3 = IA(N(1), N(2), N(3));
    MLAssertEquals(MLReversed(array1), array1, "[1] reversed should return same array");
    MLAssertEquals(MLReversed(array2), IA(N(2), N(1)), "[1, 2] reversed should return [2, 1]");
    MLAssertEquals(MLReversed(array3), IA(N(3), N(2), N(1)), "[1, 2, 3] reverse should return [3, 2, 1]");
}


void MLTestArraySorted() {
    // TODO: implement.
}


void MLTestArrayPluck() {
    // TODO: implement.
}


void MLTestArrayMin() {
    var array1 = IA();
    var array2 = IA(N(2), N(-1), N(3));
    MLAssertNull(MLMin(array1), "[] min should be null");
    MLAssertEquals(MLMin(array2), N(-1), "[2, -1, 3] min should be -1");
}


void MLTestArrayMax() {
    var array1 = IA();
    var array2 = IA(N(2), N(-1), N(3));
    MLAssertNull(MLMax(array1), "[] max should be null");
    MLAssertEquals(MLMax(array2), N(3), "[2, -1, 3] max should be 3");
}


void MLTestMutableArrayAdd() {
    var array = MA();
    MLAdd(array, N(4));
    MLAssertEquals(array, IA(N(4)), "[] add 4 should change array to [4]");
    MLAdd(array, N(5));
    MLAssertEquals(array, IA(N(4), N(5)), "[4] add 5 should change array to [4, 5]");
    MLAdd(array, N(6));
    MLAssertEquals(array, IA(N(4), N(5), N(6)), "[4, 5] add 6 should change array to [4, 5, 6]");
}


void MLTestMutableArrayAddMany() {
    var array = MA();
    MLAddMany(array, IA());
    MLAssertEquals(array, IA(), "[] add-many [] should not change array and leave it as []");
    MLAddMany(array, IA(N(4), N(5)));
    MLAssertEquals(array, IA(N(4), N(5)), "[] add-many [4, 5] should change array to [4, 5]");
    MLAddMany(array, IA(N(6), N(7), N(8)));
    MLAssertEquals(array, IA(N(4), N(5), N(6), N(7), N(8)), "[4, 5] add-many [6, 7, 8] should change array to [4, 5, 6, 7, 8]");
    MLAddMany(array, IA());
    MLAssertEquals(array, IA(N(4), N(5), N(6), N(7), N(8)), "[4, 5, 6, 7, 8] add-many [] should not change the array and leave it as [4, 5, 6, 7, 8]");
}


void MLTestMutableArrayInsert() {
    var array = MA();
    MLInsert(array, N(6));
    MLAssertEquals(array, IA(N(6)), "[] insert 4 should change array to [6]");
    MLInsert(array, N(5));
    MLAssertEquals(array, IA(N(5), N(6)), "[6] insert 5 should change array to [5, 6]");
    MLInsert(array, N(4));
    MLAssertEquals(array, IA(N(4), N(5), N(6)), "[5, 6] insert 4 should change array to [4, 5, 6]");
}


void MLTestMutableArrayInsertMany() {
    var array = MA();
    MLInsertMany(array, IA());
    MLAssertEquals(array, IA(), "[] insert-many [] should not change array and leave it as []");
    MLInsertMany(array, IA(N(5), N(4)));
    MLAssertEquals(array, IA(N(5), N(4)), "[] insert-many [5, 4] should change array to [5, 4]");
    MLInsertMany(array, IA(N(8), N(7), N(6)));
    MLAssertEquals(array, IA(N(8), N(7), N(6), N(5), N(4)), "[5, r] insert-many [8, 7, 6] should change array to [8, 7, 6, 5, 4]");
    MLInsertMany(array, IA());
    MLAssertEquals(array, IA(N(8), N(7), N(6), N(5), N(4)), "[4, 5, 6, 7, 8] insert-many [] should not change the array and leave it as [8, 7, 6, 5, 4]");
}


void MLTestMutableArrayInsertAt() {
    var array = MA();
    MLInsertAt(array, N(4), N(0));
    MLAssertEquals(array, IA(N(4)), "[] insert 4 at 0 should change array to [4]");
    MLInsertAt(array, N(6), N(1));
    MLAssertEquals(array, IA(N(4), N(6)), "[4] insert 6 at 1 should change array to [4, 6]");
    MLInsertAt(array, N(5), N(1));
    MLAssertEquals(array, IA(N(4), N(5), N(6)), "[4, 6] insert 4 at 1 should change array to [4, 5, 6]");
}


void MLTestMutableArrayInsertManyAt() {
    var array = MA();
    MLInsertManyAt(array, IA(), N(0));
    MLAssertEquals(array, IA(), "[] insert-many [] at 0 should not change array and leave it as []");
    MLInsertManyAt(array, IA(N(4), N(8)), N(0));
    MLAssertEquals(array, IA(N(4), N(8)), "[] insert-many [4, 8] at 0 should change array to [4, 8]");
    MLInsertManyAt(array, IA(N(5), N(6), N(7)), N(1));
    MLAssertEquals(array, IA(N(4), N(5), N(6), N(7), N(8)), "[4, 8] insert-many [5, 6, 7] at 1 should change array to [4, 5, 6, 7, 8]");
    MLInsertManyAt(array, IA(), N(2));
    MLAssertEquals(array, IA(N(4), N(5), N(6), N(7), N(8)), "[4, 5] insert-many [] at 2 should not change the array and leave it as [4, 5, 6, 7, 8]");
}


void MLTestMutableArrayInsertBefore() {
    var array = MA();
    MLInsertBefore(array, N(6), N(9));
    MLAssertEquals(array, IA(N(6)), "[] insert 6 before 9 should change array to [6]");
    MLInsertBefore(array, N(4), N(9));
    MLAssertEquals(array, IA(N(4), N(6)), "[6] insert 4 before 9 should change array to [4, 6]");
    MLInsertBefore(array, N(5), N(6));
    MLAssertEquals(array, IA(N(4), N(5), N(6)), "[4, 6] insert 5 before 6 should change array to [4, 5, 6]");
    MLInsertBefore(array, N(5), N(4));
    MLAssertEquals(array, IA(N(5), N(4), N(5), N(6)), "[4, 5, 6] insert 5 before 4 should change array to [5, 4, 5, 6]");
    MLInsertBefore(array, N(6), N(5));
    MLAssertEquals(array, IA(N(6), N(5), N(4), N(5), N(6)), "[5, 4, 5, 6] insert 6 before 5 should change array to [6, 5, 4, 5, 6]");
}


void MLTestMutableArrayInsertManyBefore() {
    var array = MA();
    MLInsertManyBefore(array, IA(N(6)), N(9));
    MLAssertEquals(array, IA(N(6)), "[] insert-many [6] before 9 should change array to [6]");
    MLInsertManyBefore(array, IA(N(4), N(5)), N(9));
    MLAssertEquals(array, IA(N(4), N(5), N(6)), "[6] insert-many [4, 5] before 9 should change array to [4, 5, 6]");
    MLInsertManyBefore(array, IA(N(4), N(5)), N(6));
    MLAssertEquals(array, IA(N(4), N(5), N(4), N(5), N(6)), "[4, 5, 6] insert-many [4, 5] before 6 should change array to [4, 5, 4, 5, 6]");
}


void MLTestMutableArrayInsertAfter() {
    var array = MA();
    MLInsertAfter(array, N(6), N(9));
    MLAssertEquals(array, IA(N(6)), "[] insert 6 after 9 should change array to [6]");
    MLInsertAfter(array, N(4), N(9));
    MLAssertEquals(array, IA(N(6), N(4)), "[6] insert 4 after 9 should change array to [6, 4]");
    MLInsertAfter(array, N(5), N(6));
    MLAssertEquals(array, IA(N(6), N(5), N(4)), "[6, 4] insert 5 after 6 should change array to [6, 5, 4]");
    MLInsertAfter(array, N(5), N(4));
    MLAssertEquals(array, IA(N(6), N(5), N(4), N(5)), "[6, 5, 4] insert 5 after 4 should change array to [6, 5, 4, 5]");
    MLInsertAfter(array, N(6), N(5));
    MLAssertEquals(array, IA(N(6), N(5), N(4), N(5), N(6)), "[6, 5, 4, 5] insert 6 after 5 should change array to [6, 5, 4, 5, 6]");
}


void MLTestMutableArrayInsertManyAfter() {
    var array = MA();
    MLInsertManyAfter(array, IA(N(6)), N(9));
    MLAssertEquals(array, IA(N(6)), "[] insert-many [6] after 9 should change array to [6]");
    MLInsertManyAfter(array, IA(N(4), N(5)), N(9));
    MLAssertEquals(array, IA(N(6), N(4), N(5)), "[6] insert-many [4, 5] after 9 should change array to [6, 4, 5]");
    MLInsertManyAfter(array, IA(N(4), N(5)), N(6));
    MLAssertEquals(array, IA(N(6), N(4), N(5), N(4), N(5)), "[6, 4, 5] insert-many [4, 5] after 6 should change array to [6, 4, 5, 4, 5]");
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
    var array1 = MA(N(1));
    var array2 = MA(N(1), N(2));
    var array3 = MA(N(1), N(2), N(3));
    MLReverse(array1);
    MLReverse(array2);
    MLReverse(array3);
    MLAssertEquals(array1, IA(N(1)), "[1] reverse should not change the array and leave it as [1]");
    MLAssertEquals(array2, IA(N(2), N(1)), "[1, 2] reverse should change array to [2, 1]");
    MLAssertEquals(array3, IA(N(3), N(2), N(1)), "[1, 2, 3] reverse should change array to [3, 2, 1]");
}


void MLTestMutableArraySort() {
    var array1 = MA(N(1));
    var array2 = MA(N(2), N(1));
    var array3 = MA(N(3), N(2), N(1), N(4), N(6), N(5));
    MLSort(array1);
    MLSort(array2);
    MLSort(array3);
    MLAssertEquals(array1, IA(N(1)), "[1] sort should not change the array and leave it as [1]");
    MLAssertEquals(array2, IA(N(1), N(2)), "[2, 1] sort should change array to [1, 2]");
    MLAssertEquals(array3, IA(N(1), N(2), N(3), N(4), N(5), N(6)), "[3, 2, 1, 4, 6, 5] sort should change array to [1, 2, 3, 4, 5, 6]");
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
}
