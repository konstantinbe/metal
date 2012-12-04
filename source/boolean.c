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


static var MLBooleanMetaCreate(var context, var self, var command, var arguments) {
    return MLBooleanMake(false);
}


MLPointer MLBooleanMetaDefaultMethods[] = {
    "create", MLBooleanMetaCreate,
    NULL
};


static var MLBooleanDestroy(var context, var self, var command, var arguments) {
    return null;
}


static var MLBooleanIsBoolean(var context, var self, var command, var arguments) {
    return yes;
}


static var MLBooleanIsYes(var context, var self, var command, var arguments) {
    return B(self.payload.boolean);
}


static var MLBooleanIsNo(var context, var self, var command, var arguments) {
    return B(!self.payload.boolean);
}


static var MLBooleanIsTruthy(var context, var self, var command, var arguments) {
    return B(self.payload.boolean);
}


static var MLBooleanIsFalsy(var context, var self, var command, var arguments) {
    return B(!self.payload.boolean);
}


static var MLBooleanDescription(var context, var self, var command, var arguments) {
    when (self)
        return S("yes");
    else
        return S("no");
}


static var MLBooleanEquals(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    unless (MLIsBoolean(object)) return no;
    const bool isLeftYes = self.payload.boolean;
    const bool isRightYes = object.payload.boolean;
    const bool isLeftNo = !isLeftYes;
    const bool isRightNo = !isRightYes;
    const bool equals = (isLeftYes && isRightYes) || (isLeftNo && isRightNo);
    return B(equals);
}


static var MLBooleanHash(var context, var self, var command, var arguments) {
    return W((MLNatural)self.payload.boolean);
}


static var MLBooleanCopy(var context, var self, var command, var arguments) {
    return self;
}


MLPointer MLBooleanDefaultMethods[] = {
    "destroy", MLBooleanDestroy,

    "is-yes?", MLBooleanIsYes,
    "is-no?", MLBooleanIsNo,

    "is-boolean?", MLBooleanIsBoolean,
    "is-truthy?", MLBooleanIsTruthy,
    "is-falsy?", MLBooleanIsFalsy,

    "description", MLBooleanDescription,
    "equals*?", MLBooleanEquals,
    "hash", MLBooleanHash,
    "copy", MLBooleanCopy,

    NULL
};
