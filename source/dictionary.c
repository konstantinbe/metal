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

#include "dictionary.h"
#include "dictionary-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "mutable-dictionary.h"
#include "mutable-dictionary-private.h"

#include "macros.h"
#include <string.h>


// --------------------------------------------------------------- Macros ------


#define this ((struct CRDictionary*)self.pointer)
#define that (*this)

#define CRDictionaryThrowErrorIfZero() if (this == NULL) CRError("self is zero")
#define CRDictionaryThrowErrorIfNotDictionary() if (that.class != CRDictionary.pointer && that.class != CRMutableDictionary.pointer) CRError("self is not a dictionary")


// -------------------------------------------------- Constants & Globals ------


const var CRDictionary = {&CRDictionaryClass};


// ------------------------------------------------ Creating Dictionaries ------


var CRDictionaryMake(struct CRDictionary* dictionary, CRNatural count, var* entries) {
    var self = CRReference(dictionary);

    that.class = &CRDictionaryClass;
    that.retain_count = CRRetainCountMax;
    that.mask = 0;
    that.count = count;
    that.entries = entries;

    return self;
}


var CRDictionaryCreate() {
    return CRDictionaryCreateWithCArray(0, NULL);
}


var CRDictionaryCreateWithCArray(CRNatural count, var* entries) {
    return count < CRDictionaryLinearVsHashedThreshold ? CRDictionaryCreateWithCArrayLinear(count, entries) : CRDictionaryCreateWithCArrayHashed(count, entries);
}


var CRDictionaryCreateWithCArrayLinear(CRNatural count, var* entries) {
    var self = CRReference(CRAllocate(sizeof(struct CRDictionary)));

    that.class = &CRDictionaryClass;
    that.retain_count = 1;
    that.mask = 0;
    that.count = count;
    that.entries = CRAllocate(sizeof(var) * count * 2);

    memcpy(that.entries, entries, sizeof(var) * count * 2);
    for (CRInteger i = 0; i < count * 2; i += 1) CRRetain(entries[i]);

    return self;
}


var CRDictionaryCreateWithCArrayHashed(CRNatural count, var* entries) {
    // TODO: implement.
    return zero;
}


// ----------------------------------------------------------- Properties ------


CRNatural CRDictionaryMask(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    return that.mask;
}


CRNatural CRDictionaryCapacity(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();

    const CRNatural mask = that.mask;
    const CRNatural count = that.count;
    return mask > 0 ? mask + 1 : count;
}


CRNatural CRDictionaryCount(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    return that.count;
}


var* CRDictionaryEntries(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    return that.entries;
}


bool CRDictionaryIsEmpty(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    return that.count <= 0;
}


bool CRDictionaryIsLinear(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    return that.mask == 0 && that.count > 0;
}


bool CRDictionaryIsHashed(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    return that.mask != 0 || that.count == 0;
}


// --------------------------------------------------- Accessing Elements ------


var CRDictionaryKeyAt(var self, CRInteger index) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();

    if (index < 0 || index >= CRDictionaryCapacity(self)) CRError("Index out of bounds");
    return that.entries[index * 2];
}


var CRDictionaryObjectAt(var self, CRInteger index) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();

    if (index < 0 || index >= CRDictionaryCapacity(self)) CRError("Index out of bounds");
    return that.entries[index * 2 + 1];
}


CRInteger CRDictionaryIndexOf(var self, var key) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
    return -1;
}


var CRDictionaryGet(var self, var key) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
    return zero;
}


var CRDictionaryGetMany(var self, var keys) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
    return zero;
}


// ---------------------------------------------------------------- Other ------


bool CRDictionaryContains(var self, var key) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
    return false;
}


// -------------------------------------------------------------- Private ------


struct CRClass CRDictionaryClass = {.class = &CRDictionaryMetaClass, .callbacks = &CRDictionaryCallbacks};
struct CRClass CRDictionaryMetaClass = {.class = &CRDictionaryMetaClass, .callbacks = &CRDictionaryMetaCallbacks};


struct CRCallbacks CRDictionaryCallbacks = {
    &CRDictionaryHash,
    &CRDictionaryEquals,
    &CRDictionaryCopy,
    &CRDictionaryMutableCopy,
    &CRDictionaryDestroy,
    &CRDictionaryDescription
};


struct CRCallbacks CRDictionaryMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


CRNatural64 CRDictionaryHash(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    return (CRNatural64)self.pointer;
}


bool CRDictionaryEquals(var self, var other) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
    return false;
}


var CRDictionaryCopy(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
    return zero;
}


var CRDictionaryMutableCopy(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
    return zero;
}


void CRDictionaryDestroy(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
}


var CRDictionaryDescription(var self) {
    CRDictionaryThrowErrorIfZero();
    CRDictionaryThrowErrorIfNotDictionary();
    var description = zero;
    // TODO: implement.
    return CRAutorelease(description);
}
