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


void CRTestDictionaryMake() {
    var key1 = CRString("Key 1");
    var key2 = CRString("Key 2");

    var value1 = CRNumber(1);
    var value2 = CRNumber(2);

    const var dictionary = CRDictionary(key1, value1, key2, value2);

    CRAssert(CRRetainCount(dictionary) == CRRetainCountMax, "Stack allocated dictionary should have a retain count set to max");
    CRAssert(CRDictionaryCount(dictionary) == 2, "Stack allocated dictionary with 2 key-value pairs should have a count of 2");
    CRAssert(CRDictionaryIsLinear(dictionary), "Stack allocated dictionary should be linear");

    CRAssert(CREquals(CRDictionaryKeyAt(dictionary, 0), key1), "First key should be 'Key 1'");
    CRAssert(CREquals(CRDictionaryObjectAt(dictionary, 0), value1), "First value should be number '1'");

    CRAssert(CREquals(CRDictionaryKeyAt(dictionary, 1), key2), "Second key should be 'Key 2'");
    CRAssert(CREquals(CRDictionaryObjectAt(dictionary, 1), value2), "Second value should be number '2'");
}


void CRTestDictionaryCreate() {
    var dictionary = CRDictionaryCreate();

    CRAssert(CRRetainCount(dictionary) == 1, "A newly created dictionary should have a retain count of 1");
    CRAssert(CRDictionaryCount(dictionary) == 0, "An empty dictionary should have 0 entries");
    CRAssert(CRDictionaryCapacity(dictionary) == 0, "An empty dictionary should have capacity for 0 entries");

    CRRelease(dictionary);
}


void CRTestDictionaryCreateWithCArray() {
    var key1 = CRStringCreateWithCharacters("Key 1");
    var key2 = CRStringCreateWithCharacters("Key 2");

    var value1 = CRNumber(1);
    var value2 = CRNumber(2);

    var entries[] = {key1, value1, key2, value2};
    var dictionary = CRDictionaryCreateWithCArray(2, entries);

    CRAssert(CRRetainCount(dictionary) == 1, "A newly created dictionary with C array should have a retain count of 1");
    CRAssert(CRDictionaryCount(dictionary) == 2, "An newly created dictionary with C array containing 2 entries should have a count of 2");
    CRAssert(CRDictionaryCapacity(dictionary) >= 2, "An newly created dictionary with C array containing 2 entries should have capacity for at least 2 entries");

    CRRelease(key1);
    CRRelease(key2);
    CRRelease(dictionary);
}


// -----------------------------------------------------------------------------


void CRTestDictionaryIsEmpty() {
    var empty = CRDictionaryCreate();
    var entries[] = {CRNumber(1), CRNumber(100)};
    var dictionary = CRDictionaryCreateWithCArray(1, entries);

    CRAssert(CRDictionaryIsEmpty(empty), "A dictionary without entries should be empty");
    CRAssert(!CRDictionaryIsEmpty(dictionary), "A dictionary with at least one entry should not be empty");

    CRRelease(empty);
    CRRelease(dictionary);
}


void CRTestDictionaryIsLinear() {
    var entries[] = {CRNumber(1), CRNumber(100), CRNumber(2), CRNumber(200)};
    var dictionary = CRDictionaryCreateWithCArrayLinear(2, entries);

    CRAssert(CRDictionaryIsLinear(dictionary), "A linear dictionary should be linear");
    CRAssert(!CRDictionaryIsHashed(dictionary), "A linear dictionary should not be hashed");
    CRAssert(CRDictionaryMask(dictionary) == 0, "The mask of a linear dictionary should always be 0");
    CRAssert(CRDictionaryCount(dictionary) == 2, "The count of a linear dictionary should be the number of contained entries");
    CRAssert(CRDictionaryCapacity(dictionary) == 2, "The count of a linear dictionary should always equal its capacity");

    CRRelease(dictionary);
}


void CRTestDictionaryIsHashed() {
    // TODO: implement.
}


void CRTestDictionaryKeyAt() {
    // TODO: implement.
}


void CRTestDictionaryObjectAt() {
    // TODO: implement.
}


void CRTestDictionaryIndexOf() {
    // TODO: implement.
}


void CRTestDictionaryGet() {
    // TODO: implement.
}


void CRTestDictionaryContains() {
    // TODO: implement.
}


// -----------------------------------------------------------------------------


void CRTestDictionaryHash() {
    // TODO: implement.
}


void CRTestDictionaryEquals() {
    // TODO: implement.
}


void CRTestDictionaryCopy() {
    // TODO: implement.
}


void CRTestDictionaryMutableCopy() {
    // TODO: implement.
}


void CRTestDictionaryDestroy() {
    // TODO: implement.
}


void CRTestDictionaryDescription() {
    // TODO: implement.
}


// -----------------------------------------------------------------------------


void CRTestDictionary() {
    CRTestDictionaryMake();
    CRTestDictionaryCreate();
    CRTestDictionaryCreateWithCArray();

    CRTestDictionaryIsEmpty();
    CRTestDictionaryIsLinear();
    CRTestDictionaryIsHashed();

    CRTestDictionaryKeyAt();
    CRTestDictionaryObjectAt();
    CRTestDictionaryIndexOf();
    CRTestDictionaryGet();
    CRTestDictionaryContains();

    CRTestDictionaryHash();
    CRTestDictionaryEquals();
    CRTestDictionaryCopy();
    CRTestDictionaryMutableCopy();
    CRTestDictionaryDestroy();
    CRTestDictionaryDescription();
}
