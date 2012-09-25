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

#include "object.h"
#include "pool.h"


#define meta MLClassStructure(self)
#define that MLObjectStructure(self)


static var MLObjectMetaCreate(var context, var self, var command, var arguments, var options) {
    MLError("Can't create an instance of Object, object is either abstract or doesn't implement -create");
}


static var MLObjectMetaNew(var context, var self, var command, var arguments, var options) {
    var object = MLCreate(self);
    return MLInit(object);
}


static var MLObjectMetaName(var context, var self, var command, var arguments, var options) {
    return meta.name;
}


static var MLObjectMetaSuperclass(var context, var self, var command, var arguments, var options) {
    return meta.superclass;
}


static var MLObjectMetaSubclasses(var context, var self, var command, var arguments, var options) {
    return meta.subclasses;
}


static var MLObjectMetaMethods(var context, var self, var command, var arguments, var options) {
    return meta.methods;
}


static var MLObjectMetaIsClass(var context, var self, var command, var arguments, var options) {
    return yes;
}


MLPointer MLObjectMetaDefaultMethods[] = {
    "create", MLObjectMetaCreate,
    "new", MLObjectMetaNew,

    "name", MLObjectMetaName,
    "superclass", MLObjectMetaSuperclass,
    "subclasses", MLObjectMetaSubclasses,
    "methods", MLObjectMetaMethods,

    "is-class?", MLObjectMetaIsClass,

    NULL
};


static var MLObjectInit(var context, var self, var command, var arguments, var options) {
    return self;
}


static var MLObjectDestroy(var context, var self, var command, var arguments, var options) {
    if (that.retainCount > 0) MLWarning("Destroying object with retain count > 0");
    MLFree(self.pointer);
    return null;
}


static var MLObjectClass(var context, var self, var command, var arguments, var options) {
    return MLReference(that.class);
}


static var MLObjectDescription(var context, var self, var command, var arguments, var options) {
    const int bufferSize = 1024 * 1024;
    char buffer[bufferSize + 1];
    var class = MLClass(self);
    var className = MLName(class);
    const int count = snprintf(buffer, bufferSize + 1, "<%s>", MLStringStructure(className).characters);
    char *characters = strncpy(MLAllocate(count + 1), buffer, count + 1);
    var description = MLStringMake(MLAllocate(MLStringSize), MLString, 1, count, count, characters);
    return MLAutorelease(description);
}


static var MLObjectEquals(var context, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    return B(self.pointer == object.pointer);
}


static var MLObjectHash(var context, var self, var command, var arguments, var options) {
    return W((MLNatural)self.pointer);
}


static var MLObjectCopy(var context, var self, var command, var arguments, var options) {
    MLError("Object doesn't support copying");
    return null;
}


static var MLObjectMutableCopy(var context, var self, var command, var arguments, var options) {
    MLError("Object doesn't support mutable copying");
    return null;
}


var MLObjectCompareTo(var context, var self, var command, var arguments, var options) {
    MLError("Can't compare, object doesn't support comparing");
    return no;
}


var MLObjectIsLessThan(var context, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var result = MLCompareTo(self, object);
    if (MLDecimalFrom(result) < 0) return yes;
    return no;
}


var MLObjectIsLessThanOrEquals(var context, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var result = MLCompareTo(self, object);
    if (MLDecimalFrom(result) <= 0) return yes;
    return no;
}


var MLObjectIsGreaterThan(var context, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var result = MLCompareTo(self, object);
    if (MLDecimalFrom(result) > 0) return yes;
    return no;
}


var MLObjectIsGreaterThanOrEquals(var context, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var result = MLCompareTo(self, object);
    if (MLDecimalFrom(result) >= 0) return yes;
    return no;
}


static var MLObjectIsObject(var context, var self, var command, var arguments, var options) {
    return yes;
}


