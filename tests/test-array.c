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
    MLAssertEquals(count, N(3), "Array -count returns the number of objects contained in the array");
}


void MLTestArrayContains() {
    var array = IA(N(1), N(2), N(3));
    MLAssertFalse(MLContains(array, N(0)), "Array -contains* returns no if object is not in array");
    MLAssertTrue(MLContains(array, N(1)), "Array -contains* yes if object is contained in array");
    MLAssertTrue(MLContains(array, N(2)), "Array -contains* yes if object is contained in array");
    MLAssertTrue(MLContains(array, N(3)), "Array -contains* yes if object is contained in array");
    MLAssertFalse(MLContains(array, N(4)), "Array -contains* returns no if object is not in array");
}


void MLTestArrayContainsAny() {
    var array = IA(N(1), N(2), N(3));
    MLAssertFalse(MLContainsAny(array, IA()), "Array contains-any* returns no if passed in objects array is empty");
    MLAssertFalse(MLContainsAny(array, IA(N(0), N(4))), "Array -contains-any* returns no if none of the passed in objects is contained in array");
    MLAssertTrue(MLContainsAny(array, IA(N(0), N(3))), "Array -contains-any* returns yes if all of the passed in objects are contained in array");
    MLAssertTrue(MLContainsAny(array, IA(N(1), N(3))), "Array -contains-any* returns yes of at least one of the passed in objects is contained in array");
}


void MLTestArrayContainsAll() {
    var array = IA(N(1), N(2), N(3));
    MLAssertFalse(MLContainsAll(array, IA(N(0), N(4))), "Array -contains-all* returns no if none of the passed in objects are contained in array");
    MLAssertFalse(MLContainsAll(array, IA(N(1), N(4))), "Array -contains-all* returns no if at least one of the passed in objects is not contained in array");
    MLAssertTrue(MLContainsAll(array, IA(N(1), N(3))), "Array -contains-all* returns yes if all of the passed in objects are contained in array");
    MLAssertTrue(MLContainsAll(array, IA()), "Array -contains-all* returns yes if the passed in array if objects is empty");
}


void MLTestArrayIsEmpty() {
    var arrayWithNoObjects = IA();
    var arrayWithOneObject = IA(N(1));
    var arrayWithManyObjects = IA(N(1), N(2), N(3));
    MLAssertTrue(MLIsEmpty(arrayWithNoObjects), "Array -is-empty? returns yes if array doesn't contain any objects");
    MLAssertFalse(MLIsEmpty(arrayWithOneObject), "Array -is-empty? returns no if array contains one object");
    MLAssertFalse(MLIsEmpty(arrayWithManyObjects), "Array -is-empty? returns no if array contains more than one object");
}


void MLTestArrayIsInline() {
    var array = A();
    var inlineArray = IA();
    var mutableArray = MA();
    MLAssertFalse(MLIsInline(array), "Array -is-inline? returns no if array is of type Array");
    MLAssertTrue(MLIsInline(inlineArray), "Array -is-inline? returns yes if array is of type InlineArray");
    MLAssertFalse(MLIsInline(mutableArray), "Array -is-inline? returns no if array is of type MutableArray");
}


void MLTestArrayIsMutable() {
    var array = A();
    var inlineArray = IA();
    var mutableArray = MA();
    MLAssertFalse(MLIsMutable(array), "Array -is-mutable? returns no if array is of type Array");
    MLAssertFalse(MLIsMutable(inlineArray), "Array -is-mutable? returns no if array is of type InlineArray");
    MLAssertTrue(MLIsMutable(mutableArray), "Array -is-mutable? returns yes if array is of type MutableArray");
}


void MLTestArrayAt() {
    var array = IA(N(1), N(2), N(3));
    MLAssertEquals(MLAt(array, N(0)), N(1), "Array -at* returns the first object in array if index = 0");
    MLAssertEquals(MLAt(array, N(1)), N(2), "Array -at* returns the object at index if index is within bounds");
    MLAssertEquals(MLAt(array, N(2)), N(3), "Array -at* returns the last object if index = count - 1");
    MLAssertThrows(null, "Array -at* throws if index < 0") MLAt(array, N(-1));
    MLAssertThrows(null, "Array -at* throws if index = count") MLAt(array, N(3));
    MLAssertThrows(null, "Array -at* throws if index > count") MLAt(array, N(9));
}


void MLTestArrayAtMany() {
    var array = IA(N(4), N(5), N(6));
    MLAssertEquals(MLAtMany(array, IA()), IA(), "Array -at-many* returns an empty array if passed indexes array is empty");
    MLAssertEquals(MLAtMany(array, IA(N(1))), IA(N(5)), "Array -at-many* returns an array containing the object when passing one index");
    MLAssertEquals(MLAtMany(array, IA(N(0), N(1), N(2))), array, "Array -at-many* returns an exact copy if passing all indexes that the array contains sorted ascending");
    MLAssertThrows(null, "Array -at-many* throws if any of the indexes is < 0") MLAtMany(array, IA(N(-1), N(0), N(1), N(2)));
    MLAssertThrows(null, "Array -at-many* throws if any of the indexes is = count") MLAtMany(array, IA(N(0), N(1), N(3)));
    MLAssertThrows(null, "Array -at-many* throws if any of the indexes is > count") MLAtMany(array, IA(N(0), N(1), N(9)));
}


