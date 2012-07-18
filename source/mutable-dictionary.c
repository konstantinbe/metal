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

#include "mutable-dictionary.h"
#include "mutable-dictionary-private.h"

#include "dictionary.h"
#include "dictionary-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "macros.h"


// --------------------------------------------------------------- Macros ------


#define this ((struct CRDictionary*)self.pointer)
#define that (*this)

#define CRMutableDictionaryThrowErrorIfZero() if (this == NULL) CRError("self is zero")
#define CRMutableDictionaryThrowErrorIfNotDictionary() if (that.class != CRMutableDictionary.pointer) CRError("self is not a mutable dictionary")


// -------------------------------------------------- Constants & Globals ------


const var CRMutableDictionary = {&CRMutableDictionaryClass};


// ---------------------------------------- Creating Mutable Dictionaries ------


var CRDictionaryCreateMutable() {
    var self = CRReference(CRAllocate(sizeof(struct CRDictionary)));

    that.class = &CRMutableDictionaryClass;
    that.retain_count = 1;
    that.mask = 0;
    that.count = 0;
    that.entries = NULL;

    // TODO: implement.

    return self;
}


var CRDictionaryCreateMutableWithCapacity(var capacity) {
    // TODO: implement.
    return zero;
}


// ------------------------------------------------ Mutating Dictionaries ------


void CRDictionaryPut(var self, var key, var object) {

}


void CRDictionaryPutMany(var self, var entries) {
    CRMutableDictionaryThrowErrorIfZero();
    CRMutableDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
}


void CRDictionaryRemove(var self, var key) {
    CRMutableDictionaryThrowErrorIfZero();
    CRMutableDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
}


void CRDictionaryRemoveMany(var self, var keys) {
    CRMutableDictionaryThrowErrorIfZero();
    CRMutableDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
}


void CRDictionaryClear(var self) {
    CRMutableDictionaryThrowErrorIfZero();
    CRMutableDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
}


void CRDictionaryIncreaseCapacityToAtLeast(var self, CRNatural capacity) {
    CRMutableDictionaryThrowErrorIfZero();
    CRMutableDictionaryThrowErrorIfNotDictionary();
    // TODO: implement.
}


// -------------------------------------------------------------- Private ------


struct CRClass CRMutableDictionaryClass = {.class = &CRMutableDictionaryMetaClass, .callbacks = &CRMutableDictionaryCallbacks};
struct CRClass CRMutableDictionaryMetaClass = {.class = &CRMutableDictionaryMetaClass, .callbacks = &CRMutableDictionaryMetaCallbacks};


struct CRCallbacks CRMutableDictionaryCallbacks = {
    &CRDictionaryHash,
    &CRDictionaryEquals,
    &CRDictionaryCopy,
    &CRDictionaryMutableCopy,
    &CRDictionaryDestroy,
    &CRDictionaryDescription
};


struct CRCallbacks CRMutableDictionaryMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};
