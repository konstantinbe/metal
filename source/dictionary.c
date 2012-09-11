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


static var MLDictionaryMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLDictionaryMake(MLAllocate(MLDictionarySize), MLDictionary, 1, 0, 0, NULL);
}


MLPointer MLDictionaryMetaDefaultMethods[] = {
    "create", MLDictionaryMetaCreate,
    NULL
};


static var MLDictionaryInit(var class, var self, var command, var arguments, var options) {
    self = MLSuper(command, arguments, options);
    when (self) {
        // TODO: implement.
    }
    return self;
}


static var MLDictionaryDestroy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryCount(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryContains(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryContainsAll(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryContainsAny(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryIsEmpty(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryIsInline(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryIsMutable(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryKeys(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryValues(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryGet(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryGetMany(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryWithValue(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryWithMany(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryWithout(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryWithoutMany(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryIsDictionary(var class, var self, var command, var arguments, var options) {
    return yes;
}


static var MLDictionaryDescription(var class, var self, var command, var arguments, var options) {
    MLWarning("TODO: implement method -description for dictionaries");
    return null;
}


static var MLDictionaryEquals(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryHash(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryCopy(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLDictionaryMutableCopy(var class, var self, var command, var arguments, var options) {
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


static var MLInlineDictionaryRetain(var class, var self, var command, var arguments, var options) {
    MLError("Can't retain an inline dictionary, you have to copy it");
    return null;
}


static var MLInlineDictionaryRetainCount(var class, var self, var command, var arguments, var options) {
    return N(-1);
}


static var MLInlineDictionaryRelease(var class, var self, var command, var arguments, var options) {
    MLError("Can't release an inline dictionary because it can't be retained in the first place");
    return null;
}


static var MLInlineDictionaryAutorelease(var class, var self, var command, var arguments, var options) {
    MLError("Can't autorelease an inline dictionary because it can't be retained in the first place");
    return null;
}


MLPointer MLInlineDictionaryDefaultMethods[] = {
    "retain", MLInlineDictionaryRetain,
    "retainCount", MLInlineDictionaryRetainCount,
    "release", MLInlineDictionaryRelease,
    "autorelease", MLInlineDictionaryAutorelease,
};


MLPointer MLMutableDictionaryMetaDefaultMethods[] = {
    NULL
};


static var MLMutableDictionaryPutValue(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableDictionaryPutMany(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableDictionaryRemove(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableDictionaryRemoveMany(var class, var self, var command, var arguments, var options) {
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
