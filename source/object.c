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

#include "object.h"
#include "object-private.h"

#include "class.h"
#include "class-private.h"

#include "macros.h"
#include "string.h"


// --------------------------------------------------------------- Macros ------


#define this ((struct CRObject*)self.pointer)
#define that (*this)

#define CRObjectThrowErrorIfNull() if (this == NULL) CRError("self is null")
#define CRObjectThrowErrorIfNotObject() if (that.class != CRObject.pointer) CRError("self is not an object")


// -------------------------------------------------- Constants & Globals ------


const var CRObject = {&CRObjectClass};


// -------------------------------------------------------------- Private ------


struct CRClass CRObjectMetaClass = {.class = &CRObjectMetaClass, .callbacks = &CRObjectMetaCallbacks};
struct CRClass CRObjectClass = {.class = &CRObjectMetaClass, .callbacks = &CRObjectCallbacks};


struct CRCallbacks CRObjectMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


struct CRCallbacks CRObjectCallbacks = {
    &CRObjectHash,
    &CRObjectEquals,
    &CRObjectCopy,
    &CRObjectMutableCopy,
    &CRObjectDestroy,
    &CRObjectDescription
};


CRNatural64 CRObjectHash(var self) {
    CRObjectThrowErrorIfNull();
    CRObjectThrowErrorIfNotObject();
    return (CRNatural64)self.pointer;
}


bool CRObjectEquals(var self, var other) {
    CRObjectThrowErrorIfNull();
    CRObjectThrowErrorIfNotObject();
    return self.pointer == other.pointer;
}


var CRObjectCopy(var self) {
    CRObjectThrowErrorIfNull();
    CRObjectThrowErrorIfNotObject();
    CRError("Can't create a copy of CRObject");
    return null;
}


var CRObjectMutableCopy(var self) {
    CRObjectThrowErrorIfNull();
    CRObjectThrowErrorIfNotObject();
    CRError("Can't create a mutable copy of CRObject");
    return null;
}


void CRObjectDestroy(var self) {
    CRObjectThrowErrorIfNull();
    CRObjectThrowErrorIfNotObject();
    if (self.pointer) CRFree(self.pointer);
}


var CRObjectDescription(var self) {
    CRObjectThrowErrorIfNull();
    CRObjectThrowErrorIfNotObject();

    const struct CRObject* instance = (struct CRObject*)self.pointer;
    const struct CRClass* class = instance->class;
    const CRNatural retain_count = instance->retain_count;

    const CRNatural characters_capacity = 1000;
    CRCharacter characters[characters_capacity];
    snprintf(characters, characters_capacity, "<CRObject %p, class: %p, retain_count: %lu>", instance, class, retain_count);

    var description = CRStringCreate(characters);
    return CRAutorelease(description);
}