void MLTestArrayAtCount() {
    var array = IA(N(4), N(5), N(6));
    MLAssertEquals(MLAtCount(array, N(0), N(0)), IA(), "Array -at*count* returns an empty array if count = 0");
    MLAssertEquals(MLAtCount(array, N(0), N(1)), IA(N(4)), "Array -at*count* returns an array containing the first object if index = 0, and count = 1");
    MLAssertEquals(MLAtCount(array, N(0), N(3)), array, "Array -at*count* returns an array with first `count` elements if index = 0");
    MLAssertEquals(MLAtCount(array, N(0), N(9)), array, "Array -at*count* ignores rest if count >= number of objects in array and index = 0");
    MLAssertEquals(MLAtCount(array, N(1), N(2)), IA(N(5), N(6)), "Array at*count* returns an array with count elements starting at index");
    MLAssertEquals(MLAtCount(array, N(2), N(2)), IA(N(6)), "Array -at*count* ignores rest if index + count >= number of objects in array and index > 0");
    MLAssertThrows(null, "Array -at*count* throws in index = count") MLAtCount(array, N(3), N(0));
    MLAssertThrows(null, "Array -at*count* throws in index > count") MLAtCount(array, N(4), N(0));
}


void MLTestArrayIndexOf() {
    var array = IA(N(4), N(5), N(6), N(4), N(6));
    MLAssertEquals(MLIndexOf(array, N(5)), N(1), "Array -index-of* returns the index of object if object is contained only once");
    MLAssertEquals(MLIndexOf(array, N(6)), N(2), "Array -index-of* returns the index of the first occurence of passed in object if object is contained multiple times");
    MLAssertEquals(MLIndexOf(array, N(7)), N(-1), "Array -index-of* returns -1 if object is not in array");
}


void MLTestArrayLastIndexOf() {
    var array = IA(N(4), N(5), N(6), N(4), N(6));
    MLAssertEquals(MLLastIndexOf(array, N(4)), N(3), "Array -last-index-of* returns the index of object if object is contained only once");
    MLAssertEquals(MLLastIndexOf(array, N(6)), N(4), "Array -last-index-of* returns the index of the last occurence of object if object is contained multiple times");
    MLAssertEquals(MLLastIndexOf(array, N(7)), N(-1), "Array -last-index-of* returns -1 if object is not in array");
}


void MLTestArrayIndexesOf() {
    var array = IA(N(4), N(5), N(6), N(4), N(6), N(4));
    MLAssertEquals(MLIndexesOf(array, N(5)), IA(N(1)), "Array -indexes-of* returns an array with the index of object if object is contained only once");
    MLAssertEquals(MLIndexesOf(array, N(4)), IA(N(0), N(3), N(5)), "Array -indexes-of* returns an array with indexes of all occurences of object if it is contained multiple times");
    MLAssertEquals(MLIndexesOf(array, N(9)), IA(), "Array -indexes-of* returns an empty array of object is not contained in array");
}


void MLTestArrayFirst() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertNull(MLFirst(array1), "Array -first returns null if array is empty");
    MLAssertEquals(MLFirst(array2), N(5), "Array -first returns the first object when array has only one");
    MLAssertEquals(MLFirst(array3), N(4), "Array -first returns the first object when array has more than one");
}


void MLTestArrayFirstCount() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertEquals(MLFirstCount(array1, N(0)), IA(), "Array first* returns an empty array if array is empty and count = 0");
    MLAssertEquals(MLFirstCount(array1, N(2)), IA(), "Array first* returns an empty array if array is empty and count > 0 (here: count = 1)");
    MLAssertEquals(MLFirstCount(array1, N(9)), IA(), "Array first* returns an empty array if array is empty and count > 0 (here: count = 9)");
    MLAssertEquals(MLFirstCount(array2, N(0)), IA(), "Array first* returns an empty array if array is not empty but count = 0");
    MLAssertEquals(MLFirstCount(array2, N(1)), IA(N(5)), "Array first* returns an exacy copy if array contains 1 object and count = 1");
    MLAssertEquals(MLFirstCount(array2, N(2)), IA(N(5)), "Array first* returns an exacy copy if array contains 1 object and count > 1 (here: count = 2)");
    MLAssertEquals(MLFirstCount(array2, N(9)), IA(N(5)), "Array first* returns an exacy copy if array contains 1 object and count > 1 (here: count = 9)");
    MLAssertEquals(MLFirstCount(array3, N(0)), IA(), "Array first* returns an empty array if array contains many objects and count = 0");
    MLAssertEquals(MLFirstCount(array3, N(1)), IA(N(4)), "Array first* returns an array containing just the first object if array contains many objects and count = 1");
    MLAssertEquals(MLFirstCount(array3, N(2)), IA(N(4), N(5)), "Array first* returns an array containing just the first N objects if array contains many objects and count = N (here: N = 2)");
    MLAssertEquals(MLFirstCount(array3, N(3)), IA(N(4), N(5), N(6)), "Array first* returns an exacy copy if array contains N objects and count = N (here: N = 3)");
    MLAssertEquals(MLFirstCount(array3, N(4)), IA(N(4), N(5), N(6)), "Array first* returns an exacy copy if array contains N objects and count > N (here: N = 3, count = 4)");
    MLAssertEquals(MLFirstCount(array3, N(9)), IA(N(4), N(5), N(6)), "Array first* returns an exacy copy if array contains N objects and count > N (here: N = 3, count = 9)");
}


void MLTestArrayLast() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertNull(MLLast(array1), "Array -last returns null if array is empty");
    MLAssertEquals(MLLast(array2), N(5), "Array -last returns the last object when array has only one");
    MLAssertEquals(MLLast(array3), N(6), "Array -last returns the last object when array has more than one");
}


