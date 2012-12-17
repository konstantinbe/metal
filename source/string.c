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
#include "string.h"
#include "helper.h"


#define MLMutableStringMinCapacity 63 // + 1 byte will be allocated for \0


#define meta MLClassStructure(self)
#define that MLStringStructure(self)


static var MLStringMetaCreate(var context, var self, var command, var arguments) {
    return MLStringMake(MLAllocate(MLStringSize), self, 1, 0, 0, NULL);
}


static var MLStringMetaNewWithString(var context, var self, var command, var arguments) {
    var string = MLArgument(0);
    var new = MLCreate(self);
    return MLInitWithString(new, string);
}


MLPointer MLStringMetaDefaultMethods[] = {
    "create", MLStringMetaCreate,
    "new-with-string*", MLStringMetaNewWithString,
    NULL
};


static var MLStringInit(var context, var self, var command, var arguments) {
    self = MLSuper("init");
    when (self) {
        that.retainCount = 1;
        that.capacity = 0;
        that.length = 0;
        that.characters = NULL;
    }
    return self;
}


static var MLStringInitWithString(var context, var self, var command, var arguments) {
    var string = MLArgument(0);
    self = MLSuper("init");
    when (self) {
        const var length = MLLength(string);
        const MLInteger lengthInteger = MLIntegerFrom(length);
        const MLInteger integerCapacity = lengthInteger;

        that.capacity = integerCapacity;
        that.length = lengthInteger;
        that.characters = NULL;

        if (lengthInteger > 0) {
            char *characters = MLStringStructure(string).characters;
            that.characters = MLAllocate(sizeof(char) * (lengthInteger + 1));
            memcpy(that.characters, characters, lengthInteger + 1);
        }
    }
    return self;
}


static var MLStringDestroy(var context, var self, var command, var arguments) {
    MLFree(that.characters);
    MLFree(&that);
    return null;
}


static var MLStringLength(var context, var self, var command, var arguments) {
    return N(that.length);
}


static var MLStringContains(var context, var self, var command, var arguments) {
    var string = MLArgument(0);
    var index = MLIndexOf(self, string);
    return MLIsGreaterThanOrEquals(index, N(0));
}


static var MLStringIsEmpty(var context, var self, var command, var arguments) {
    return B(that.length == 0);
}


static var MLStringIsInline(var context, var self, var command, var arguments) {
    return B(that.class == MLInlineString.pointer);
}


static var MLStringIsMutable(var context, var self, var command, var arguments) {
    return B(that.class == MLMutableString.pointer);
}


static var MLStringIndexOf(var context, var self, var command, var arguments) {
    // TODO: optimize.
    var string = MLArgument(0);
    var indexes = MLIndexesOf(self, string);
    return MLFirst(indexes);
}


static var MLStringLastIndexOf(var context, var self, var command, var arguments) {
    // TODO: optimize.
    var string = MLArgument(0);
    var indexes = MLIndexesOf(self, string);
    return MLLast(indexes);
}


