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

#include "null.h"
#include "null-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "macros.h"
#include "string.h"
#include <math.h>


// --------------------------------------------------------------- Macros ------


#define this ((struct CRObject*)self.pointer)
#define that (*this)

#define CRNullThrowErrorIfZero() if (this == NULL) CRError("self is zero")
#define CRNullThrowErrorIfNotNull() if (that.class != CRNull.pointer) CRError("self is not null")


// -------------------------------------------------- Constants & Globals ------


const var CRNull = {&CRNullClass};
const var null = {&CRNullClass, .payload.decimal = NAN};


// -------------------------------------------------------------- Private ------


struct CRClass CRNullClass = {.class = &CRNullMetaClass, .callbacks = &CRNullCallbacks};
struct CRClass CRNullMetaClass = {.class = &CRNullMetaClass, .callbacks = &CRNullMetaCallbacks};


struct CRCallbacks CRNullCallbacks = {
    &CRNullHash,
    &CRNullEquals,
    &CRNullCopy,
    NULL,
    NULL,
    &CRNullDescription
};


struct CRCallbacks CRNullMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


CRNatural64 CRNullHash(var self) {
    CRNullThrowErrorIfZero();
    CRNullThrowErrorIfNotNull();
    return (CRNatural64)self.payload.natural;
}


bool CRNullEquals(var self, var other) {
    CRNullThrowErrorIfZero();
    CRNullThrowErrorIfNotNull();
    return self.payload.decimal == other.payload.decimal;
}


var CRNullCopy(var self) {
    CRNullThrowErrorIfZero();
    CRNullThrowErrorIfNotNull();
    return self;
}


var CRNullDescription(var self) {
    CRNullThrowErrorIfZero();
    CRNullThrowErrorIfNotNull();

    var description = CRStringCreateWithCharacters("null");
    return CRAutorelease(description);
}
