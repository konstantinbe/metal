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


void CRTestMutableArrayCreateMutable() {
    var array = CRArrayCreateMutable();
    CRAssert(CRArrayCount(array) == 0, "Count of an empty mutable array should be 0");
    CRAssert(CRArrayCapacity(array) >= 0, "Capacity of an empty mutable array should be at least 0");
    CRAssert(CRArrayIsMutable(array), "An empty mutable array should be mutable");
}


void CRTestMutableArrayCreateMutableWithCapacity() {
    var array = CRArrayCreateMutableWithCapacity(100);
    CRAssert(CRArrayCount(array) == 0, "Count of an empty mutable array created with capacity should be 0");
    CRAssert(CRArrayCapacity(array) >= 100, "Capacity of an empty mutable array created with capacity should be at least the passed in capacity");
    CRAssert(CRArrayIsMutable(array), "An empty mutable array created with capacity should be mutable");
}


void CRTestMutableArrayCreateMutableWithCArray() {
    var objects[] = {CRNumber(1), CRNumber(2), CRNumber(3)};
    var array = CRArrayCreateMutableWithCArray(3, objects);

    CRAssert(CRArrayCount(array) == 3, "Count of an mutable array created with C array should be 0");
    CRAssert(CRArrayCapacity(array) >= 3, "Capacity of an mutable array created with C array should be at least its count");
    CRAssert(CRArrayIsMutable(array), "An mutable array created with C array should be mutable");

    CRAssert(CREquals(CRArrayObjectAt(array, 0), objects[0]), "A mutable array created with a C array should have the same object at index 0 as the C array");
    CRAssert(CREquals(CRArrayObjectAt(array, 1), objects[1]), "A mutable array created with a C array should have the same object at index 1 as the C array");
    CRAssert(CREquals(CRArrayObjectAt(array, 2), objects[2]), "A mutable array created with a C array should have the same object at index 2 as the C array");
}


// -----------------------------------------------------------------------------


void CRTestMutableArrayAdd() {
    var numbers = CRArrayCreateMutable();
    var strings = CRArrayCreateMutable();

    CRArrayAdd(numbers, CRNumber(1));
    CRAssert(CRArrayCount(numbers) == 1, "After adding number 1 to array [] its count should be 1");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 0), CRNumber(1)), "After adding number 1 to array [] it should have 1 at index 0");

    CRArrayAdd(numbers, CRNumber(2));
    CRAssert(CRArrayCount(numbers) == 2, "After adding number 2 to array [1] its count should be 2");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 0), CRNumber(1)), "After adding number 2 to array [1] it should have 1 at index 0");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 1), CRNumber(2)), "After adding number 2 to array [1] it should have 2 at index 1");

    CRArrayAdd(numbers, CRNumber(3));
    CRAssert(CRArrayCount(numbers) == 3, "After adding number 3 to array [1, 2] its count should be 3");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 0), CRNumber(1)), "After adding number 3 to array [1, 2] it should have 1 at index 0");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 1), CRNumber(2)), "After adding number 3 to array [1, 2] it should have 2 at index 1");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 2), CRNumber(3)), "After adding number 3 to array [1, 2] it should have 3 at index 2");

    var string = CRStringCreateWithCharacters("Hello World!");
    const CRNatural retain_count_before_adding = CRRetainCount(string);
    CRArrayAdd(strings, string);
    CRAssert(CRRetainCount(string) == retain_count_before_adding + 1, "Adding an object to an array should retain the object");
    CRRelease(string);

    CRRelease(numbers);
    CRRelease(strings);
}