void MLTestArrayLastCount() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertEquals(MLLastCount(array1, N(0)), IA(), "Array last* returns an empty array if array is empty and count = 0");
    MLAssertEquals(MLLastCount(array1, N(1)), IA(), "Array last* returns an empty array if array is empty and count > 0 (here: count = 1)");
    MLAssertEquals(MLLastCount(array1, N(9)), IA(), "Array last* returns an empty array if array is empty and count > 0 (here: count = 9)");
    MLAssertEquals(MLLastCount(array2, N(0)), IA(), "Array last* returns an empty array if array is not empty but count = 0");
    MLAssertEquals(MLLastCount(array2, N(1)), IA(N(5)), "Array last* returns an exacy copy if array contains 1 object and count = 1");
    MLAssertEquals(MLLastCount(array2, N(2)), IA(N(5)), "Array last* returns an exacy copy if array contains 1 object and count > 1 (here: count = 2)");
    MLAssertEquals(MLLastCount(array2, N(9)), IA(N(5)), "Array last* returns an exacy copy if array contains 1 object and count > 1 (here: count = 9)");
    MLAssertEquals(MLLastCount(array3, N(0)), IA(), "Array last* returns an empty array if array contains many objects and count = 0");
    MLAssertEquals(MLLastCount(array3, N(1)), IA(N(6)), "Array last* returns an array containing just the last object if array contains many objects and count = 1");
    MLAssertEquals(MLLastCount(array3, N(2)), IA(N(5), N(6)), "Array last* returns an array containing just the last N objects if array contains many objects and count = N (here: N = 2)");
    MLAssertEquals(MLLastCount(array3, N(3)), IA(N(4), N(5), N(6)), "Array first* returns an exacy copy if array contains N objects and count = N (here: N = 3)");
    MLAssertEquals(MLLastCount(array3, N(4)), IA(N(4), N(5), N(6)), "Array first* returns an exacy copy if array contains N objects and count > N (here: N = 3, count = 4)");
    MLAssertEquals(MLLastCount(array3, N(9)), IA(N(4), N(5), N(6)), "Array first* returns an exacy copy if array contains N objects and count > N (here: N = 3, count = 9)");
}


void MLTestArraySecond() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertNull(MLSecond(array1), "Array -second returns null if array is empty");
    MLAssertNull(MLSecond(array2), "Array -second returns null if array has only one object");
    MLAssertEquals(MLSecond(array3), N(5), "Array -second returns the second if array has more than one object");
}


void MLTestArrayThird() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertNull(MLThird(array1), "Array -third returns null if array is empty");
    MLAssertNull(MLThird(array2), "Array -third returns null if array contains less than 3 objects");
    MLAssertEquals(MLThird(array3), N(6), "Array -third returns the third object if array contains at least 3 objects");
}


void MLTestArrayRest() {
    var array1 = IA();
    var array2 = IA(N(5));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertEquals(MLRest(array1), IA(), "Array -rest returns an empty array if receiver is empty");
    MLAssertEquals(MLRest(array2), IA(), "Array -rest returns an empty array if receiver contains only one object");
    MLAssertEquals(MLRest(array3), IA(N(5), N(6)), "Array -rest returns an array containing all but the first object");
}


void MLTestArrayWith() {
    var array = IA(N(1), N(2));
    MLAssertEquals(MLWith(array, N(3)), IA(N(1), N(2), N(3)), "Array -with* returns a new array by adding one object");
}


void MLTestArrayWithMany() {
    var array = IA(N(1), N(2));
    MLAssertEquals(MLWithMany(array, IA(N(3), N(4))), IA(N(1), N(2), N(3), N(4)), "Array -with-many* returns a new array by adding many objects");
    MLAssertEquals(MLWithMany(array, IA()), array, "Array -with-many* returns self without modifying it when passing no objects (empty array)");
}


void MLTestArrayWithAt() {
    var array1 = IA();
    var array2 = IA(N(4));
    var array3 = IA(N(4), N(6));
    MLAssertEquals(MLWithAt(array1, N(4), N(0)), IA(N(4)), "Array -with*at* returns a new array containing only the object when array is empty");
    MLAssertEquals(MLWithAt(array2, N(6), N(1)), IA(N(4), N(6)), "Array -with*at* returns a new array by inserting the object at the end if index = count");
    MLAssertEquals(MLWithAt(array3, N(5), N(1)), IA(N(4), N(5), N(6)), "Array -with*at* returns a new array by inserting the passed object at the specified index");
}


void MLTestArrayWithManyAt() {
    var array1 = IA();
    var array2 = IA();
    var array3 = IA(N(4), N(8));
    var array4 = IA(N(4), N(5), N(6), N(7), N(8));
    MLAssertEquals(MLWithManyAt(array1, IA(), N(0)), IA(), "Array -with-many*at*, when array is empty and no objects are passed, returns an empty array");
    MLAssertEquals(MLWithManyAt(array2, IA(N(4), N(8)), N(0)), IA(N(4), N(8)), "Array -with-many*at*, when array is empty and objects are passed, returns an array containing only that objects");
    MLAssertEquals(MLWithManyAt(array3, IA(N(5), N(6), N(7)), N(1)), IA(N(4), N(5), N(6), N(7), N(8)), "Array -with-many*at*, when array has objects and objects are passed, returns a new array by inserting the passed objects at the specified index");
    MLAssertEquals(MLWithManyAt(array4, IA(), N(2)), IA(N(4), N(5), N(6), N(7), N(8)), "Array -with-many*at*, when array has objects and no objects are passed, returns an exact copy of the array");
}


