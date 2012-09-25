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


static var MLStringMetaCreate(var context, var self, var command, var arguments, var options) {
    return MLStringMake(MLAllocate(MLStringSize), self, 1, 0, 0, NULL);
}


static var MLStringMetaNewWithString(var context, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    var new = MLCreate(self);
    return MLInitWithString(new, string);
}


MLPointer MLStringMetaDefaultMethods[] = {
    "create", MLStringMetaCreate,
    "new-with-string*", MLStringMetaNewWithString,
    NULL
};


static var MLStringInit(var context, var self, var command, var arguments, var options) {
    self = MLSuper(command, arguments, options);
    when (self) {
        that.retainCount = 1;
        that.capacity = 0;
        that.count = 0;
        that.characters = NULL;
    }
    return self;
}


static var MLStringInitWithString(var context, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    self = MLSuper(IS("init"), arguments, options);
    when (self) {
        const var count = MLCount(string);
        const MLInteger countInteger = MLIntegerFrom(count);
        const MLInteger integerCapacity = countInteger;

        that.capacity = integerCapacity;
        that.count = countInteger;
        that.characters = NULL;

        if (countInteger > 0) {
            char *characters = MLStringStructure(string).characters;
            that.characters = MLAllocate(sizeof(char) * (countInteger + 1));
            memcpy(that.characters, characters, countInteger + 1);
        }
    }
    return self;
}


static var MLStringDestroy(var context, var self, var command, var arguments, var options) {
    MLFree(that.characters);
    MLFree(&that);
    return null;
}


static var MLStringCount(var context, var self, var command, var arguments, var options) {
    return N(that.count);
}


static var MLStringContains(var context, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    var index = MLIndexOf(self, string);
    return MLIsGreaterThanOrEquals(index, N(0));
}


static var MLStringIsEmpty(var context, var self, var command, var arguments, var options) {
    return B(that.count == 0);
}


static var MLStringIsInline(var context, var self, var command, var arguments, var options) {
    return B(that.class == MLInlineString.pointer);
}


static var MLStringIsMutable(var context, var self, var command, var arguments, var options) {
    return B(that.class == MLMutableString.pointer);
}


static var MLStringAt(var context, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    MLError("TODO: implement.");
    return null;
}


static var MLStringAtMany(var context, var self, var command, var arguments, var options) {
    var indexes = MLArgument(0);
    MLError("TODO: implement.");
    return null;
}


