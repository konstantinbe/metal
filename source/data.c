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


static var MLDataMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLDataMake(MLAllocate(MLDataSize), MLData, 1, 0, 0, NULL);
}


MLPointer MLDataMetaDefaultMethods[] = {
    "create", MLDataMetaCreate,
    NULL
};


static var MLDataInit(var class, var self, var command, var arguments, var options) {
    self = MLSuper(command, arguments, options);
    when (self) {
        // TODO: implement.
    }
    return self;
}


static var MLDataDestroy(var class, var self, var command, var arguments, var options) {
    return null;
}


static var MLDataIsData(var class, var self, var command, var arguments, var options) {
    return yes;
}


static var MLDataDescription(var class, var self, var command, var arguments, var options) {
    MLWarning("TODO: implement method -description for data");
    return null;
}


static var MLDataEquals(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDataHash(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDataCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDataMutableCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


MLPointer MLDataDefaultMethods[] = {
    "init", MLDataInit,
    "destroy", MLDataDestroy,
    "is_data?", MLDataIsData,

    "description", MLDataDescription,
    "equals*?", MLDataEquals,
    "hash", MLDataHash,
    "copy", MLDataCopy,
    "mutable_copy", MLDataMutableCopy,
    NULL
};


MLPointer MLInlineDataMetaDefaultMethods[] = {
    NULL
};


static var MLInlineDataRetain(var class, var self, var command, var arguments, var options) {
    MLError("Can't retain an inline data, you have to copy it");
    return null;
}


static var MLInlineDataRetainCount(var class, var self, var command, var arguments, var options) {
    return N(-1);
}


static var MLInlineDataRelease(var class, var self, var command, var arguments, var options) {
    MLError("Can't release an inline data because it can't be retained in the first place");
    return null;
}


static var MLInlineDataAutorelease(var class, var self, var command, var arguments, var options) {
    MLError("Can't autorelease an inline data because it can't be retained in the first place");
    return null;
}


MLPointer MLInlineDataDefaultMethods[] = {
    "retain", MLInlineDataRetain,
    "retainCount", MLInlineDataRetainCount,
    "release", MLInlineDataRelease,
    "autorelease", MLInlineDataAutorelease,
};


MLPointer MLMutableDataMetaDefaultMethods[] = {
    NULL
};


MLPointer MLMutableDataDefaultMethods[] = {
    NULL
};
