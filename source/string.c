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

#include "string.h"
#include "string-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "mutable-string.h"
#include "macros.h"
#include "digest.h"
#include <string.h>


// --------------------------------------------------------------- Macros ------


#define this ((struct CRString*)self.pointer)
#define that (*this)

#define CRStringThrowErrorIfZero() if (this == NULL) CRError("self is zero")
#define CRStringThrowErrorIfNotString() if (that.class != CRString.pointer && that.class != CRMutableString.pointer) CRError("self is not a string")


// -------------------------------------------------- Constants & Globals ------


const var CRString = {&CRStringClass};


// ----------------------------------------------------- Creating Strings ------


var CRStringMake(struct CRString* string, const CRCharacter* characters) {
    const CRNatural length = strlen(characters);
    const CRNatural64 hash = CRDigest(length, characters);
    var self = {.pointer = string, .payload.natural = hash};

    that.class = &CRStringClass;
    that.retain_count = CRRetainCountMax;
    that.capacity = length;
    that.length = length;
    that.characters = (CRCharacter*)characters;
    return self;
}


var CRStringCreate() {
    return CRStringCreateWithCharacters("");
}


var CRStringCreateWithCharacters(const CRCharacter* characters) {
    const CRNatural length = strlen(characters);
    const CRNatural64 hash = CRDigest(length, characters);

    struct CRString* string = CRAllocate(sizeof(struct CRString));
    var self = {.pointer = string, .payload.natural = hash};

    that.class = &CRStringClass;
    that.retain_count = 1;
    that.capacity = length;
    that.length = length;
    that.characters = CRAllocate(length + 1);

    strncpy(that.characters, characters, length + 1);
    return self;
}


// ----------------------------------------------------------- Properties ------


CRNatural CRStringCapacity(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return that.capacity;
}


CRNatural CRStringLength(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return that.length;
}


CRCharacter* CRStringCharacters(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return that.characters;
}


bool CRStringIsEmpty(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return that.length == 0;
}


bool CRStringIsMutable(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return that.class == CRMutableString.pointer;
}


// ----------------------------------------------------- Querying Strings ------


CRCharacter CRStringCharacterAt(var self, CRInteger index) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    if (index < 0 || index >= that.length) CRError("Index out of bounds");
    return that.characters[index];
}


CRInteger CRStringIndexOf(var self, var string) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();

    const CRInteger length_of_string = CRStringLength(string);
    if (length_of_string > that.length) return -1;
    if (length_of_string == 0) return 0;

    CRCharacter* found = strstr(that.characters, CRStringCharacters(string));
    if (!found) return -1;

    return that.length - strlen(found);
}


bool CRStringContains(var self, var string) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return CRStringIndexOf(self, string) >= 0;
}


bool CRStringBeginsWith(var self, var string) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return CRStringIndexOf(self, string) == 0;
}


bool CRStringEndsWith(var self, var string) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();

    const CRInteger length_of_string = CRStringLength(string);

    if (length_of_string == 0) return true;
    if (length_of_string > that.length) return false;

    const CRInteger index_of_string = CRStringIndexOf(self, string);
    if (index_of_string < 0) return false;

    return index_of_string == that.length - CRStringLength(string);
}


// -------------------------------------------------------------- Private ------


struct CRClass CRStringClass = {.class = &CRStringMetaClass, .callbacks = &CRStringCallbacks};
struct CRClass CRStringMetaClass = {.class = &CRStringMetaClass, .callbacks = &CRStringMetaCallbacks};


struct CRCallbacks CRStringCallbacks = {
    &CRStringHash,
    &CRStringEquals,
    &CRStringCopy,
    &CRStringMutableCopy,
    &CRStringDestroy,
    &CRStringDescription
};


struct CRCallbacks CRStringMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


CRNatural64 CRStringHash(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return self.payload.natural ? self.payload.natural : CRDigest(that.length, that.characters);
}


bool CRStringEquals(var self, var other) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();

    CRCharacter* characters1 = CRStringCharacters(self);
    CRCharacter* characters2 = CRStringCharacters(other);
    if (characters1 == characters2) return true;

    CRNatural length1 = CRStringLength(self);
    CRNatural length2 = CRStringLength(other);
    if (length1 != length2) return false;

    return strncmp(characters1, characters2, length1) == 0;
}


var CRStringCopy(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return CRStringIsMutable(self) ? CRStringCreateWithCharacters(that.characters) : CRRetain(self);
}


var CRStringMutableCopy(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return CRStringCreateMutableWithCharacters(that.characters);
}


void CRStringDestroy(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();

    CRCharacter* characters = that.characters;

    that.class = NULL;
    that.retain_count = 0;
    that.capacity = 0;
    that.length = 0;
    that.characters = NULL;

    CRFree(characters);
    CRFree(this);
}


var CRStringDescription(var self) {
    CRStringThrowErrorIfZero();
    CRStringThrowErrorIfNotString();
    return self;
}
