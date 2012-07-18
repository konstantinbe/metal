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

#include "number.h"
#include "number-private.h"

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

#define CRNumberThrowErrorIfZero() if (this == NULL) CRError("self is zero")
#define CRNumberThrowErrorIfNotNumber() if (that.class != CRNumber.pointer) CRError("self is not a number")


// -------------------------------------------------- Constants & Globals ------


const var CRNumber = {&CRNumberClass};
const var CRUndefined = {&CRNumberProxy, .payload.decimal = NAN};
const var CRInfinity = {&CRNumberProxy, .payload.decimal = INFINITY};


// ----------------------------------------------------- Creating Numbers ------


var CRNumberMake(CRDecimal value) {
    var number = zero;
    number.pointer = &CRNumberProxy;
    number.payload.decimal = value;
    return number;
}


// -------------------------------------------------------------- Private ------


struct CRObject CRNumberProxy = {&CRNumberClass, 0};


struct CRClass CRNumberClass = {.class = &CRNumberMetaClass, .callbacks = &CRNumberCallbacks};
struct CRClass CRNumberMetaClass = {.class = &CRNumberMetaClass, .callbacks = &CRNumberMetaCallbacks};


struct CRCallbacks CRNumberCallbacks = {
    &CRNumberHash,
    &CRNumberEquals,
    &CRNumberCopy,
    NULL,
    NULL,
    &CRNumberDescription
};


struct CRCallbacks CRNumberMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


CRNatural64 CRNumberHash(var self) {
    CRNumberThrowErrorIfZero();
    CRNumberThrowErrorIfNotNumber();
    return (CRNatural64)self.payload.natural;
}


bool CRNumberEquals(var self, var other) {
    CRNumberThrowErrorIfZero();
    CRNumberThrowErrorIfNotNumber();
    return self.payload.decimal == other.payload.decimal;
}


var CRNumberCopy(var self) {
    CRNumberThrowErrorIfZero();
    CRNumberThrowErrorIfNotNumber();
    return self;
}


var CRNumberDescription(var self) {
    CRNumberThrowErrorIfZero();
    CRNumberThrowErrorIfNotNumber();

    const CRNatural characters_capacity = 1000;
    CRCharacter characters[characters_capacity];
    snprintf(characters, characters_capacity, "%g", self.payload.decimal);

    var description = CRStringCreateWithCharacters(characters);
    return CRAutorelease(description);
}
