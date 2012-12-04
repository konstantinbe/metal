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

#include <string.h>
#include "word.h"


#define meta MLClassStructure(self)
#define that MLWord(self)


static var MLWordMetaCreate(var context, var self, var command, var arguments) {
    return MLWordMake(0);
}


MLPointer MLWordMetaDefaultMethods[] = {
    "create", MLWordMetaCreate,
    NULL
};


static var MLWordDestroy(var context, var self, var command, var arguments) {
    return null;
}


static var MLWordIsWord(var context, var self, var command, var arguments) {
    return yes;
}


static var MLWordDescription(var context, var self, var command, var arguments) {
    const int bufferSize = 1024 * 1024;
    char buffer[bufferSize + 1];
    const int count = snprintf(buffer, bufferSize + 1, "%llx", self.payload.natural);
    char *characters = strncpy(MLAllocate(count + 1), buffer, count + 1);
    var description = MLStringMake(MLAllocate(MLStringSize), MLString, 1, count, count, characters);
    return MLAutorelease(description);
}


static var MLWordEquals(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    unless (MLIsWord(object)) return no;
    return B(self.payload.natural == object.payload.natural);
}


static var MLWordHash(var context, var self, var command, var arguments) {
    return self;
}


static var MLWordCopy(var context, var self, var command, var arguments) {
    return self;
}


MLPointer MLWordDefaultMethods[] = {
    "destroy", MLWordDestroy,

    "is-word?", MLWordIsWord,

    "description", MLWordDescription,
    "equals*?", MLWordEquals,
    "hash", MLWordHash,
    "copy", MLWordCopy,

    NULL
};