void MLTestArrayWithBefore() {
    var array1 = IA(N(4), N(6));
    var array2 = IA(N(6));
    var array3 = IA(N(5), N(4), N(5), N(6));
    var array4 = IA(N(4), N(5), N(6));
    var array5 = IA();
    MLAssertEquals(MLWithBefore(array1, N(5), N(6)), IA(N(4), N(5), N(6)), "Array -with*before* returns a new array by inserting the passed in object before a given object");
    MLAssertEquals(MLWithBefore(array2, N(4), N(9)), IA(N(4), N(6)), "Array -with*before* returns a new array by inserting the passing in object at the beginning if the object to isert before is not contained in the array");
    MLAssertEquals(MLWithBefore(array3, N(6), N(5)), IA(N(6), N(5), N(4), N(5), N(6)), "Array -with*before* returns a new array by inserting the passed in object before the first occurence of a given object if the bofore object is contained more than once in the array");
    MLAssertEquals(MLWithBefore(array4, N(5), N(4)), IA(N(5), N(4), N(5), N(6)), "Array -with*before* returns a new array by inserting the passed in object before a given object even if the passed in object is already in the array");
    MLAssertEquals(MLWithBefore(array5, N(6), N(9)), IA(N(6)), "Array -with*before* returns a new array containing just the passed in object if array is empty ignoring the before object");
}


void MLTestArrayWithManyBefore() {
    var array1 = IA();
    var array2 = IA(N(6));
    var array3 = IA(N(4), N(5), N(6));
    MLAssertEquals(MLWithManyBefore(array1, IA(N(6)), N(9)), IA(N(6)), "[] with-many [6] before 9 should return [6]");
    MLAssertEquals(MLWithManyBefore(array2, IA(N(4), N(5)), N(9)), IA(N(4), N(5), N(6)), "[6] with-many [4, 5] before 9 should return [4, 5, 6]");
    MLAssertEquals(MLWithManyBefore(array3, IA(N(4), N(5)), N(6)), IA(N(4), N(5), N(4), N(5), N(6)), "[4, 5, 6] with-many [4, 5] before 6 should return [4, 5, 4, 5, 6]");
}


void MLTestArrayWithAfter() {
    var array1 = IA(N(1), N(2), N(3), N(2), N(4));
    var array2 = IA(N(1), N(2), N(3));
    var array3 = IA();
    MLAssertEquals(MLWithAfter(array1, N(9), N(2)), IA(N(1), N(2), N(3), N(2), N(9), N(4)), "Array -with*after* returns a new array by inserting an object after the last occurence of the passed in object");
    MLAssertEquals(MLWithAfter(array2, N(9), N(7)), IA(N(1), N(2), N(3), N(9)), "Array -with*after* returns a new array by adding an object at the end if object after which to be inserted is not contained in the array");
    MLAssertEquals(MLWithAfter(array3, N(9), N(7)), IA(N(9)), "Array -with*after* returns a new array containing the object to be inserted if array is empty");
}


void MLTestArrayWithManyAfter() {
    var array1 = IA(N(1), N(2), N(3), N(2), N(4));
    var array2 = IA(N(1), N(2), N(3));
    var array3 = IA();
    MLAssertEquals(MLWithManyAfter(array1, IA(N(8), N(9)), N(2)), IA(N(1), N(2), N(3), N(2), N(8), N(9), N(4)), "Array -with-many*after* returns a new array by inserting an array of objects after the last occurence of the passed in object");
    MLAssertEquals(MLWithManyAfter(array2, IA(N(8), N(9)), N(7)), IA(N(1), N(2), N(3), N(8), N(9)), "Array -with-many*after* returns a new array by adding an array of objects at the end if object after which to be inserted is not contained in the array");
    MLAssertEquals(MLWithManyAfter(array3, IA(N(8), N(9)), N(7)), IA(N(8), N(9)), "Array -with-many*after* returns a new array containing only the objects to be inserted if array is empty");
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
    var array1 = IA(N(1), N(2), N(3), N(2));
    var array2 = IA(N(1), N(2), N(3));
    MLAssertEquals(MLWithout(array1, N(2)), IA(N(1), N(3)), "Array -without* returns a new array by removing all occurences of the passed in object");
    MLAssertEquals(MLWithout(array2, N(9)), IA(N(1), N(2), N(3)), "Array -without* returns an exact copy if passed in object is not contained in the array");
}


void MLTestArrayWithoutMany() {
    var array1 = IA(N(1), N(2), N(3), N(2), N(5), N(6));
    var array2 = IA(N(1), N(2), N(3));
    var array3 = IA(N(1), N(2), N(3));
    MLAssertEquals(MLWithoutMany(array1, IA(N(2), N(5))), IA(N(1), N(3), N(6)), "Array -without-many* returns a new array by removing all occurences of all passed in objects");
    MLAssertEquals(MLWithoutMany(array2, IA(N(2), N(5))), IA(N(1), N(3)), "Array -without-many* ignores objects that aren't contained in the array");
    MLAssertEquals(MLWithoutMany(array3, IA()), array3, "Array -without-many* returns an exact copy if passed in objects array is empty");
}


void MLTestArrayWithoutAt() {
    var array1 = IA(N(1), N(2), N(3), N(2));
    MLAssertEquals(MLWithoutAt(array1, N(1)), IA(N(1), N(3), N(2)), "Array -without-at* returns a new array by removing object at the passed in index");
}


void MLTestArrayWithoutAtMany() {
    var array1 = IA(N(1), N(2), N(3), N(4), N(5));
    var array2 = IA(N(1), N(2), N(3));
    MLAssertEquals(MLWithoutAtMany(array1, IA(N(1), N(3))), IA(N(1), N(3), N(5)), "Array -without-at-many* returns a new array by removing objects at the passed in indexes");
    MLAssertEquals(MLWithoutAtMany(array2, IA()), array2, "Array -without-at-many* returns an exact copy if indexes array is empty");
}


