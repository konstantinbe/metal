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

#include "date.h"


#define meta MLClassStructure(self)
#define that MLObjectStructure(self)


static var MLDateMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLDateMake(0);
}


MLPointer MLDateMetaDefaultMethods[] = {
    "create", MLDateMetaCreate,
    NULL
};


static var MLDateInit(var class, var self, var command, var arguments, var options) {
    return self;
}


static var MLDateInitWithSecondsSince1970(var class, var self, var command, var arguments, var options) {
    var secondsSince1970 = MLArgument(0);
    unless (secondsSince1970) return null;
    return MLDateMake(MLDecimalFrom(secondsSince1970));
}


static var MLDateDestroy(var class, var self, var command, var arguments, var options) {
    return null;
}


static var MLDateParse(var class, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    // TODO: implement.
    return null;
}


static var MLDateIsDate(var class, var self, var command, var arguments, var options) {
    return yes;
}


static var MLDateDescription(var class, var self, var command, var arguments, var options) {
    MLWarning("TODO: implement method -description for dates");
    return null;
}


static var MLDateEquals(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDateHash(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDateCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDateMutableCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


MLPointer MLDateDefaultMethods[] = {
    "init", MLDateInit,
    "init_with_seconds_since_1970*", MLDateInitWithSecondsSince1970,
    "destroy", MLDateDestroy,
    "is_date?", MLDateIsDate,

    "description", MLDateDescription,
    "equals*?", MLDateEquals,
    "hash", MLDateHash,
    "copy", MLDateCopy,
    "mutable_copy", MLDateMutableCopy,
     NULL
};
