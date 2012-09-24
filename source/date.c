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
#include "time.h"


#define meta MLClassStructure(self)
#define that MLObjectStructure(self)


static var MLDateMetaCreate(var context, var self, var command, var arguments, var options) {
    return MLDateMake(0);
}


MLPointer MLDateMetaDefaultMethods[] = {
    "create", MLDateMetaCreate,
    NULL
};


static var MLDateInit(var context, var self, var command, var arguments, var options) {
    return self;
}


static var MLDateInitWithSecondsSince1970(var context, var self, var command, var arguments, var options) {
    var secondsSince1970 = MLArgument(0);
    unless (secondsSince1970) return null;
    return MLDateMake(MLDecimalFrom(secondsSince1970));
}


static var MLDateDestroy(var context, var self, var command, var arguments, var options) {
    return null;
}


static var MLDateParse(var context, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    // TODO: implement.
    return null;
}


static var MLDateIsDate(var context, var self, var command, var arguments, var options) {
    return yes;
}


static var MLDateDescription(var context, var self, var command, var arguments, var options) {
    const int stringCount = 1024 * 1024;
    char* string = MLInline(stringCount + 1);
    time_t time = (time_t)self.payload.decimal;
    struct tm* tm = gmtime(&time);
    strftime(string, stringCount, "%FT%TZ", tm);
    return S(string);
}


static var MLDateEquals(var context, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    unless (MLIsDate(object)) return no;
    return B(self.payload.decimal == object.payload.decimal);
}


static var MLDateHash(var context, var self, var command, var arguments, var options) {
    return W(self.payload.natural);
}


static var MLDateCopy(var context, var self, var command, var arguments, var options) {
    return self;
}


MLPointer MLDateDefaultMethods[] = {
    "init", MLDateInit,
    "init-with-seconds-since-1970*", MLDateInitWithSecondsSince1970,
    "destroy", MLDateDestroy,

    "is-date?", MLDateIsDate,

    "description", MLDateDescription,
    "equals*?", MLDateEquals,
    "hash", MLDateHash,
    "copy", MLDateCopy,

     NULL
};
