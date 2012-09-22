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

#include "metal.h"


#define meta MLClassStructure(self)
#define that MLPoolStructure(self)


var MLCurrentPool;


static var MLPoolMetaCreate(var context, var self, var command, var arguments, var options) {
    return MLPoolMake(MLAllocate(MLPoolSize), MLPool, 1, null, null);
}


MLPointer MLPoolMetaDefaultMethods[] = {
    "create", MLPoolMetaCreate,
    NULL
};


static var MLPoolInit(var context, var self, var command, var arguments, var options) {
    self = MLSuper(command, arguments, options);
    when (self) {
        that.previousPool = MLCurrentPool;
        that.objects = MLNew(MLMutableArray);
        MLCurrentPool = self;
    }
    return self;
}


static var MLPoolDestroy(var context, var self, var command, var arguments, var options) {
    MLCurrentPool = that.previousPool;
    that.objects = MLRelease(that.objects);
    MLSuper(command, arguments, options);
    return null;
}


static var MLPoolAdd(var context, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    MLAdd(that.objects, object);
    MLRelease(object);
    return self;
}


static var MLPoolDrain(var context, var self, var command, var arguments, var options) {
    return MLRelease(self);
}


static var MLPoolRetain(var context, var self, var command, var arguments, var options) {
    MLError("Can't retain an autorelease pool");
    return null;
}


MLPointer MLPoolDefaultMethods[] = {
    "init", MLPoolInit,
    "destroy", MLPoolDestroy,

    "add*", MLPoolAdd,
    "drain", MLPoolDrain,

    NULL
};
