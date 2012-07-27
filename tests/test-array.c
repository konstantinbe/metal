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
// THE SOFTWARE IS PROVvarED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "test.h"


void CRTestArrayMake() {
    const var array = CRArray(CRNumber(1), CRNumber(2), CRNumber(3));
    assert(array.pointer != NULL);

    const struct CRArray* instance = array.pointer;
    const var* objects = instance->objects;

    assert(instance != NULL);
    assert(instance->count == 3);

    assert(objects != NULL);
    assert(objects[0].payload.decimal == 1);
    assert(objects[1].payload.decimal == 2);
    assert(objects[2].payload.decimal == 3);
}


void CRTestArrayCreate() {
    const var array = CRArrayCreate();
    assert(CRArrayCount(array) == 0);
    assert(CRArrayCapacity(array) == 0);
}


void CRTestArrayCreateWithCArray() {
    var objects_array[] = {CRNumber(1), CRNumber(2), CRNumber(3)};
    const var array = CRArrayCreateWithCArray(3, objects_array);
    assert(array.pointer != NULL);

    const struct CRArray* instance = array.pointer;
    const var* objects = instance->objects;

    assert(instance != NULL);
    assert(instance->count == 3);
    assert(instance->objects != objects_array);

    assert(objects != NULL);
    assert(objects[0].payload.decimal == 1);
    assert(objects[1].payload.decimal == 2);
    assert(objects[2].payload.decimal == 3);
}



// -----------------------------------------------------------------------------


void CRTestArrayCapacity() {
    var empty_array = CRArrayCreate();
    assert(CRArrayCapacity(empty_array) == 0);

    var inline_array = CRArray(CRNumber(1), CRNumber(2));
    assert(CRArrayCapacity(inline_array) == 2);

    var objects_array[] = {CRNumber(1), CRNumber(2), CRNumber(3)};
    const var array = CRArrayCreateWithCArray(3, objects_array);
    assert(CRArrayCapacity(array) == 3);
}


void CRTestArrayCount() {
    var empty_array = CRArrayCreate();
    assert(CRArrayCount(empty_array) == 0);

    var inline_array = CRArray(CRNumber(1), CRNumber(2));
    assert(CRArrayCount(inline_array) == 2);

    var objects_array[] = {CRNumber(1), CRNumber(2), CRNumber(3)};
    const var array = CRArrayCreateWithCArray(3, objects_array);
    assert(CRArrayCount(array) == 3);
}


void CRTestArrayContains() {
    var array = CRArray(CRNumber(1), CRNumber(2), CRNumber(4), CRNumber(5));

    assert(CRArrayContains(array, CRNumber(0)) == false);
    assert(CRArrayContains(array, CRNumber(3)) == false);
    assert(CRArrayContains(array, CRNumber(6)) == false);

    assert(CRArrayContains(array, CRNumber(1)) == true);
    assert(CRArrayContains(array, CRNumber(2)) == true);
    assert(CRArrayContains(array, CRNumber(4)) == true);
    assert(CRArrayContains(array, CRNumber(5)) == true);
}


void CRTestArrayIsEmpty() {
    var empty_array = CRArrayCreate();
    assert(CRArrayIsEmpty(empty_array) == true);

    var array = CRArray(CRNumber(1));
    assert(CRArrayIsEmpty(array) == false);
}


void CRTestArrayIsMutable() {
    var array = CRArrayCreate();
    assert(!CRArrayIsMutable(array));

    var mutable_array = CRArrayCreateMutable();
    assert(CRArrayIsMutable(mutable_array));
}


// -----------------------------------------------------------------------------


void CRTestArrayObjectAt() {
    var array = CRArray(CRNumber(0), CRNumber(1), CRNumber(2), CRNumber(3), CRNumber(4));
    assert(CRArrayObjectAt(array, 0).payload.decimal == 0);
    assert(CRArrayObjectAt(array, 1).payload.decimal == 1);
    assert(CRArrayObjectAt(array, 2).payload.decimal == 2);
    assert(CRArrayObjectAt(array, 3).payload.decimal == 3);
    assert(CRArrayObjectAt(array, 4).payload.decimal == 4);
}


