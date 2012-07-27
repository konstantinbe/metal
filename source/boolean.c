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

#include "boolean.h"
#include "boolean-private.h"

#include "class.h"
#include "class-private.h"

#include "macros.h"
#include "string.h"


// --------------------------------------------------------------- Macros ------


#define this ((struct CRObject*)self.pointer)
#define that (*this)

#define CRBooleanThrowErrorIfNull() if (this == NULL) CRError("self is null")
#define CRBooleanThrowErrorIfNotBoolean() if (that.class != CRBoolean.pointer) CRError("self is not a boolean")


// -------------------------------------------------- Constants & Globals ------


const var CRBoolean = {&CRBooleanClass};
const var CRTrue = {&CRBooleanProxy, {true}};
const var CRFalse = {&CRBooleanProxy, {false}};


// ---------------------------------------------------- Creating Booleans ------


var CRBooleanMake(bool boolean) {
    return boolean ? CRTrue : CRFalse;
}

// -------------------------------------------------------------- Private ------


struct CRObject CRBooleanProxy = {&CRBooleanClass, 0};


struct CRClass CRBooleanClass = {.class = &CRBooleanMetaClass, .callbacks = &CRBooleanCallbacks};
struct CRClass CRBooleanMetaClass = {.class = &CRBooleanMetaClass, .callbacks = &CRBooleanMetaCallbacks};


struct CRCallbacks CRBooleanCallbacks = {
    &CRBooleanHash,
    &CRBooleanEquals,
    &CRBooleanCopy,
    NULL,
    NULL,
    &CRBooleanDescription
};


struct CRCallbacks CRBooleanMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


CRNatural64 CRBooleanHash(var self) {
    CRBooleanThrowErrorIfNull();
    CRBooleanThrowErrorIfNotBoolean();
    return self.payload.boolean ? 1ull : 0ull;
}


bool CRBooleanEquals(var self, var other) {
    CRBooleanThrowErrorIfNull();
    CRBooleanThrowErrorIfNotBoolean();

    const bool boolean1 = self.payload.boolean;
    const bool boolean2 = other.payload.boolean;

    if (boolean1 && boolean2) return true;
    if (!boolean1 && !boolean2) return true;

    return false;
}


var CRBooleanCopy(var self) {
    CRBooleanThrowErrorIfNull();
    CRBooleanThrowErrorIfNotBoolean();
    return self;
}


var CRBooleanDescription(var self) {
    CRBooleanThrowErrorIfNull();
    CRBooleanThrowErrorIfNotBoolean();

    const bool is_true = self.payload.boolean != 0;
    const char* characters = is_true ? "true" : "false";
    const var description = CRStringCreateWithCharacters(characters);

    return CRAutorelease(description);
}
