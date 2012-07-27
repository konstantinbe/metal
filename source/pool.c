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

#include "pool.h"
#include "pool-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "mutable-array.h"
#include "macros.h"
#include <string.h>


// --------------------------------------------------------------- Macros ------


#define this ((struct CRPool*)self.pointer)
#define that (*this)

#define CRPoolThrowErrorIfNull() if (this == NULL) CRError("self is null")
#define CRPoolThrowErrorIfNotPool() if (that.class != CRPool.pointer) CRError("self is not an autorelease pool")


// -------------------------------------------------- Constants & Globals ------


const var CRPool = {&CRPoolClass};


// ------------------------------------------- Creating Autorelease Pools ------


var CRPoolCreate() {
    struct CRPool* pool = CRAllocateAndClear(sizeof(struct CRPool));
    var self = CRReference(pool);

    that.class = &CRPoolClass;
    that.retain_count = 1;
    that.objects = CRArrayCreateMutableWithCapacity(CRPoolMinCapacity);

    return self;
}


// ----------------------------------------------------------- Properties ------


var CRPoolObjects(var self) {
    CRPoolThrowErrorIfNull();
    CRPoolThrowErrorIfNotPool();
    const var objects = CRCopy(that.objects);
    return CRAutorelease(objects);
}


// ---------------------------------------------------------------- Other ------


void CRPoolAdd(var self, var object) {
    CRPoolThrowErrorIfNull();
    CRPoolThrowErrorIfNotPool();
    CRArrayAdd(that.objects, object);
}


void CRPoolDrain(var self) {
    CRPoolThrowErrorIfNull();
    CRPoolThrowErrorIfNotPool();
    CRArrayClear(that.objects);
}


// -------------------------------------------------------------- Private ------


struct CRClass CRPoolClass = {.class = &CRPoolMetaClass, .callbacks = &CRPoolCallbacks};
struct CRClass CRPoolMetaClass = {.class = &CRPoolMetaClass, .callbacks = &CRPoolMetaCallbacks};


struct CRCallbacks CRPoolCallbacks = {
    NULL,
    NULL,
    NULL,
    NULL,
    &CRPoolDestroy,
    &CRPoolDescription
};


struct CRCallbacks CRPoolMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


void CRPoolDestroy(var self) {
    CRPoolThrowErrorIfNull();
    CRPoolThrowErrorIfNotPool();

    CRPoolDrain(self);
    CRRelease(that.objects);

    that.class = NULL;
    that.retain_count = 0;
    that.objects = null;

    CRFree(this);
}


var CRPoolDescription(var self) {
    CRPoolThrowErrorIfNull();
    CRPoolThrowErrorIfNotPool();
    return CRDescription(that.objects);
}
