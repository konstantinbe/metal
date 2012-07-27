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
// THE SOFTWARE IS PROVvarED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "array.h"
#include "array-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "macros.h"
#include "mutable-array.h"

#include "string.h"
#include <string.h>


// --------------------------------------------------------------- Macros ------


#define this ((struct CRArray*)self.pointer)
#define that (*this)

#define CRArrayThrowErrorIfNull() if (this == NULL) CRError("self is null")
#define CRArrayThrowErrorIfNotArray() if (that.class != CRArray.pointer && that.class != CRMutableArray.pointer) CRError("self is not an array")


// -------------------------------------------------- Constants & Globals ------


const var CRArray = {&CRArrayClass};


// ------------------------------------------------------ Creating Arrays ------


var CRArrayMake(struct CRArray* array, CRNatural count, var* objects) {
    var self = CRReference(array);
    that.class = &CRArrayClass;
    that.retain_count = CRRetainCountMax;
    that.capacity = count;
    that.count = count;
    that.objects = objects;
    return self;
}


var CRArrayCreate() {
    return CRArrayCreateWithCArray(0, NULL);
}


var CRArrayCreateWithCArray(CRNatural count, var* objects) {
    var self = CRReference(CRAllocate(sizeof(struct CRArray)));
    that.class = &CRArrayClass;
    that.retain_count = 1;
    that.capacity = count;
    that.count = count;
    that.objects = CRAllocate(sizeof(var) * count);

    memcpy(that.objects, objects, sizeof(var) * count);
    for (CRInteger i = 0; i < count; i += 1) CRRetain(objects[i]);

    return self;
}


// ----------------------------------------------------- Array Properties ------


CRNatural CRArrayCapacity(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    return that.capacity;
}


CRNatural CRArrayCount(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    return that.count;
}


var* CRArrayObjects(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    return that.objects;
}


bool CRArrayIsEmpty(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    return that.count <= 0;
}


bool CRArrayIsMutable(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    return that.class == CRMutableArray.pointer;
}


// ------------------------------------------------------ Querying Arrays ------


var CRArrayObjectAt(var self, CRInteger index) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    if (index < 0 || index >= that.count) CRError("Index out of bounds");
    return that.objects[index];
}


CRInteger CRArrayIndexOf(var self, var object) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    const CRNatural count = that.count;

    CRInteger index = -1;
    for (CRInteger i = 0; i < count; i += 1) {
        var objectAtIndex = CRArrayObjectAt(self, i);
        if (CREquals(objectAtIndex, object)) {
            index = i;
            break;
        }
    }

    return index;
}


bool CRArrayContains(var self, var object) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    return CRArrayIndexOf(self, object) >= 0;
}


// -------------------------------------------------------------- Private ------


struct CRClass CRArrayClass = {.class = &CRArrayMetaClass, .callbacks = &CRArrayCallbacks};
struct CRClass CRArrayMetaClass = {.class = &CRArrayMetaClass, .callbacks = &CRArrayMetaCallbacks};


struct CRCallbacks CRArrayCallbacks = {
    &CRArrayHash,
    &CRArrayEquals,
    &CRArrayCopy,
    &CRArrayMutableCopy,
    &CRArrayDestroy,
    &CRArrayDescription
};


struct CRCallbacks CRArrayMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


CRNatural64 CRArrayHash(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    return (CRNatural64)self.pointer;
}


bool CRArrayEquals(var self, var other) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();

    const CRNatural count = that.count;
    if (count != CRArrayCount(other)) return false;

    for (CRInteger index = 0; index < count; index += 1) {
        const var object = that.objects[index];
        const var other_object = CRArrayObjectAt(other, index);
        if (CREquals(object, other_object) == false) return false;
    }

    return true;
}


var CRArrayCopy(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    return CRArrayIsMutable(self) ? CRArrayCreateWithCArray(that.count, that.objects) : CRRetain(self);
}


var CRArrayMutableCopy(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();
    return CRArrayCreateMutableWithCArray(CRArrayCount(self), CRArrayObjects(self));
}


void CRArrayDestroy(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();

    var* objects = that.objects;
    for (CRInteger i = 0; i < CRArrayCount(self); i += 1) {
        CRRelease(CRArrayObjectAt(self, i));
    }

    that.class = NULL;
    that.retain_count = 0;
    that.capacity = 0;
    that.count = 0;
    that.objects = NULL;

    CRFree(objects);
    CRFree(this);
}


var CRArrayDescription(var self) {
    CRArrayThrowErrorIfNull();
    CRArrayThrowErrorIfNotArray();

    var description = CRStringCreateWithCharacters("[TODO: implement CRArrayDescription().]");
    return CRAutorelease(description);
}