static var MLStringAtCount(var context, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    var count = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLStringIndexOf(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringLastIndexOf(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringIndexesOf(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringFirst(var context, var self, var command, var arguments, var options) {
    if (that.count <= 0) return null;
    var index = N(0);
    var count = N(1);
    return MLAtCount(self, index, count);
}


static var MLStringFirstCount(var context, var self, var command, var arguments, var options) {
    var count = MLArgument(0);
    if (MLDecimalFrom(count) <= that.count) return MLAutorelease(MLCopy(self));
    var index = N(0);
    return MLAtCount(self, index, count);
}


static var MLStringLast(var context, var self, var command, var arguments, var options) {
    if (that.count <= 0) return null;
    var index = N(that.count - 1);
    var count = N(1);
    return MLAtCount(self, index, count);
}


static var MLStringLastCount(var context, var self, var command, var arguments, var options) {
    var count = MLArgument(0);
    const MLDecimal decimalCount = MLDecimalFrom(count);
    if (decimalCount <= that.count) return MLAutorelease(MLCopy(self));
    var index = N(that.count - decimalCount);
    return MLAtCount(self, index, count);
}


static var MLStringSecond(var context, var self, var command, var arguments, var options) {
    if (that.count <= 1) return null;
    var index = N(1);
    var count = N(1);
    return MLAtCount(self, index, count);
}


static var MLStringThird(var context, var self, var command, var arguments, var options) {
    if (that.count <= 2) return null;
    var index = N(2);
    var count = N(1);
    return MLAtCount(self, index, count);
}


static var MLStringRest(var context, var self, var command, var arguments, var options) {
    if (that.count <= 1) return null;
    var index = N(1);
    var count = N(that.count - 1);
    return MLAtCount(self, index, count);
}


static var MLStringWith(var context, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    return MLWithMany(self, IA(string));
}


static var MLStringWithMany(var context, var self, var command, var arguments, var options) {
    var strings = MLArgument(0);
    MLError("TODO: implement.");
    return null;
}


static var MLStringWithAt(var context, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    var index = MLArgument(1);
    return MLWithManyAt(self, IA(string), index);
}


static var MLStringWithManyAt(var context, var self, var command, var arguments, var options) {
    var strings = MLArgument(0);
    var index = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLStringWithBefore(var context, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    var before = MLArgument(1);
    return MLWithManyBefore(self, IA(string), before);
}


static var MLStringWithManyBefore(var context, var self, var command, var arguments, var options) {
    var strings = MLArgument(0);
    var before = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLStringWithAfter(var context, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    var after = MLArgument(1);
    return MLWithManyAfter(self, IA(string), after);
}


static var MLStringWithManyAfter(var context, var self, var command, var arguments, var options) {
    var strings = MLArgument(0);
    var after = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLStringWithout(var context, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    return MLWithoutMany(self, IA(string));
}


static var MLStringWithoutMany(var context, var self, var command, var arguments, var options) {
    var strings = MLArgument(0);
    MLError("TODO: implement.");
    return null;
}


static var MLStringWithoutAt(var context, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    return MLWithoutAtCount(self, index, N(1));
}


static var MLStringWithoutAtCount(var context, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    var count = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLStringWithoutAtMany(var context, var self, var command, var arguments, var options) {
    var indexes = MLArgument(0);
    MLError("TODO: implement.");
    return null;
}


static var MLStringCodeAt(var context, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    const MLInteger integerIndex = MLIntegerFrom(index);
    if (integerIndex >= that.count) return null;
    return W(that.characters[integerIndex]);
}


static var MLStringCodesAt(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCharacters(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCodes(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringLines(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringParagraphs(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringBeginsWith(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringEndsWith(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringReversed(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringUppercased(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringLowercased(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCapitalized(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringIsString(var context, var self, var command, var arguments, var options) {
    return yes;
}


static var MLStringDescription(var context, var self, var command, var arguments, var options) {
    return MLAutorelease(MLCopy(self));
}


static var MLStringEquals(var context, var self, var command, var arguments, var options) {
    const var string = MLArgument(0);
    unless (MLIsString(string)) return no;

    const MLInteger count = MLStringStructure(string).count;
    if (count != that.count) return no;
    if (count == 0) return yes;

    char* characters = MLStringStructure(string).characters;
    const MLInteger result = strncmp(characters, that.characters, that.count);

    return B(result == 0);
}


static var MLStringHash(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCopy(var context, var self, var command, var arguments, var options) {
    return MLRetain(self);
}


static var MLStringMutableCopy(var context, var self, var command, var arguments, var options) {
    var mutableCopy = MLCreate(MLMutableString);
    return MLInitWithString(mutableCopy, self);
}


MLPointer MLStringDefaultMethods[] = {
    "init", MLStringInit,
    "init-with-string*", MLStringInitWithString,
    "destroy", MLStringDestroy,

    "count", MLStringCount,
    "contains*", MLStringContains,

    "is-empty?", MLStringIsEmpty,
    "is-inline?", MLStringIsInline,
    "is-mutable?", MLStringIsMutable,

    "at*", MLStringAt,
    "at-many*", MLStringAtMany,
    "at*count*", MLStringAtCount,

    "index-of*", MLStringIndexOf,
    "last-index-of*", MLStringLastIndexOf,
    "indexes-of*", MLStringIndexesOf,

    "first", MLStringFirst,
    "first*", MLStringFirstCount,

    "last", MLStringLast,
    "last*", MLStringLastCount,

    "second", MLStringSecond,
    "third", MLStringThird,
    "rest", MLStringRest,

    "with*", MLStringWith,
    "with-many*", MLStringWithMany,

    "with*at*", MLStringWithAt,
    "with-many*at*", MLStringWithManyAt,

    "with*before*", MLStringWithBefore,
    "with-many*before*", MLStringWithManyBefore,

    "with*after*", MLStringWithAfter,
    "with-many*after*", MLStringWithManyAfter,

    "without*", MLStringWithout,
    "without-many*", MLStringWithoutMany,

    "without-at*", MLStringWithoutAt,
    "without-at*count*", MLStringWithoutAtCount,
    "without-at-many*", MLStringWithoutAtMany,

    "code-at*", MLStringCodeAt,
    "codes-at*", MLStringCodesAt,

    "characters", MLStringCharacters,
    "codes", MLStringCodes,

    "lines", MLStringLines,
    "paragraphs", MLStringParagraphs,

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


static var MLInlineStringMetaCreate(var context, var self, var command, var arguments, var options) {
    MLError("Can't create an inline string, inline strings can only be inlined");
    return null;
}


MLPointer MLInlineStringMetaDefaultMethods[] = {
    "create", MLInlineStringMetaCreate,
    NULL
};


static var MLInlineStringCopy(var context, var self, var command, var arguments, var options) {
    return MLNewWithString(MLString, self);
}


static var MLInlineStringMutableCopy(var context, var self, var command, var arguments, var options) {
    return MLNewWithString(MLMutableString, self);
}


static var MLInlineStringRetain(var context, var self, var command, var arguments, var options) {
    MLError("Can't retain an inline string, you have to copy it");
    return null;
}


static var MLInlineStringRetainCount(var context, var self, var command, var arguments, var options) {
    return N(-1);
}


static var MLInlineStringRelease(var context, var self, var command, var arguments, var options) {
    MLError("Can't release an inline string because it can't be retained in the first place");
    return null;
}


static var MLInlineStringAutorelease(var context, var self, var command, var arguments, var options) {
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


static var MLMutableStringMetaCreate(var context, var self, var command, var arguments, var options) {
    return MLStringMake(MLAllocate(MLStringSize), self, 1, 0, 0, NULL);
}


MLPointer MLMutableStringMetaDefaultMethods[] = {
    NULL
};


static var MLMutableStringInitWithCapacity(var context, var self, var command, var arguments, var options) {
    var capacity = MLArgument(0);
    self = MLSuper(IS("init"), null, null);
    when (self) {
        MLIncreaseCapacity(self, capacity);
    }
    return self;
}


static var MLMutableStringCapacity(var context, var self, var command, var arguments, var options) {
    return N(that.capacity);
}


static var MLMutableStringIncreaseCapacity(var context, var self, var command, var arguments, var options) {
    const var capacity = MLArgument(0);

    MLInteger integerCapacity = MLIntegerFrom(capacity);
    if (integerCapacity < that.capacity) return self;
    if (integerCapacity < MLMutableStringMinCapacity) integerCapacity = MLMutableStringMinCapacity;
    integerCapacity = MLHelperRoundUpToPowerOfTwo(integerCapacity) - 1;

    that.capacity = integerCapacity;
    that.characters = MLResize(that.characters, sizeof(char) * (integerCapacity + 1));

    return self;
}


static var MLMutableStringReplaceWith(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringPrepend(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringAppend(var context, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringCopy(var context, var self, var command, var arguments, var options) {
    var copy = MLCreate(MLString);
    return MLInitWithString(copy, self);
}


static var MLMutableStringMutableCopy(var context, var self, var command, var arguments, var options) {
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

    "copy", MLMutableStringCopy,
    "mutable-copy", MLMutableStringMutableCopy,
    NULL
};