void CRTestMutableArrayAddMany() {
    var numbers = CRArrayCreateMutable();
    var strings = CRArrayCreateMutable();

    CRArrayAddMany(numbers, CRArray());
    CRAssert(CRArrayCount(numbers) == 0, "After adding [] to array [] its count should be 0");

    CRArrayAddMany(numbers, CRArray(CRNumber(1)));
    CRAssert(CRArrayCount(numbers) == 1, "After adding [1] to array [] its count should be 1");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 0), CRNumber(1)), "After adding number [1] to array [] it should have 1 at index 0");

    CRArrayAddMany(numbers, CRArray(CRNumber(2), CRNumber(3), CRNumber(4)));
    CRAssert(CRArrayCount(numbers) == 4, "After adding [2, 3, 4] to array [1] its count should be 4");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 0), CRNumber(1)), "After adding [2, 3, 4] to array [1] it should have 1 at index 0");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 1), CRNumber(2)), "After adding [2, 3, 4] to array [1] it should have 2 at index 1");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 2), CRNumber(3)), "After adding [2, 3, 4] to array [1] it should have 3 at index 2");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 3), CRNumber(4)), "After adding [2, 3, 4] to array [1] it should have 4 at index 3");

    CRArrayAddMany(numbers, CRArray());
    CRAssert(CRArrayCount(numbers) == 4, "After adding [] to array [1, 2, 3, 4] its count should be 4");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 0), CRNumber(1)), "After adding [] to array [1, 2, 3, 4] it should have 1 at index 0");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 1), CRNumber(2)), "After adding [] to array [1, 2, 3, 4] it should have 2 at index 1");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 2), CRNumber(3)), "After adding [] to array [1, 2, 3, 4] it should have 3 at index 2");
    CRAssert(CREquals(CRArrayObjectAt(numbers, 3), CRNumber(4)), "After adding [] to array [1, 2, 3, 4] it should have 4 at index 3");

    var string = CRStringCreateWithCharacters("Hello World!");
    const CRNatural retain_count_before_adding = CRRetainCount(string);
    CRArrayAddMany(strings, CRArray(string));
    CRAssert(CRRetainCount(string) == retain_count_before_adding + 1, "Adding many objects to an array should retain the objects");
    CRRelease(string);

    CRRelease(numbers);
    CRRelease(strings);
}


void CRTestMutableArrayInsertAt() {
    var numbers = CRArrayCreateMutable();
    var strings = CRArrayCreateMutable();

    CRArrayAdd(numbers, CRNumber(1));
    CRArrayAdd(numbers, CRNumber(3));

    CRArrayInsertAt(numbers, CRNumber(0), 0);
    CRArrayInsertAt(numbers, CRNumber(2), 2);
    CRArrayInsertAt(numbers, CRNumber(4), 4);

    assert(CRArrayCount(numbers) == 5);
    assert(CREquals(CRArrayObjectAt(numbers, 0), CRNumber(0)));
    assert(CREquals(CRArrayObjectAt(numbers, 1), CRNumber(1)));
    assert(CREquals(CRArrayObjectAt(numbers, 2), CRNumber(2)));
    assert(CREquals(CRArrayObjectAt(numbers, 3), CRNumber(3)));
    assert(CREquals(CRArrayObjectAt(numbers, 4), CRNumber(4)));

    var string = CRStringCreateWithCharacters("Hello World!");
    const CRNatural retain_count_before_inserting = CRRetainCount(string);
    CRArrayInsertAt(strings, string, 0);
    CRAssert(CRRetainCount(string) == retain_count_before_inserting + 1, "Inserting an object into an array should retain the object");
    CRRelease(string);

    CRRelease(numbers);
    CRRelease(strings);
}