void MLTestArrayWithoutAtCount() {
    var array1 = IA(N(1), N(2), N(3), N(4), N(5));
    var array2 = IA(N(1), N(2), N(3));
    MLAssertEquals(MLWithoutAtCount(array1, N(1), N(3)), IA(N(1), N(5)), "Array -without-at*count* returns a new array by removing count objects starting at `index`");
    MLAssertEquals(MLWithoutAtCount(array2, N(1), N(0)), array2, "Array -without-at*count* returns an exact copy if `count` is 0");
}


void MLTestArrayReversed() {
    var array1 = IA(N(1));
    var array2 = IA(N(1), N(2));
    var array3 = IA(N(1), N(2), N(3));
    MLAssertEquals(MLReversed(array1), array1, "Array -reversed returns an exact copy if array contains only one object");
    MLAssertEquals(MLReversed(array2), IA(N(2), N(1)), "Array -reversed returns a new array with objects beeing in reversed order when array has even number of objects");
    MLAssertEquals(MLReversed(array3), IA(N(3), N(2), N(1)), "Array -reversed returns a new array with objects beeing in reversed order when array has odd number of objects");
}


void MLTestArraySorted() {
    var array1 = IA(N(1));
    var array2 = IA(N(2), N(1));
    var array3 = IA(N(3), N(2), N(1), N(4), N(6), N(5));
    MLAssertEquals(MLSorted(array1), IA(N(1)), "Array -sorted returns an exact copy if array contains only one object");
    MLAssertEquals(MLSorted(array2), IA(N(1), N(2)), "Array -sorted returns a new array with objects beeing sorted in ascending order when array contains 2 objects");
    MLAssertEquals(MLSorted(array3), IA(N(1), N(2), N(3), N(4), N(5), N(6)), "Array -sorted returns a new array with objects beeing sorted in ascending order when array any number of objects");
}


void MLTestArrayPluck() {
    var numbers = IA(N(1), N(2), N(3));
    MLAssertEquals(MLPluck(numbers, IS("is-number?")), IA(yes, yes, yes), "Array -pluck returns a new array containing the return values by sending the passed in command to each object (1/2)");
    MLAssertEquals(MLPluck(numbers, IS("is-boolean?")), IA(no, no, no), "Array -pluck returns a new array containing the return values by sending the passed in command to each object (2/2)");
}


void MLTestArrayMin() {
    var array1 = IA();
    var array2 = IA(N(2), N(-1), N(3));
    MLAssertNull(MLMin(array1), "Array -min returns null if array is empty");
    MLAssertEquals(MLMin(array2), N(-1), "Array -min returns the smallest object in array");
}


void MLTestArrayMax() {
    var array1 = IA();
    var array2 = IA(N(2), N(-1), N(3));
    MLAssertNull(MLMax(array1), "Array -max returns null if array if empty");
    MLAssertEquals(MLMax(array2), N(3), "Array -max returns the greatest object in array");
}


void MLTestMutableArrayAdd() {
    var array1 = MA();
    var array2 = MA(N(4));
    var array3 = MA(N(4), N(5));
    MLAdd(array1, N(4));
    MLAdd(array2, N(5));
    MLAdd(array3, N(6));
    MLAssertEquals(array1, IA(N(4)), "MutableArray -add* adds the object at the end (when array is empty)");
    MLAssertEquals(array2, IA(N(4), N(5)), "MutableArray -add* adds the object at the end (when array contains one object)");
    MLAssertEquals(array3, IA(N(4), N(5), N(6)), "MutableArray -add* adds the object at the end (when array contains multiple objects)");
}


void MLTestMutableArrayAddMany() {
    var array1 = MA();
    var array2 = MA();
    var array3 = MA(N(4), N(5));
    var array4 = MA(N(4), N(5), N(6), N(7), N(8));
    MLAddMany(array1, IA());
    MLAddMany(array2, IA(N(4), N(5)));
    MLAddMany(array3, IA(N(6), N(7), N(8)));
    MLAddMany(array4, IA());
    MLAssertEquals(array1, IA(), "MutableArray -add-many* doesn't change the array if passed in objects array is empty (when array is empty)");
    MLAssertEquals(array2, IA(N(4), N(5)), "MutableArray -add-many* adds many objects at the end (when array is empty)");
    MLAssertEquals(array3, IA(N(4), N(5), N(6), N(7), N(8)), "MutableArray -add-many* adds many objects at the end (when array is not empty)");
    MLAssertEquals(array4, IA(N(4), N(5), N(6), N(7), N(8)), "MutableArray -add-many* doesn't change the array if passed in objects array is empty (when array is not empty)");
}


void MLTestMutableArrayInsert() {
    var array1 = MA();
    var array2 = MA(N(6));
    var array3 = MA(N(5), N(6));
    MLInsert(array1, N(6));
    MLInsert(array2, N(5));
    MLInsert(array3, N(4));
    MLAssertEquals(array1, IA(N(6)), "MutableArray -insert* inserts the object at the beginning (when array is empty)");
    MLAssertEquals(array2, IA(N(5), N(6)), "MutableArray -insert* inserts the object at the beginning (when array contains one object)");
    MLAssertEquals(array3, IA(N(4), N(5), N(6)), "MutableArray -insert* inserts the object at the beginning (when array contains more than one object)");
}


