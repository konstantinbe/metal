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

#include "autorelease-pool.h"
#include "autorelease-pool-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "mutable-array.h"
#include "macros.h"
#include <string.h>


// --------------------------------------------------------------- Macros ------


#define this ((struct CRAutoreleasePool*)self.pointer)
#define that (*this)

#define CRAutoreleasePoolThrowErrorIfZero() if (this == NULL) CRError("self is zero")
#define CRAutoreleasePoolThrowErrorIfNotAutoreleasePool() if (that.class != CRAutoreleasePool.pointer) CRError("self is not an autorelease pool")


// -------------------------------------------------- Constants & Globals ------


const var CRAutoreleasePool = {&CRAutoreleasePoolClass};


// ------------------------------------------- Creating Autorelease Pools ------


var CRAutoreleasePoolCreate() {
    struct CRAutoreleasePool* autoreleasePool = CRAllocateAndClear(sizeof(struct CRAutoreleasePool));
    var self = CRReference(autoreleasePool);

    that.class = &CRAutoreleasePoolClass;
    that.retain_count = 1;
    that.objects = CRArrayCreateMutableWithCapacity(CRAutoreleasePoolMinCapacity);

    return self;
}


// ----------------------------------------------------------- Properties ------


var CRAutoreleasePoolObjects(var self) {
    CRAutoreleasePoolThrowErrorIfZero();
    CRAutoreleasePoolThrowErrorIfNotAutoreleasePool();
    const var objects = CRCopy(that.objects);
    return CRAutorelease(objects);
}


// ---------------------------------------------------------------- Other ------


void CRAutoreleasePoolAdd(var self, var object) {
    CRAutoreleasePoolThrowErrorIfZero();
    CRAutoreleasePoolThrowErrorIfNotAutoreleasePool();
    CRArrayAdd(that.objects, object);
}


void CRAutoreleasePoolDrain(var self) {
    CRAutoreleasePoolThrowErrorIfZero();
    CRAutoreleasePoolThrowErrorIfNotAutoreleasePool();
    CRArrayClear(that.objects);
}


// -------------------------------------------------------------- Private ------


struct CRClass CRAutoreleasePoolClass = {.class = &CRAutoreleasePoolMetaClass, .callbacks = &CRAutoreleasePoolCallbacks};
struct CRClass CRAutoreleasePoolMetaClass = {.class = &CRAutoreleasePoolMetaClass, .callbacks = &CRAutoreleasePoolMetaCallbacks};


struct CRCallbacks CRAutoreleasePoolCallbacks = {
    NULL,
    NULL,
    NULL,
    NULL,
    &CRAutoreleasePoolDestroy,
    &CRAutoreleasePoolDescription
};


struct CRCallbacks CRAutoreleasePoolMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


void CRAutoreleasePoolDestroy(var self) {
    CRAutoreleasePoolThrowErrorIfZero();
    CRAutoreleasePoolThrowErrorIfNotAutoreleasePool();

    CRAutoreleasePoolDrain(self);
    CRRelease(that.objects);

    that.class = NULL;
    that.retain_count = 0;
    that.objects = zero;

    CRFree(this);
}


var CRAutoreleasePoolDescription(var self) {
    CRAutoreleasePoolThrowErrorIfZero();
    CRAutoreleasePoolThrowErrorIfNotAutoreleasePool();
    return CRDescription(that.objects);
}
