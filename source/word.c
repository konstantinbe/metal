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

#include "word.h"


#define meta MLClassStructure(self)
#define that MLWord(self)


static var MLWordMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLWordMake(0);
}


MLPointer MLWordMetaDefaultMethods[] = {
    "create", MLWordMetaCreate,
    NULL
};


static var MLWordDestroy(var class, var self, var command, var arguments, var options) {
    return null;
}


static var MLWordIsWord(var class, var self, var command, var arguments, var options) {
    return yes;
}


static var MLWordDescription(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLWordEquals(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLWordHash(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLWordCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLWordMutableCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


MLPointer MLWordDefaultMethods[] = {
    "destroy", MLWordDestroy,
    "is-word?", MLWordIsWord,

    "description", MLWordDescription,
    "equals*?", MLWordEquals,
    "hash", MLWordHash,
    "copy", MLWordCopy,
    "mutable-copy", MLWordMutableCopy,
    NULL
};
