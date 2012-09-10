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

#include "boolean.h"


#define meta MLClassStructure(self)
#define that MLBoolean(self)


static var MLBooleanMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLBooleanMake(false);
}


MLPointer MLBooleanMetaDefaultMethods[] = {
    "create", MLBooleanMetaCreate,
    NULL
};


static var MLBooleanDestroy(var class, var self, var command, var arguments, var options) {
    return null;
}


static var MLBooleanIsBoolean(var class, var self, var command, var arguments, var options) {
    return yes;
}


static var MLBooleanDescription(var class, var self, var command, var arguments, var options) {
    MLWarning("TODO: implement method -description for booleans");
    return null;
}


static var MLBooleanEquals(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    const bool isTrueLeft = MLIsTrue(self);
    const bool isTrueRight = MLIsTrue(object);
    const bool equals = (isTrueLeft && isTrueRight) || (!isTrueLeft && !isTrueRight);
    return B(equals);
}


static var MLBooleanHash(var class, var self, var command, var arguments, var options) {
    return W((MLNatural)self.payload.boolean);
}


static var MLBooleanCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLBooleanMutableCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


MLPointer MLBooleanDefaultMethods[] = {
    "destroy", MLBooleanDestroy,
    "is_boolean?", MLBooleanIsBoolean,

    "description", MLBooleanDescription,
    "equals*?", MLBooleanEquals,
    "hash", MLBooleanHash,
    "copy", MLBooleanCopy,
    "mutable_copy", MLBooleanMutableCopy,
    NULL
};