void MLTestMutableArrayInsertMany() {
    var array1 = MA();
    var array2 = MA();
    var array3 = MA(N(5), N(4));
    var array4 = MA(N(8), N(7), N(6), N(5), N(4));
    MLInsertMany(array1, IA());
    MLInsertMany(array2, IA(N(5), N(4)));
    MLInsertMany(array3, IA(N(8), N(7), N(6)));
    MLInsertMany(array4, IA());
    MLAssertEquals(array1, IA(), "MutableArray -insert-many* doesn't change the array of passed in objects array is empty (when array is empty)");
    MLAssertEquals(array2, IA(N(5), N(4)), "MutableArray -insert-many* inserts objects at the beginning if the array (when array is empty)");
    MLAssertEquals(array3, IA(N(8), N(7), N(6), N(5), N(4)), "MutableArray -insert-many* inserts objects at the beginning if the array (when array is not empty)");
    MLAssertEquals(array4, IA(N(8), N(7), N(6), N(5), N(4)), "MutableArray -insert-many* doesn't change the array of passed in objects array is empty (when array is not empty)");
}


void MLTestMutableArrayInsertAt() {
    var array1 = MA();
    var array2 = MA(N(4));
    var array3 = MA(N(4), N(6));
    MLInsertAt(array1, N(4), N(0));
    MLInsertAt(array2, N(6), N(1));
    MLInsertAt(array3, N(5), N(1));
    MLAssertEquals(array1, IA(N(4)), "MutableArray -insert*at* inserts an object at index 0 into an empty array ");
    MLAssertEquals(array2, IA(N(4), N(6)), "MutableArray -insert*at* inserts an object at the end when index = count");
    MLAssertEquals(array3, IA(N(4), N(5), N(6)), "MutableArray -insert*at* inserts an object in the middle of an array when index is in the middle");
}


void MLTestMutableArrayInsertManyAt() {
    var array1 = MA(N(4), N(8));
    var array2 = MA();
    var array3 = MA(N(4), N(5), N(6), N(7), N(8));
    var array4 = MA();
    MLInsertManyAt(array1, IA(N(5), N(6), N(7)), N(1));
    MLInsertManyAt(array2, IA(N(4), N(8)), N(0));
    MLInsertManyAt(array3, IA(), N(2));
    MLInsertManyAt(array4, IA(), N(0));
    MLAssertEquals(array1, IA(N(4), N(5), N(6), N(7), N(8)), "MutableArray -insert-many*at* inserts objects at the specified index");
    MLAssertEquals(array2, IA(N(4), N(8)), "MutableArray -insert-many*at* inserts 2 objects into an empty array at index 0");
    MLAssertEquals(array3, IA(N(4), N(5), N(6), N(7), N(8)), "MutableArray -insert-many*at* doesn't change the array when passing an empty array of objects");
    MLAssertEquals(array4, IA(), "MutableArray -insert-many*at* leaves the array empty when inserting no objects (empty array) into an empty array (at index 0)");
}


void MLTestMutableArrayInsertBefore() {
    var array1 = MA(N(4), N(6));
    var array2 = MA(N(6));
    var array3 = MA();
    var array4 = MA(N(4), N(5), N(6));
    MLInsertBefore(array1, N(5), N(6));
    MLInsertBefore(array2, N(4), N(9));
    MLInsertBefore(array3, N(6), N(9));
    MLInsertBefore(array4, N(5), N(4));
    MLAssertEquals(array1, IA(N(4), N(5), N(6)), "MutableArray -insert*before* inserts an object before a passed in object");
    MLAssertEquals(array2, IA(N(4), N(6)), "MutableArray -insert*before* inserts an object at the beginning if the before object is not contained");
    MLAssertEquals(array3, IA(N(6)), "MutableArray -insert*before* inserts an object into an empty array ignoring the passed in before object");
    MLAssertEquals(array4, IA(N(5), N(4), N(5), N(6)), "MutableArray -insert*before* inserts an object at the beginning if the before object is the first in the array");
}


void MLTestMutableArrayInsertManyBefore() {
    var array1 = MA(N(4), N(5), N(6));
    var array2 = MA();
    var array3 = MA(N(6));
    MLInsertManyBefore(array1, IA(N(4), N(5)), N(6));
    MLInsertManyBefore(array2, IA(N(6)), N(9));
    MLInsertManyBefore(array3, IA(N(4), N(5)), N(9));
    MLAssertEquals(array1, IA(N(4), N(5), N(4), N(5), N(6)), "MutableArray -insert-many*before* inserts many objects before the first occurence of the passed object");
    MLAssertEquals(array2, IA(N(6)), "MutableArray -insert-many*before* ignores the before object and inserts at the beginning if array is empty");
    MLAssertEquals(array3, IA(N(4), N(5), N(6)), "MutableArray -insert-many*before* inserts many objects at the beginning if before object isn't contained");
}


void MLTestMutableArrayInsertAfter() {
    var array1 = MA(N(6), N(4));
    var array2 = MA(N(6));
    var array3 = MA();
    var array4 = MA(N(6), N(5), N(4), N(5));
    MLInsertAfter(array1, N(5), N(6));
    MLInsertAfter(array2, N(4), N(9));
    MLInsertAfter(array3, N(6), N(9));
    MLInsertAfter(array4, N(6), N(5));
    MLAssertEquals(array1, IA(N(6), N(5), N(4)), "MutableArray -insert*after* inserts an object after a passed in object");
    MLAssertEquals(array2, IA(N(6), N(4)), "MutableArray -insert*after* inserts an object at the end if the after object is not contained");
    MLAssertEquals(array3, IA(N(6)), "MutableArray -insert*after* inserts an object into an empty array ignoring the passed in after object");
    MLAssertEquals(array4, IA(N(6), N(5), N(4), N(5), N(6)), "MutableArray -insert*after* inserts an object after the last occurence of the passed in object if after object is contained multiple times");
}