static var MLObjectIsClass(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsNull(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsBlock(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsBoolean(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsNumber(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsWord(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsDate(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsData(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsArray(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsString(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsDictionary(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsTruthy(var context, var self, var command, var arguments, var options) {
    return yes;
}


static var MLObjectIsFalsy(var context, var self, var command, var arguments, var options) {
    return no;
}


static var MLObjectIsKindOf(var context, var self, var command, var arguments, var options) {
    var class = MLArgument(0);
    var current = MLClass(self);
    whilst (current) {
        when (MLEquals(current, class)) return yes;
        current = MLSuperclass(current);
    }
    return no;
}


static var MLObjectIsInstanceOf(var context, var self, var command, var arguments, var options) {
    var class = MLArgument(0);
    return MLEquals(MLClass(self), class);
}


static var MLObjectRetain(var context, var self, var command, var arguments, var options) {
    if (that.retainCount < MLRetainCountMax) that.retainCount += 1;
    return self;
}


static var MLObjectRetainCount(var context, var self, var command, var arguments, var options) {
    return N(that.retainCount);
}


static var MLObjectRelease(var context, var self, var command, var arguments, var options) {
    if (that.retainCount >= MLRetainCountMax) return self;
    if (that.retainCount <= 0) MLWarning("Releasing an object whose retain count is already 0");
    that.retainCount -= 1;
    if (that.retainCount <= 0) self = MLDestroy(self);
    return self;
}


static var MLObjectAutorelease(var context, var self, var command, var arguments, var options) {
    if (MLCurrentPool.pointer == NULL) MLError("Can't autorelease, no autorelase pool found");
    if (that.retainCount < MLRetainCountMax) MLAdd(MLCurrentPool, self);
    return self;
}


static var MLObjectPerformArgumentsOptionsBlock(var context, var self, var command, var arguments, var options) {
    var commandToPerform = MLArgument(0);
    var argumentsToPass = MLArgument(1);
    var optionsToPass = MLArgument(2);
    var class = MLClass(self);
    var className = MLName(class);
    MLError("Can't perform command, %s doesn't respond to \"%s\"", MLStringStructure(className).characters, MLStringStructure(commandToPerform).characters);
    return null;
}


static var MLObjectSendArgumentsOptionsBlock(var context, var self, var command, var arguments, var options) {
    return null;
}


static var MLObjectRespondsTo(var context, var self, var command, var arguments, var options) {
    var commandToLookup = MLArgument(0);
    var class = MLClass(self);
    var method = MLLookup(class, commandToLookup, NULL);
    return MLIsTruthy(method);
}


MLPointer MLObjectDefaultMethods[] = {
    "init", MLObjectInit,
    "destroy", MLObjectDestroy,

    "class", MLObjectClass,
    "description", MLObjectDescription,
    "equals*?", MLObjectEquals,
    "hash", MLObjectHash,

    "copy", MLObjectCopy,
    "mutable-copy", MLObjectMutableCopy,

    "compare-to*", MLObjectCompareTo,
    "is-less-than*?", MLObjectIsLessThan,
    "is-less-than-or-equals*?", MLObjectIsLessThanOrEquals,
    "is-greater-than*?", MLObjectIsGreaterThan,
    "is-greater-than-or-equals*?", MLObjectIsGreaterThanOrEquals,

    "is-object?", MLObjectIsObject,
    "is-null?", MLObjectIsNull,
    "is-class?", MLObjectIsClass,
    "is-block?", MLObjectIsBlock,

    "is-boolean?", MLObjectIsBoolean,
    "is-number?", MLObjectIsNumber,
    "is-word?", MLObjectIsWord,
    "is-date?", MLObjectIsDate,
    "is-data?", MLObjectIsData,
    "is-array?", MLObjectIsArray,
    "is-string?", MLObjectIsString,
    "is-dictionary?", MLObjectIsDictionary,

    "is-truthy?", MLObjectIsTruthy,
    "is-falsy?", MLObjectIsFalsy,

    "is-kind-of*?", MLObjectIsKindOf,
    "is-instance-of*?", MLObjectIsInstanceOf,

    "retain", MLObjectRetain,
    "retain-count", MLObjectRetainCount,
    "release", MLObjectRelease,
    "autorelease", MLObjectAutorelease,

    "perform*arguments*options*", MLObjectPerformArgumentsOptionsBlock,
    "send*arguments*options*", MLObjectSendArgumentsOptionsBlock,
    "responds-to*?", MLObjectRespondsTo,

    NULL
};
