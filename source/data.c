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

#include "data.h"


#define MLMutableDataMinCapacity 64


#define meta MLClassStructure(self)
#define that MLDataStructure(self)


static var MLDataMetaCreate(var context, var self, var command, var arguments) {
    return MLDataMake(MLAllocate(MLDataSize), MLData, 1, 0, 0, NULL);
}


MLPointer MLDataMetaDefaultMethods[] = {
    "create", MLDataMetaCreate,
    NULL
};


static var MLDataInit(var context, var self, var command, var arguments) {
    self = MLSuper("init");
    when (self) {
        // TODO: implement.
    }
    return self;
}


static var MLDataDestroy(var context, var self, var command, var arguments) {
    return null;
}


static var MLDataIsData(var context, var self, var command, var arguments) {
    return yes;
}


static var MLDataDescription(var context, var self, var command, var arguments) {
    MLWarning("TODO: implement method -description for data");
    return null;
}


static var MLDataEquals(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDataHash(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDataCopy(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDataMutableCopy(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


MLPointer MLDataDefaultMethods[] = {
    "init", MLDataInit,
    "destroy", MLDataDestroy,
    "is-data?", MLDataIsData,

    "description", MLDataDescription,
    "equals*?", MLDataEquals,
    "hash", MLDataHash,
    "copy", MLDataCopy,
    "mutable-copy", MLDataMutableCopy,
    NULL
};


MLPointer MLInlineDataMetaDefaultMethods[] = {
    NULL
};


static var MLInlineDataRetain(var context, var self, var command, var arguments) {
    MLError("Can't retain an inline data, you have to copy it");
    return null;
}


static var MLInlineDataRetainCount(var context, var self, var command, var arguments) {
    return N(-1);
}


static var MLInlineDataRelease(var context, var self, var command, var arguments) {
    MLError("Can't release an inline data because it can't be retained in the first place");
    return null;
}


static var MLInlineDataAutorelease(var context, var self, var command, var arguments) {
    MLError("Can't autorelease an inline data because it can't be retained in the first place");
    return null;
}


MLPointer MLInlineDataDefaultMethods[] = {
    "retain", MLInlineDataRetain,
    "retainCount", MLInlineDataRetainCount,
    "release", MLInlineDataRelease,
    "autorelease", MLInlineDataAutorelease,
    NULL,
};


MLPointer MLMutableDataMetaDefaultMethods[] = {
    NULL
};


MLPointer MLMutableDataDefaultMethods[] = {
    NULL
};