void MLTestMutableArrayInsertManyAfter() {
    // TODO: fix test descriptions.
    var array1 = MA();
    var array2 = MA(N(6));
    var array3 = MA(N(6), N(4), N(5));
    MLInsertManyAfter(array1, IA(N(6)), N(9));
    MLInsertManyAfter(array2, IA(N(4), N(5)), N(9));
    MLInsertManyAfter(array3, IA(N(4), N(5)), N(6));
    MLAssertEquals(array1, IA(N(6)), "[] insert-many [6] after 9 should change array to [6]");
    MLAssertEquals(array2, IA(N(6), N(4), N(5)), "[6] insert-many [4, 5] after 9 should change array to [6, 4, 5]");
    MLAssertEquals(array3, IA(N(6), N(4), N(5), N(4), N(5)), "[6, 4, 5] insert-many [4, 5] after 6 should change array to [6, 4, 5, 4, 5]");
}


void MLTestMutableArrayReplaceWith() {
    var array1 = MA(N(1), N(2), N(3), N(2), N(4));
    var array2 = MA(N(1));
    var array3 = MA();
    MLReplaceWith(array1, N(2), N(9));
    MLReplaceWith(array2, N(2), N(3));
    MLReplaceWith(array3, N(1), N(2));
    MLAssertEquals(array1, IA(N(1), N(9), N(3), N(9), N(4)), "MutableArray -replace*with* replaces all occurences of an object with another object");
    MLAssertEquals(array2, IA(N(1)), "MutableArray -replace*with* doesn't change the array if object to be replaced is not contained in the array");
    MLAssertEquals(array3, IA(), "MutableArray -replace*with* doesn't change the array if array is empty");
}


void MLTestMutableArrayReplaceWithMany() {
    var array1 = MA(N(1), N(2), N(3), N(2), N(4));
    var array2 = MA(N(1));
    var array3 = MA();
    MLReplaceWithMany(array1, N(2), IA(N(8), N(9)));
    MLReplaceWithMany(array2, N(2), IA(N(3)));
    MLReplaceWithMany(array3, N(1), IA(N(2)));
    MLAssertEquals(array1, IA(N(1), N(8), N(9), N(3), N(8),  N(9), N(4)), "MutableArray -replace*with-many* replaces all occurences of an object with an array of objects");
    MLAssertEquals(array2, IA(N(1)), "MutableArray -replace*with-many* doesn't change the array if object to be replaced is not contained in the array");
    MLAssertEquals(array3, IA(), "MutableArray -replace*with-many* doesn't change the array if array is empty");
}


void MLTestMutableArrayReplaceAtWith() {
    var array1 = MA(N(1), N(2), N(3), N(2), N(4));
    MLReplaceAtWith(array1, N(1), N(9));
    MLAssertEquals(array1, IA(N(1), N(9), N(3), N(2), N(4)), "MutableArray -replace-at*with* replaces object at the specified index  with another object");
}


void MLTestMutableArrayReplaceAtWithMany() {
    var array1 = MA(N(1), N(2), N(3), N(2), N(4));
    MLReplaceAtWithMany(array1, N(1), IA(N(8), N(9)));
    MLAssertEquals(array1, IA(N(1), N(8), N(9), N(3), N(2), N(4)), "MutableArray -replace-at*with-many* replaces object at the specified index  with many objects");
}


void MLTestMutableArrayReplaceAtCountWith() {
    var array1 = MA(N(1), N(2), N(3), N(4), N(5));
    MLReplaceAtCountWith(array1, N(1), N(3), N(9));
    MLAssertEquals(array1, IA(N(1), N(9), N(5)), "MutableArray -replace-at*count*with* replaces N objects at the specified index with another object");
}


void MLTestMutableArrayReplaceAtCountWithMany() {
    var array1 = MA(N(1), N(2), N(3), N(4), N(5));
    var array2 = MA();
    MLReplaceAtCountWithMany(array1, N(1), N(3), IA(N(7), N(8), N(9)));
    MLReplaceAtCountWithMany(array2, N(0), N(0), IA());
    MLAssertEquals(array1, IA(N(1), N(7), N(8), N(9), N(5)), "MutableArray -replace-at*count*with-many* replaces N objects at the specified index with other objects");
    MLAssertEquals(array2, IA(), "MutableArray -replace-at*count*with-many* doesn't change the array when count is 0 and replacements array is empty");
}


void MLTestMutableArrayRemove() {
    var array1 = MA(N(1), N(2), N(3), N(4), N(3));
    var array2 = MA(N(1), N(3), N(4), N(3));
    var array3 = MA(N(1), N(3), N(4), N(3));
    MLRemove(array1, N(2));
    MLRemove(array2, N(9));
    MLRemove(array3, N(3));
    MLAssertEquals(array1, IA(N(1), N(3), N(4), N(3)), "MutableArray -remove* removes an object from the array");
    MLAssertEquals(array2, IA(N(1), N(3), N(4), N(3)), "MutableArray -remove* doesn't modify the array if the passed object is not contained in it");
    MLAssertEquals(array3, IA(N(1), N(4)), "MutableArray -remove* removes all occurences of an object if it is contained multiple times");
}


