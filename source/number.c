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
    MLError("TODO: implement this.");
    return null;
}


static var MLNumberIsFinite(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement this.");
    return null;
}


static var MLNumberIsInfinite(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement this.");
    return null;
}


static var MLNumberIsNumber(var class, var self, var command, var arguments, var options) {
    return yes;
}


static var MLNumberDescription(var class, var self, var command, var arguments, var options) {
    MLWarning("TODO: implement method -description for numbers");
    return null;
}


static var MLNumberEquals(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    unless (MLIsNumber(object)) return no;
    return B(self.payload.decimal == object.payload.decimal);
}


static var MLNumberHash(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLNumberCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLNumberMutableCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
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
    "mutable-copy", MLNumberMutableCopy,
    NULL
};
