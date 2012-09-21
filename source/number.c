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

#include "number.h"
#include "math.h"


#define meta MLClassStructure(self)
#define that MLObjectStructure(self)


static var MLNumberMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLNumberMake(0);
}


MLPointer MLNumberMetaDefaultMethods[] = {
    "create", MLNumberMetaCreate,
    NULL
};


static var MLNumberDestroy(var class, var self, var command, var arguments, var options) {
    return null;
}


static var MLNumberIsNaN(var class, var self, var command, var arguments, var options) {
    return B(isnan(self.payload.decimal));
}


static var MLNumberIsFinite(var class, var self, var command, var arguments, var options) {
    return B(isfinite(self.payload.decimal));
}


static var MLNumberIsInfinite(var class, var self, var command, var arguments, var options) {
    return B(isinf(self.payload.decimal));
}


static var MLNumberIsNumber(var class, var self, var command, var arguments, var options) {
    return yes;
}


static var MLNumberDescription(var class, var self, var command, var arguments, var options) {
    const int stringCount = 1024 * 1024;
    char* string = MLInline(stringCount + 1);
    snprintf(string, stringCount, "%f", self.payload.decimal);
    return S(string);
}


static var MLNumberEquals(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    unless (MLIsNumber(object)) return no;
    return B(self.payload.decimal == object.payload.decimal);
}


static var MLNumberHash(var class, var self, var command, var arguments, var options) {
    return W(self.payload.natural);
}


static var MLNumberCopy(var class, var self, var command, var arguments, var options) {
    return self;
}


var MLNumberCompareTo(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    unless (MLIsNumber(object)) MLError("Can't compare number to object, object is not a number");
    if (MLDecimalFrom(self) < MLDecimalFrom(object)) return N(-1);
    if (MLDecimalFrom(self) > MLDecimalFrom(object)) return N(+1);
    return N(0);
}


MLPointer MLNumberDefaultMethods[] = {
    "destroy", MLNumberDestroy,

    "is-nan?", MLNumberIsNaN,
    "is-finite?", MLNumberIsFinite,
    "is-infinite?", MLNumberIsInfinite,

    "is-number?", MLNumberIsNumber,

    "description", MLNumberDescription,
    "equals*?", MLNumberEquals,
    "hash", MLNumberHash,
    "copy", MLNumberCopy,

    "compare-to*", MLNumberCompareTo,

    NULL
};