void MLTestMutableArrayRemoveMany() {
    var array1 = MA(N(1), N(2), N(3), N(4), N(3));
    var array2 = MA(N(1), N(3), N(4), N(3));
    var array3 = MA(N(1), N(3), N(4), N(3));
    var array4 = MA(N(3), N(4), N(3));
    MLRemoveMany(array1, IA(N(2)));
    MLRemoveMany(array2, IA());
    MLRemoveMany(array3, IA(N(1), N(9)));
    MLRemoveMany(array4, IA(N(3)));
    MLAssertEquals(array1, IA(N(1), N(3), N(4), N(3)), "MutableArray -remove-many* removes each passed object from the array");
    MLAssertEquals(array2, IA(N(1), N(3), N(4), N(3)), "MutableArray -remove-many* doesn't modify the array if no objects were passed");
    MLAssertEquals(array3, IA(N(3), N(4), N(3)), "MutableArray -remove-many* ignores objects that are not contained in the array");
    MLAssertEquals(array4, IA(N(4)), "MutableArray -remove-many* removes all occurences of an object");
}


void MLTestMutableArrayRemoveAt() {
    // TODO: fix test descriptions.
    var array1 = MA(N(1), N(2), N(3), N(4));
    var array2 = MA(N(1), N(2), N(3));
    var array3 = MA(N(1), N(3));
    var array4 = MA(N(3));
    MLRemoveAt(array1, N(3));
    MLRemoveAt(array2, N(1));
    MLRemoveAt(array3, N(0));
    MLRemoveAt(array4, N(0));
    MLAssertEquals(array1, IA(N(1), N(2), N(3)), "Removing an object at the last index should remove the last object from the array");
    MLAssertEquals(array2, IA(N(1), N(3)), "Removing an object at some index inbetween should remove the object from the array");
    MLAssertEquals(array3, IA(N(3)), "Removing an object at index 0 should remove the first object from the array");
    MLAssertEquals(array4, IA(), "Removing an object at index 0 from an array with just one element should remove that object resulting in an empty array");
    MLAssertThrows(null, "MutableArray remove-at* throws if index < 0") MLRemoveAt(MA(N(1), N(2), N(3)), N(-1));
    MLAssertThrows(null, "MutableArray remove-at* throws if index = count") MLRemoveAt(MA(N(1), N(2), N(3)), N(3));
    MLAssertThrows(null, "MutableArray remove-at* throws if index > count") MLRemoveAt(MA(N(1), N(2), N(3)), N(4));
}


void MLTestMutableArrayRemoveAtMany() {
    var array = MA(N(1), N(2), N(3), N(4), N(5), N(6));
    MLRemoveAtMany(array, IA(N(1), N(3), N(5)));
    MLAssertEquals(array, IA(N(1), N(3), N(5)), "MutableArray -remove-at-many* removes the objects at the passed in indexes");
    MLAssertThrows(null, "MutableArray -remove-at-many* throws if one of the indexes is < 0") MLRemoveAtMany(MA(N(1), N(2), N(3)), IA(N(-1)));
    MLAssertThrows(null, "MutableArray -remove-at-many* throws if one of the indexes is = count") MLRemoveAtMany(MA(N(1), N(2), N(3)), IA(N(3)));
    MLAssertThrows(null, "MutableArray -remove-at-many* throws if one of the indexes is > count") MLRemoveAtMany(MA(N(1), N(2), N(3)), IA(N(4)));
}


void MLTestMutableArrayRemoveAtCount() {
    var array1 = MA(N(1), N(2), N(3), N(4), N(5));
    var array2 = MA(N(1), N(2), N(3), N(4), N(5));
    var array3 = MA(N(2), N(3), N(4), N(5));
    var array4 = MA(N(2), N(5));
    var array5 = MA(N(2), N(5));
    MLRemoveAtCount(array1, N(0), N(0));
    MLRemoveAtCount(array2, N(0), N(1));
    MLRemoveAtCount(array3, N(1), N(2));
    MLRemoveAtCount(array4, N(-2), N(2));
    MLRemoveAtCount(array5, N(2), N(2));
    MLAssertEquals(array1, IA(N(1), N(2), N(3), N(4), N(5)), "MutableArray -remove-at*count* doesn't change the array and ignores the index if count = 0");
    MLAssertEquals(array2, IA(N(2), N(3), N(4), N(5)), "MutableArray -remove-at*count* removes the first object if index = 0 and count = 1 and array contains at least 1 object");
    MLAssertEquals(array3, IA(N(2), N(5)), "MutableArray -remove-at*count* removes N objects starting at a given index");
    MLAssertEquals(array4, IA(N(2), N(5)), "MutableArray -remove-at*count* doesn't change the array when removing N objects at an index = -N");
    MLAssertEquals(array5, IA(N(2), N(5)), "MutableArray -remove-at*count* doesn't change the array when removing N objects at an index >= count, ignores N");
}


void MLTestMutableArrayRemoveAll() {
    var array = MA(N(1), N(2), N(3), N(4));
    MLRemoveAll(array);
    MLAssertEquals(array, IA(), "MutableArray -remove-all removes all objects from an array");
}


void MLTestMutableArrayReverse() {
    var array1 = MA(N(1), N(2), N(3));
    var array2 = MA(N(1));
    MLReverse(array1);
    MLReverse(array2);
    MLAssertEquals(array1, IA(N(3), N(2), N(1)), "MutableArray -reverse reorders the contained objects to be in reversed order");
    MLAssertEquals(array2, IA(N(1)), "MutableArray -reverse doesn't change the array if count = 1");
}


void MLTestMutableArraySort() {
    var array2 = MA(N(3), N(2), N(1), N(4), N(6), N(5));
    var array1 = MA(N(1));
    MLSort(array2);
    MLSort(array1);
    MLAssertEquals(array2, IA(N(1), N(2), N(3), N(4), N(5), N(6)), "MutableArray -sort sorts the array ascending");
    MLAssertEquals(array1, IA(N(1)), "MutableArray -sort doesn't change the array if count = 1");
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
