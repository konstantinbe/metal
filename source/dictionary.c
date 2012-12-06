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


#define MLMutableDictionaryMinCapacity 16


#define meta MLClassStructure(self)
#define that MLDictionaryStructure(self)


static var MLDictionaryMetaCreate(var context, var self, var command, var arguments) {
    return MLDictionaryMake(MLAllocate(MLDictionarySize), MLDictionary, 1, 0, 0, NULL);
}


MLPointer MLDictionaryMetaDefaultMethods[] = {
    "create", MLDictionaryMetaCreate,
    NULL
};


static var MLDictionaryInit(var context, var self, var command, var arguments) {
    self = MLSuper("init");
    when (self) {
        // TODO: implement.
    }
    return self;
}


static var MLDictionaryDestroy(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryCount(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryContains(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryContainsAll(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryContainsAny(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryIsEmpty(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryIsInline(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryIsMutable(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryKeys(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryValues(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryGet(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryGetMany(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryWithValue(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryWithMany(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryWithout(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryWithoutMany(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryIsDictionary(var context, var self, var command, var arguments) {
    return yes;
}


static var MLDictionaryDescription(var context, var self, var command, var arguments) {
    MLWarning("TODO: implement method -description for dictionaries");
    return null;
}


static var MLDictionaryEquals(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryHash(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryCopy(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryMutableCopy(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


MLPointer MLDictionaryDefaultMethods[] = {
    "init", MLDictionaryInit,
    "destroy", MLDictionaryDestroy,

    "count", MLDictionaryCount,
    "contains*?", MLDictionaryContains,
    "contains-all*?", MLDictionaryContainsAll,
    "contains-any*?", MLDictionaryContainsAny,

    "is-empty?", MLDictionaryIsEmpty,
    "is-inline?", MLDictionaryIsInline,
    "is-mutable?", MLDictionaryIsMutable,

    "keys", MLDictionaryKeys,
    "values", MLDictionaryValues,

    "get", MLDictionaryGet,
    "get-many", MLDictionaryGetMany,

    "with*value*", MLDictionaryWithValue,
    "with-many*", MLDictionaryWithMany,

    "without*", MLDictionaryWithout,
    "without-many*", MLDictionaryWithoutMany,

    "is-dictionary?", MLDictionaryIsDictionary,

    "description", MLDictionaryDescription,
    "equals*?", MLDictionaryEquals,
    "hash", MLDictionaryHash,
    "copy", MLDictionaryCopy,
    "mutable-copy", MLDictionaryMutableCopy,
    NULL
};


MLPointer MLInlineDictionaryMetaDefaultMethods[] = {
    NULL
};


static var MLInlineDictionaryRetain(var context, var self, var command, var arguments) {
    MLError("Can't retain an inline dictionary, you have to copy it");
    return null;
}


static var MLInlineDictionaryRetainCount(var context, var self, var command, var arguments) {
    return N(-1);
}


static var MLInlineDictionaryRelease(var context, var self, var command, var arguments) {
    MLError("Can't release an inline dictionary because it can't be retained in the first place");
    return null;
}


static var MLInlineDictionaryAutorelease(var context, var self, var command, var arguments) {
    MLError("Can't autorelease an inline dictionary because it can't be retained in the first place");
    return null;
}


MLPointer MLInlineDictionaryDefaultMethods[] = {
    "retain", MLInlineDictionaryRetain,
    "retainCount", MLInlineDictionaryRetainCount,
    "release", MLInlineDictionaryRelease,
    "autorelease", MLInlineDictionaryAutorelease,
    NULL
};


MLPointer MLMutableDictionaryMetaDefaultMethods[] = {
    NULL
};


static var MLMutableDictionaryPutValue(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableDictionaryPutMany(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableDictionaryRemove(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableDictionaryRemoveMany(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


MLPointer MLMutableDictionaryDefaultMethods[] = {
    "put*value*", MLMutableDictionaryPutValue,
    "put-many*", MLMutableDictionaryPutMany,

    "remove*", MLMutableDictionaryRemove,
    "remove-many*", MLMutableDictionaryRemoveMany,

    NULL
};
