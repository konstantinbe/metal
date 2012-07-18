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

#include "word.h"
#include "word-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "macros.h"
#include "string.h"


// --------------------------------------------------------------- Macros ------


#define this ((struct CRObject*)self.pointer)
#define that (*this)

#define CRWordThrowErrorIfZero() if (this == NULL) CRError("self is zero")
#define CRWordThrowErrorIfNotWord() if (that.class != CRWord.pointer) CRError("self is not a word")


// -------------------------------------------------- Constants & Globals ------


const var CRWord = {&CRWordClass};


// ------------------------------------------------------- Creating Words ------


var CRWordMake(CRNatural value) {
    var word = zero;
    word.pointer = &CRWordProxy;
    word.payload.natural = value;
    return word;
}


// -------------------------------------------------------------- Private ------


struct CRObject CRWordProxy = {&CRWordClass, 0};


struct CRClass CRWordClass = {.class = &CRWordMetaClass, .callbacks = &CRWordCallbacks};
struct CRClass CRWordMetaClass = {.class = &CRWordMetaClass, .callbacks = &CRWordMetaCallbacks};


struct CRCallbacks CRWordCallbacks = {
    &CRWordHash,
    &CRWordEquals,
    &CRWordCopy,
    NULL,
    NULL,
    &CRWordDescription
};


struct CRCallbacks CRWordMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


CRNatural64 CRWordHash(var self) {
    CRWordThrowErrorIfZero();
    CRWordThrowErrorIfNotWord();
    return (CRNatural64)self.payload.natural;
}


bool CRWordEquals(var self, var other) {
    CRWordThrowErrorIfZero();
    CRWordThrowErrorIfNotWord();
    return self.payload.natural == other.payload.natural;
}


var CRWordCopy(var self) {
    CRWordThrowErrorIfZero();
    CRWordThrowErrorIfNotWord();
    return self;
}


var CRWordDescription(var self) {
    CRWordThrowErrorIfZero();
    CRWordThrowErrorIfNotWord();

    const CRNatural characters_capacity = 1000;
    CRCharacter characters[characters_capacity];
    snprintf(characters, characters_capacity, "0x%llx", self.payload.natural);

    var description = CRStringCreateWithCharacters(characters);
    return CRAutorelease(description);
}