static var MLStringIndexesOf(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringFirstCount(var context, var self, var command, var arguments) {
    var count = MLArgument(0);
    if (MLDecimalFrom(count) <= that.length) return MLAutorelease(MLCopy(self));
    var index = N(0);
    return MLAtCount(self, index, count);
}


static var MLStringLastCount(var context, var self, var command, var arguments) {
    var count = MLArgument(0);
    const MLDecimal decimalCount = MLDecimalFrom(count);
    if (decimalCount <= that.length) return MLAutorelease(MLCopy(self));
    var index = N(that.length - decimalCount);
    return MLAtCount(self, index, count);
}


static var MLStringBeginsWith(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringEndsWith(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringReversed(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringUppercased(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringLowercased(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCapitalized(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringIsString(var context, var self, var command, var arguments) {
    return yes;
}


static var MLStringDescription(var context, var self, var command, var arguments) {
    return MLAutorelease(MLCopy(self));
}


static var MLStringEquals(var context, var self, var command, var arguments) {
    const var string = MLArgument(0);
    unless (MLIsString(string)) return no;

    const MLInteger length = MLStringStructure(string).length;
    if (length != that.length) return no;
    if (length == 0) return yes;

    char* characters = MLStringStructure(string).characters;
    const MLInteger result = strncmp(characters, that.characters, that.length);

    return B(result == 0);
}


static var MLStringHash(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCopy(var context, var self, var command, var arguments) {
    return MLRetain(self);
}


static var MLStringMutableCopy(var context, var self, var command, var arguments) {
    var mutableCopy = MLCreate(MLMutableString);
    return MLInitWithString(mutableCopy, self);
}


MLPointer MLStringDefaultMethods[] = {
    "init", MLStringInit,
    "init-with-string*", MLStringInitWithString,
    "destroy", MLStringDestroy,

    "length", MLStringLength,
    "contains*", MLStringContains,

    "is-empty?", MLStringIsEmpty,
    "is-inline?", MLStringIsInline,
    "is-mutable?", MLStringIsMutable,

    "index-of*", MLStringIndexOf,
    "last-index-of*", MLStringLastIndexOf,
    "indexes-of*", MLStringIndexesOf,

    "first*", MLStringFirstCount,
    "last*", MLStringLastCount,

    "begins-with*", MLStringBeginsWith,
    "ends-with*", MLStringEndsWith,

    "reversed", MLStringReversed,
    "uppercased", MLStringUppercased,
    "lowercased", MLStringLowercased,
    "capitalized", MLStringCapitalized,

    "is-string?", MLStringIsString,

    "description", MLStringDescription,
    "equals*?", MLStringEquals,
    "hash", MLStringHash,
    "copy", MLStringCopy,
    "mutable-copy", MLStringMutableCopy,

    NULL
};


static var MLInlineStringMetaCreate(var context, var self, var command, var arguments) {
    MLError("Can't create an inline string, inline strings can only be inlined");
    return null;
}


MLPointer MLInlineStringMetaDefaultMethods[] = {
    "create", MLInlineStringMetaCreate,
    NULL
};


static var MLInlineStringCopy(var context, var self, var command, var arguments) {
    return MLNewWithString(MLString, self);
}


static var MLInlineStringMutableCopy(var context, var self, var command, var arguments) {
    return MLNewWithString(MLMutableString, self);
}


static var MLInlineStringRetain(var context, var self, var command, var arguments) {
    MLError("Can't retain an inline string, you have to copy it");
    return null;
}


static var MLInlineStringRetainCount(var context, var self, var command, var arguments) {
    return N(-1);
}


static var MLInlineStringRelease(var context, var self, var command, var arguments) {
    MLError("Can't release an inline string because it can't be retained in the first place");
    return null;
}


static var MLInlineStringAutorelease(var context, var self, var command, var arguments) {
    MLError("Can't autorelease an inline string because it can't be retained in the first place");
    return null;
}


MLPointer MLInlineStringDefaultMethods[] = {
    "copy", MLInlineStringCopy,
    "mutable-copy", MLInlineStringMutableCopy,

    "retain", MLInlineStringRetain,
    "retainCount", MLInlineStringRetainCount,
    "release", MLInlineStringRelease,
    "autorelease", MLInlineStringAutorelease,

    NULL
};


static var MLMutableStringMetaCreate(var context, var self, var command, var arguments) {
    return MLStringMake(MLAllocate(MLStringSize), self, 1, 0, 0, NULL);
}


MLPointer MLMutableStringMetaDefaultMethods[] = {
    NULL
};


static var MLMutableStringInitWithCapacity(var context, var self, var command, var arguments) {
    var capacity = MLArgument(0);
    self = MLSuper("init");
    when (self) {
        MLIncreaseCapacity(self, capacity);
    }
    return self;
}


static var MLMutableStringCapacity(var context, var self, var command, var arguments) {
    return N(that.capacity);
}


static var MLMutableStringIncreaseCapacity(var context, var self, var command, var arguments) {
    const var capacity = MLArgument(0);

    MLInteger integerCapacity = MLIntegerFrom(capacity);
    if (integerCapacity < that.capacity) return self;
    if (integerCapacity < MLMutableStringMinCapacity) integerCapacity = MLMutableStringMinCapacity;
    integerCapacity = MLHelperRoundUpToPowerOfTwo(integerCapacity) - 1;

    that.capacity = integerCapacity;
    that.characters = MLResize(that.characters, sizeof(char) * (integerCapacity + 1));

    return self;
}


static var MLMutableStringReplaceWith(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringPrepend(var context, var self, var command, var arguments) {
    // TODO: optimize.
    var prefix = MLArgument(0);
    var copy = MLCopy(self);
    MLClear(self);
    MLAppend(self, prefix);
    MLAppend(self, copy);
    MLRelease(copy);
    return self;
}


static var MLMutableStringAppend(var context, var self, var command, var arguments) {
    var suffix = MLArgument(0);
    char *suffixCharacters = MLStringStructure(suffix).characters;
    const MLInteger suffixLength = MLStringStructure(suffix).length;
    MLIncreaseCapacity(self, N(that.length + suffixLength));
    strncat(that.characters, suffixCharacters, suffixLength);
    return self;
}


static var MLMutableStringClear(var context, var self, var command, var arguments) {
    if (that.length > 0 && that.capacity > 0) {
        that.length = 0;
        that.characters[0] = '\0';
    }
    return self;
}


static var MLMutableStringReverse(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringUppercase(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringLowercase(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringCapitalize(var context, var self, var command, var arguments) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringIsMutable(var context, var self, var command, var arguments) {
    return MLIsKindOf(self, MLMutableString);
}


static var MLMutableStringCopy(var context, var self, var command, var arguments) {
    var copy = MLCreate(MLString);
    return MLInitWithString(copy, self);
}


static var MLMutableStringMutableCopy(var context, var self, var command, var arguments) {
    var mutableCopy = MLCreate(MLMutableString);
    return MLInitWithString(mutableCopy, self);
}


MLPointer MLMutableStringDefaultMethods[] = {
    "init-with-capacity*", MLMutableStringInitWithCapacity,

    "capacity", MLMutableStringCapacity,
    "increase-capacity*", MLMutableStringIncreaseCapacity,

    "replace*with*", MLMutableStringReplaceWith,
    "prepend*", MLMutableStringPrepend,
    "append*", MLMutableStringAppend,
    "clear", MLMutableStringClear,

    "reverse", MLMutableStringReverse,
    "uppercase", MLMutableStringUppercase,
    "lowercase", MLMutableStringLowercase,
    "capitalize", MLMutableStringCapitalize,

    "is-mutable?", MLMutableStringIsMutable,

    "copy", MLMutableStringCopy,
    "mutable-copy", MLMutableStringMutableCopy,

    NULL
};
