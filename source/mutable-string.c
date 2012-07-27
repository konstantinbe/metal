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

#include "mutable-string.h"
#include "mutable-string-private.h"

#include "string.h"
#include "string-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "macros.h"
#include "math.h"
#include <string.h>


// --------------------------------------------------------------- Macros ------


#define this ((struct CRString*)self.pointer)
#define that (*this)

#define CRMutableStringThrowErrorIfNull() if (this == NULL) CRError("self is null")
#define CRMutableStringThrowErrorIfNotMutableString() if (that.class != CRMutableString.pointer) CRError("self is not a mutable string")


// -------------------------------------------------- Constants & Globals ------


const var CRMutableString = {&CRMutableStringClass};


// ----------------------------------------------------- Creating Strings ------


var CRStringCreateMutable() {
    return CRStringCreateMutableWithCharacters("");
}


var CRStringCreateMutableWithCapacity(CRNatural capacity) {
    struct CRString* string = CRAllocate(sizeof(struct CRString));
    var self = CRReference(string);

    if (capacity < CRMutableStringMinCapacity) capacity = CRMutableStringMinCapacity;
    capacity = CRRoundUpToPowerOfTwo(capacity);

    that.class = &CRMutableStringClass;
    that.retain_count = 1;
    that.capacity = capacity;
    that.length = 0;
    that.characters = CRAllocate(capacity + 1);

    return self;
}


var CRStringCreateMutableWithCharacters(const CRCharacter* characters) {
    var self = CRStringCreateWithCharacters(characters);
    that.class = &CRMutableStringClass;
    self.payload.natural = (CRNatural64)this;
    return self;
}


// ----------------------------------------------------- Mutating Strings ------


void CRStringPrepend(var self, var string) {
    CRMutableStringThrowErrorIfNull();
    CRMutableStringThrowErrorIfNotMutableString();

    CRAssert(CRIsNull(string) == false, "Can't prepend string, string is null");
    CRAssert(CRStringIsMutable(self), "Can't prepend string '%s' to non-mutable string '%s'", CRStringCharacters(string), CRStringCharacters(self));

    const CRNatural string_length = CRStringLength(string);
    if (string_length == 0) return;

    const var copy = CRCopy(self);
    CRStringIncreaseCapacityToAtLeast(self, that.length + string_length);
    that.characters[0] = '\0';
    strncat(that.characters, CRStringCharacters(string), string_length);
    strncat(that.characters, CRStringCharacters(copy), CRStringLength(copy));
    that.length += string_length;
    CRRelease(copy);
}


void CRStringAppend(var self, var string) {
    CRMutableStringThrowErrorIfNull();
    CRMutableStringThrowErrorIfNotMutableString();

    CRAssert(CRIsNull(string) == false, "Can't append string, string is null");

    const CRNatural string_length = CRStringLength(string);
    if (string_length == 0) return;

    CRStringIncreaseCapacityToAtLeast(self, that.length + string_length);
    strncat(that.characters, CRStringCharacters(string), string_length);
    that.length += string_length;
}


void CRStringClear(var self) {
    CRMutableStringThrowErrorIfNull();
    CRMutableStringThrowErrorIfNotMutableString();

    if (that.length == 0) return;

    that.length = 0;
    that.characters[0] = '\0';
}


// ---------------------------------------------------------------- Other ------


void CRStringIncreaseCapacityToAtLeast(var self, CRNatural capacity) {
    CRMutableStringThrowErrorIfNull();
    CRMutableStringThrowErrorIfNotMutableString();

    if (capacity <= that.capacity) return;
    if (capacity < CRMutableStringMinCapacity) capacity = CRMutableStringMinCapacity;
    CRRoundUpToPowerOfTwo(capacity);

    that.capacity = capacity;
    that.characters = CRResize(that.characters, sizeof(CRCharacter) * (capacity + 1));
}


// -------------------------------------------------------------- Private ------


struct CRClass CRMutableStringClass = {.class = &CRMutableStringMetaClass, .callbacks = &CRMutableStringCallbacks};
struct CRClass CRMutableStringMetaClass = {.class = &CRMutableStringMetaClass, .callbacks = &CRMutableStringMetaCallbacks};


struct CRCallbacks CRMutableStringCallbacks = {
    &CRStringHash,
    &CRStringEquals,
    &CRStringCopy,
    &CRStringMutableCopy,
    &CRStringDestroy,
    &CRStringDescription
};


struct CRCallbacks CRMutableStringMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};
