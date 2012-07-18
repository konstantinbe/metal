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

#include "date.h"
#include "date-private.h"

#include "class.h"
#include "class-private.h"

#include "macros.h"
#include "string.h"

#include <time.h>
#include <math.h>
#include <stdio.h>


// --------------------------------------------------------------- Macros ------


#define this ((struct CRObject*)self.pointer)
#define that (*this)

#define CRDateThrowErrorIfZero() if (this == NULL) CRError("self is zero")
#define CRDateThrowErrorIfNotDate() if (that.class != CRDate.pointer) CRError("self is not a date")


// -------------------------------------------------- Constants & Globals ------


const var CRDate = {&CRDateClass};


// ------------------------------------------------------- Creating Dates ------


var CRDateMake(CRDecimal seconds_since_1970) {
    var date = zero;
    date.pointer = &CRDateProxy;
    date.payload.decimal = round(seconds_since_1970);
    return date;
}


var CRDateNow() {
    time_t seconds = 0;
    time(&seconds);
    return CRDateMake(round((CRDecimal)seconds));
}


var CRDateParse(var string) {
    return zero;
}


// ----------------------------------------------------------- Properties ------


CRDecimal CRDateSecondsSince1970(var self) {
    return self.payload.decimal;
}


// -------------------------------------------------------------- Private ------


struct CRObject CRDateProxy = {&CRDateClass, 0};


struct CRClass CRDateClass = {.class = &CRDateMetaClass, .callbacks = &CRDateCallbacks};
struct CRClass CRDateMetaClass = {.class = &CRDateMetaClass, .callbacks = &CRDateMetaCallbacks};


struct CRCallbacks CRDateCallbacks = {
    &CRDateHash,
    &CRDateEquals,
    &CRDateCopy,
    NULL,
    NULL,
    &CRDateDescription
};


struct CRCallbacks CRDateMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


CRNatural64 CRDateHash(var self) {
    return (CRNatural64)self.payload.natural;
}


bool CRDateEquals(var self, var other) {
    return self.payload.decimal == other.payload.decimal;
}


var CRDateCopy(var self) {
    return self;
}


var CRDateDescription(var self) {
    // TODO: implement proper description for dates.
    time_t time = (time_t)self.payload.decimal;
    const char* characters = asctime(gmtime(&time));
    const var description = CRStringCreateWithCharacters(characters);
    return CRAutorelease(description);
}