void CRTestMutableArrayInsertManyAt() {
    var numbers = CRArrayCreateMutable();
    var strings = CRArrayCreateMutable();

    CRArrayAdd(numbers, CRNumber(2));
    CRArrayAdd(numbers, CRNumber(6));

    CRArrayInsertManyAt(numbers, CRArray(CRNumber(0), CRNumber(1)), 0);
    assert(CRArrayCount(numbers) == 4);
    assert(CRArrayObjectAt(numbers, 0).payload.decimal == 0);
    assert(CRArrayObjectAt(numbers, 1).payload.decimal == 1);
    assert(CRArrayObjectAt(numbers, 2).payload.decimal == 2);
    assert(CRArrayObjectAt(numbers, 3).payload.decimal == 6);

    CRArrayInsertManyAt(numbers, CRArray(CRNumber(3), CRNumber(4), CRNumber(5)), 3);
    assert(CRArrayCount(numbers) == 7);
    assert(CRArrayObjectAt(numbers, 0).payload.decimal == 0);
    assert(CRArrayObjectAt(numbers, 1).payload.decimal == 1);
    assert(CRArrayObjectAt(numbers, 2).payload.decimal == 2);
    assert(CRArrayObjectAt(numbers, 3).payload.decimal == 3);
    assert(CRArrayObjectAt(numbers, 4).payload.decimal == 4);
    assert(CRArrayObjectAt(numbers, 5).payload.decimal == 5);
    assert(CRArrayObjectAt(numbers, 6).payload.decimal == 6);

    CRArrayInsertManyAt(numbers, CRArray(CRNumber(7), CRNumber(8), CRNumber(9)), 7);
    assert(CRArrayCount(numbers) == 10);
    assert(CRArrayObjectAt(numbers, 0).payload.decimal == 0);
    assert(CRArrayObjectAt(numbers, 1).payload.decimal == 1);
    assert(CRArrayObjectAt(numbers, 2).payload.decimal == 2);
    assert(CRArrayObjectAt(numbers, 3).payload.decimal == 3);
    assert(CRArrayObjectAt(numbers, 4).payload.decimal == 4);
    assert(CRArrayObjectAt(numbers, 5).payload.decimal == 5);
    assert(CRArrayObjectAt(numbers, 6).payload.decimal == 6);
    assert(CRArrayObjectAt(numbers, 7).payload.decimal == 7);
    assert(CRArrayObjectAt(numbers, 8).payload.decimal == 8);
    assert(CRArrayObjectAt(numbers, 9).payload.decimal == 9);

    var string = CRStringCreateWithCharacters("Hello World!");
    const CRNatural retain_count_before_inserting = CRRetainCount(string);
    CRArrayInsertManyAt(strings, CRArray(string), 0);
    CRAssert(CRRetainCount(string) == retain_count_before_inserting + 1, "Inserting many objects into an array should retain the objects");
    CRRelease(string);

    CRRelease(numbers);
    CRRelease(strings);
}


void CRTestMutableArrayRemove() {
    var numbers = CRArrayCreateMutable();
    var strings = CRArrayCreateMutable();

    CRArrayAddMany(numbers, CRArray(CRNumber(0), CRNumber(1), CRNumber(2)));
    CRArrayRemove(numbers, CRNumber(1));
    assert(CRArrayCount(numbers) == 2);
    assert(CRArrayObjectAt(numbers, 0).payload.decimal == 0);
    assert(CRArrayObjectAt(numbers, 1).payload.decimal == 2);

    var string = CRStringCreateWithCharacters("Hello World!");
    CRArrayAdd(strings, string);
    const CRNatural retain_count_before_removing = CRRetainCount(string);
    CRArrayRemove(strings, string);
    CRAssert(CRRetainCount(string) == retain_count_before_removing - 1, "Removing an object from an array should release the object");
    CRRelease(string);

    CRRelease(numbers);
    CRRelease(strings);
}


void CRTestMutableArrayRemoveMany() {
    var numbers = CRArrayCreateMutable();
    var strings = CRArrayCreateMutable();

    CRArrayAdd(numbers, CRNumber(100));
    CRArrayAdd(numbers, CRNumber(200));
    CRArrayAdd(numbers, CRNumber(300));
    CRArrayAdd(numbers, CRNumber(400));
    CRArrayAdd(numbers, CRNumber(100));
    assert(CRArrayCount(numbers) == 5);

    CRArrayRemoveMany(numbers, CRArray(CRNumber(100), CRNumber(300)));
    assert(CRArrayCount(numbers) == 2);
    assert(CRArrayObjectAt(numbers, 0).payload.decimal == 200);
    assert(CRArrayObjectAt(numbers, 1).payload.decimal == 400);

    var string = CRStringCreateWithCharacters("Hello World!");
    CRArrayAdd(strings, string);
    const CRNatural retain_count_before_removing = CRRetainCount(string);
    CRArrayRemoveMany(strings, CRArray(string));
    CRAssert(CRRetainCount(string) == retain_count_before_removing - 1, "Removing many objects from an array should release the objects");
    CRRelease(string);

    CRRelease(numbers);
    CRRelease(strings);
}


