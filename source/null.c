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

#include "null.h"


#define meta MLClassStructure(self)
#define that MLObjectStructure(self)


static var MLNullMetaCreate(var context, var self, var command, var arguments, var options) {
    return null;
}


MLPointer MLNullMetaDefaultMethods[] = {
    "create", MLNullMetaCreate,
    NULL
};


static var MLNullDestroy(var context, var self, var command, var arguments, var options) {
    return null;
}


static var MLNullIsNull(var context, var self, var command, var arguments, var options) {
    return yes;
}


static var MLNullIsTruthy(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLNullIsFalsy(var context, var self, var command, var arguments, var options) {
    return yes;
}


static var MLNullDescription(var context, var self, var command, var arguments, var options) {
    return S("null");
}


static var MLNullEquals(var context, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    return MLSend(object, IS("is-null?"), null, null);
}


static var MLNullHash(var context, var self, var command, var arguments, var options) {
    return W(0xffffffffffffffffull);
}


static var MLNullCopy(var context, var self, var command, var arguments, var options) {
    return self;
}


static var MLNullMutableCopy(var context, var self, var command, var arguments, var options) {
    return self;
}


static var MLNullPerformArgumentsOptionsBlock(var context, var self, var command, var arguments, var options) {
    return null;
}


MLPointer MLNullDefaultMethods[] = {
    "destroy", MLNullDestroy,

    "is-null?", MLNullIsNull,
    "is-truthy?", MLNullIsTruthy,
    "is-falsy?", MLNullIsFalsy,

    "description", MLNullDescription,
    "equals*?", MLNullEquals,
    "hash", MLNullHash,
    "copy", MLNullCopy,
    "mutable-copy", MLNullCopy,

    "perform*arguments*options*", MLNullPerformArgumentsOptionsBlock,
    NULL
};