void CRTestArrayIndexOf() {
    var array = CRArray(CRNumber(1), CRNumber(2), CRNumber(4), CRNumber(5));

    assert(CRArrayIndexOf(array, CRNumber(0)) == -1);
    assert(CRArrayIndexOf(array, CRNumber(3)) == -1);
    assert(CRArrayIndexOf(array, CRNumber(6)) == -1);

    assert(CRArrayIndexOf(array, CRNumber(1)) == 0);
    assert(CRArrayIndexOf(array, CRNumber(2)) == 1);
    assert(CRArrayIndexOf(array, CRNumber(4)) == 2);
    assert(CRArrayIndexOf(array, CRNumber(5)) == 3);
}


// -----------------------------------------------------------------------------


void CRTestArrayHash() {
    var array = CRArray();
    CRAssert(CRHash(array) == (CRNatural64)array.pointer, "Hash of an array should be its memory address");
}


void CRTestArrayEquals() {
    const var empty_array_1 = CRArray();
    const var empty_array_2 = CRArray();

    const var array_with_same_elements_1 = CRArray(CRNumber(1), CRNumber(2), CRNumber(3));
    const var array_with_same_elements_2 = CRArray(CRNumber(1), CRNumber(2), CRNumber(3));

    const var array_with_different_count_1 = CRArray(CRNumber(1), CRNumber(2));
    const var array_with_different_count_2 = CRArray(CRNumber(1), CRNumber(2), CRNumber(3));

    const var array_with_same_count_but_different_objects_1 = CRArray(CRNumber(1), CRNumber(2), CRNumber(3));
    const var array_with_same_count_but_different_objects_2 = CRArray(CRNumber(1), CRNumber(2), CRNumber(4));

    CRExpectObjectToBeEqualTo(empty_array_1, empty_array_2);
    CRExpectObjectToBeEqualTo(array_with_same_elements_1, array_with_same_elements_2);
    CRExpectObjectNotToBeEqualTo(array_with_different_count_1, array_with_different_count_2);
    CRExpectObjectNotToBeEqualTo(array_with_same_count_but_different_objects_1, array_with_same_count_but_different_objects_2);
}


void CRTestArrayCopy() {
    var objects[] = {CRNumber(1), CRNumber(2), CRNumber(3)};
    var array = CRArrayCreateWithCArray(3, objects);

    const CRNatural array_retain_count_before_copying = CRRetainCount(array);
    var copy_of_array = CRCopy(array);
    const CRNatural array_retain_count_after_copying = CRRetainCount(array);

    CRExpectObjectToBeIdenticalTo(array, copy_of_array);
    assert(array_retain_count_after_copying == array_retain_count_before_copying + 1);
}


void CRTestArrayMutableCopy() {
    var objects[] = {CRNumber(1), CRNumber(2), CRNumber(3)};

    var array = CRArrayCreateMutableWithCArray(3, objects);
    var copy_of_array = CRMutableCopy(array);

    CRExpectObjectNotToBeIdenticalTo(array, copy_of_array);
    CRExpectObjectToBeEqualTo(array, copy_of_array);
    CRAssert(CRArrayIsMutable(copy_of_array), "Mutable copy of an array should be mutable");
}


void CRTestArrayDestroy() {
    var objects[] = {CRNumber(1), CRNumber(2), CRNumber(3)};
    var array = CRArrayCreateWithCArray(3, objects);
    CRRelease(array);
}


void CRTestArrayDescription() {
    var description = CRDescription(CRArray());
    // TODO: implement.
}


void CRTestArray() {
    CRTestArrayMake();
    CRTestArrayCreate();
    CRTestArrayCreateWithCArray();
    CRTestArrayDestroy();

    CRTestArrayCount();
    CRTestArrayCapacity();
    CRTestArrayContains();
    CRTestArrayIsEmpty();

    CRTestArrayObjectAt();
    CRTestArrayIndexOf();

    CRTestArrayHash();
    CRTestArrayEquals();
    CRTestArrayCopy();
    CRTestArrayMutableCopy();
    CRTestArrayDestroy();
    CRTestArrayDescription();
}