void CRTestMutableArrayRemoveAt() {
    var numbers = CRArrayCreateMutable();
    var strings = CRArrayCreateMutable();

    CRArrayAdd(numbers, CRNumber(0));
    CRArrayAdd(numbers, CRNumber(1));
    CRArrayAdd(numbers, CRNumber(2));
    CRArrayAdd(numbers, CRNumber(3));
    CRArrayAdd(numbers, CRNumber(4));

    CRArrayRemoveAt(numbers, 4);
    CRArrayRemoveAt(numbers, 2);
    CRArrayRemoveAt(numbers, 0);

    assert(CRArrayCount(numbers) == 2);
    assert(CRArrayObjectAt(numbers, 0).payload.decimal == 1);
    assert(CRArrayObjectAt(numbers, 1).payload.decimal == 3);

    var string = CRStringCreateWithCharacters("Hello World!");
    CRArrayAdd(strings, string);
    const CRNatural retain_count_before_removing = CRRetainCount(string);
    CRArrayRemoveAt(strings, 0);
    CRAssert(CRRetainCount(string) == retain_count_before_removing - 1, "Removing an object at a specific index from an array should release the object");
    CRRelease(string);

    CRRelease(numbers);
    CRRelease(strings);
}


void CRTestMutableArrayRemoveManyAt() {
    var numbers = CRArrayCreateMutable();
    var strings = CRArrayCreateMutable();

    CRArrayAddMany(numbers, CRArray(CRNumber(0), CRNumber(1), CRNumber(2), CRNumber(3), CRNumber(4), CRNumber(5), CRNumber(6), CRNumber(7), CRNumber(8), CRNumber(9)));
    assert(CRArrayCount(numbers) == 10);

    CRArrayRemoveManyAt(numbers, CRArray(CRNumber(8), CRNumber(7), CRNumber(9)));
    assert(CRArrayCount(numbers) == 7);
    assert(CRArrayObjectAt(numbers, 0).payload.decimal == 0);
    assert(CRArrayObjectAt(numbers, 1).payload.decimal == 1);
    assert(CRArrayObjectAt(numbers, 2).payload.decimal == 2);
    assert(CRArrayObjectAt(numbers, 3).payload.decimal == 3);
    assert(CRArrayObjectAt(numbers, 4).payload.decimal == 4);
    assert(CRArrayObjectAt(numbers, 5).payload.decimal == 5);
    assert(CRArrayObjectAt(numbers, 6).payload.decimal == 6);

    CRArrayRemoveManyAt(numbers, CRArray(CRNumber(5), CRNumber(3), CRNumber(3), CRNumber(4)));
    assert(CRArrayCount(numbers) == 4);
    assert(CRArrayObjectAt(numbers, 0).payload.decimal == 0);
    assert(CRArrayObjectAt(numbers, 1).payload.decimal == 1);
    assert(CRArrayObjectAt(numbers, 2).payload.decimal == 2);
    assert(CRArrayObjectAt(numbers, 3).payload.decimal == 6);

    var string = CRStringCreateWithCharacters("Hello World!");
    CRArrayAdd(strings, string);
    const CRNatural retain_count_before_removing = CRRetainCount(string);
    CRArrayRemoveManyAt(strings, CRArray(CRNumber(0)));
    CRAssert(CRRetainCount(string) == retain_count_before_removing - 1, "Removing many objects at a specific indexes from an array should release the objects");
    CRRelease(string);

    CRRelease(numbers);
    CRRelease(strings);
}


void CRTestMutableArrayClear() {
    var strings = CRArrayCreateMutable();
    var string1 = CRStringCreateWithCharacters("String 1");
    var string2 = CRStringCreateWithCharacters("String 2");

    CRArrayAddMany(strings, CRArray(string1, string2));

    const CRNatural retain_count_1 = CRRetainCount(string1);
    const CRNatural retain_count_2 = CRRetainCount(string2);

    CRArrayClear(strings);

    CRAssert(CRArrayCount(strings) == 0, "Clearing an array removes all objects, count should be 0");
    CRAssert(CRRetainCount(string1) == retain_count_1 - 1, "Clearing an array releases all contained objects");
    CRAssert(CRRetainCount(string2) == retain_count_2 - 1, "Clearing an array releases all contained objects");

    CRRelease(strings);
    CRRelease(string1);
    CRRelease(string2);
}


void CRTestMutableArray() {
    CRTestMutableArrayCreateMutable();
    CRTestMutableArrayCreateMutableWithCapacity();
    CRTestMutableArrayCreateMutableWithCArray();

    CRTestMutableArrayAdd();
    CRTestMutableArrayAddMany();

    CRTestMutableArrayInsertAt();
    CRTestMutableArrayInsertManyAt();

    CRTestMutableArrayRemove();
    CRTestMutableArrayRemoveMany();

    CRTestMutableArrayRemoveAt();
    CRTestMutableArrayRemoveManyAt();

    CRTestMutableArrayClear();
}
