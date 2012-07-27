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

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "macros.h"
#include "string.h"


// --------------------------------------------------------------- Macros ------


#define this ((struct CRClass*)self.pointer)
#define that (*this)

#define CRClassThrowErrorIfNull() if (this == NULL) CRError("self is null")
#define CRClassThrowErrorIfNotClass() if (that.class != CRClass.pointer) CRError("self is not a class")


// -------------------------------------------------- Constants & Globals ------


const var CRClass = {&CRClassClass};


// -------------------------------------------------------------- Private ------


struct CRClass CRClassMetaClass = {.class = &CRClassMetaClass, .callbacks = &CRClassMetaCallbacks};
struct CRClass CRClassClass = {.class = &CRClassMetaClass, .callbacks = &CRClassCallbacks};


struct CRCallbacks CRClassMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


struct CRCallbacks CRClassCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


CRNatural64 CRClassHash(var self) {
    CRClassThrowErrorIfNull();
    CRClassThrowErrorIfNotClass();
    return (CRNatural64)self.pointer;
}


bool CRClassEquals(var self, var other) {
    CRClassThrowErrorIfNull();
    CRClassThrowErrorIfNotClass();
    return self.pointer == other.pointer;
}


var CRClassCopy(var self) {
    CRClassThrowErrorIfNull();
    CRClassThrowErrorIfNotClass();
    CRError("Can't create a copy of CRClass");
    return null;
}


var CRClassMutableCopy(var self) {
    CRClassThrowErrorIfNull();
    CRClassThrowErrorIfNotClass();
    CRError("Can't create a mutable copy of CRClass");
    return null;
}


void CRClassDestroy(var self) {
    CRClassThrowErrorIfNull();
    CRClassThrowErrorIfNotClass();

    // TODO: imeplement proper destroying of CRClass instances.
    if (self.pointer) CRFree(self.pointer);
}


var CRClassDescription(var self) {
    CRClassThrowErrorIfNull();
    CRClassThrowErrorIfNotClass();

    // TODO: implement proper description for CRClass instances.
    const struct CRObject* instance = (struct CRObject*)self.pointer;
    const struct CRClass* class = instance->class;

    const CRNatural retain_count = instance->retain_count;

    const CRNatural characters_capacity = 1000;
    CRCharacter characters[characters_capacity];
    snprintf(characters, characters_capacity, "<CRClass %p, class: %p, retain_count: %lu>", instance, class, retain_count);

    var description = CRStringCreate(characters);
    return CRAutorelease(description);
}
