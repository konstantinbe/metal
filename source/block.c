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

#include "block.h"


#define meta MLClassStructure(self)
#define that MLObjectStructure(self)


static var MLBlockMetaCreate(var context, var self, var command, var arguments, var options) {
    return MLBlockMake(NULL);
}


MLPointer MLBlockMetaDefaultMethods[] = {
    "create", MLBlockMetaCreate,
    NULL
};


static var MLBlockDestroy(var context, var self, var command, var arguments, var options) {
    return null;
}


static var MLBlockIsBlock(var context, var self, var command, var arguments, var options) {
    return yes;
}


static var MLBlockDescription(var context, var self, var command, var arguments, var options) {
    return S("<Block>");
}


static var MLBlockEquals(var context, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    return B(self.pointer == object.pointer && self.payload.code == object.payload.code);
}


static var MLBlockHash(var context, var self, var command, var arguments, var options) {
    return W((size_t)self.payload.code);
}


static var MLBlockCopy(var context, var self, var command, var arguments, var options) {
    return self;
}


MLPointer MLBlockDefaultMethods[] = {
    "destroy", MLBlockDestroy,
    "is-block?", MLBlockIsBlock,

    "description", MLBlockDescription,
    "equals*?", MLBlockEquals,
    "hash", MLBlockHash,
    "copy", MLBlockCopy,

    NULL
};
