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

#include "string.h"
#include "helper.h"


#define MLMutableStringMinCapacity 63 // + 1 byte will be allocated for \0


#define meta MLClassStructure(self)
#define that MLStringStructure(self)


static var MLStringMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLStringMake(MLAllocate(MLStringSize), MLString, 1, 0, 0, NULL);
}


MLPointer MLStringMetaDefaultMethods[] = {
    "create", MLStringMetaCreate,
    NULL
};


static var MLStringInit(var class, var self, var command, var arguments, var options) {
    self = MLSuper(command, arguments, options);
    when (self) {
        that.retainCount = 1;
        that.capacity = 0;
        that.count = 0;
        that.characters = NULL;
    }
    return self;
}


static var MLStringInitWithString(var class, var self, var command, var arguments, var options) {
    var string = MLArgument(0);
    self = MLSuper(command, arguments, options);
    when (self) {
        const var count = MLCount(string);
        const MLInteger countInteger = MLIntegerFrom(count);
        const MLInteger integerCapacity = countInteger;

        that.retainCount = 1;
        that.capacity = integerCapacity;
        that.count = countInteger;
        that.characters = NULL;

        if (countInteger > 0) {
            MLCharacter *characters = MLStringStructure(string).characters;
            that.characters = MLAllocate(sizeof(MLCharacter) * (countInteger + 1));
            memcpy(that.characters, characters, countInteger + 1);
        }
    }
    return self;
}


static var MLStringDestroy(var class, var self, var command, var arguments, var options) {
    MLFree(that.characters);
    MLFree(&that);
    return null;
}


static var MLStringCount(var class, var self, var command, var arguments, var options) {
    return N(that.count);
}


static var MLStringContains(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringIsEmpty(var class, var self, var command, var arguments, var options) {
    return B(that.count == 0);
}


static var MLStringIsInline(var class, var self, var command, var arguments, var options) {
    return B(that.class == MLInlineString.pointer);
}


static var MLStringIsMutable(var class, var self, var command, var arguments, var options) {
    return B(that.class == MLMutableString.pointer);
}


static var MLStringFirst(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringSecond(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringThird(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringRest(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringLast(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCodeAt(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCodesAt(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCharacters(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCodes(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringLines(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringParagraphs(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringIndexOf(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringLastIndexOf(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringIndexesOf(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringBeginsWith(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringEndsWith(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringUppercased(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringLowercased(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCapitalized(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringIsString(var class, var self, var command, var arguments, var options) {
    return yes;
}


static var MLStringDescription(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringEquals(var class, var self, var command, var arguments, var options) {
    const var string = MLArgument(0);
    unless (MLIsString(string)) return no;

    const MLInteger count = MLStringStructure(string).count;
    if (count != that.count) return no;
    if (count == 0) return yes;

    MLCharacter* characters = MLStringStructure(string).characters;
    const MLInteger result = strncmp(characters, that.characters, that.count);

    return B(result == 0);
}


static var MLStringHash(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLStringCopy(var class, var self, var command, var arguments, var options) {
    return MLRetain(self);
}


static var MLStringMutableCopy(var class, var self, var command, var arguments, var options) {
    var mutableCopy = MLCreate(MLMutableString);
    return MLInitWithArray(mutableCopy, self);
}


MLPointer MLStringDefaultMethods[] = {
    "init", MLStringInit,
    "init_with_string*", MLStringInitWithString,
    "destroy", MLStringDestroy,

    "count", MLStringCount,
    "contains*", MLStringContains,

    "is_empty?", MLStringIsEmpty,
    "is_inline?", MLStringIsInline,
    "is_mutable?", MLStringIsMutable,

    "first", MLStringFirst,
    "first*", MLStringFirst,

    "second", MLStringSecond,
    "third", MLStringThird,
    "rest", MLStringRest,

    "last", MLStringLast,
    "last*", MLStringLast,

    "code_at*", MLStringCodeAt,
    "codes_at*", MLStringCodesAt,

    "characters", MLStringCharacters,
    "codes", MLStringCodes,

    "lines", MLStringLines,
    "paragraphs", MLStringParagraphs,

    "index_of*", MLStringIndexOf,
    "last_index_of*", MLStringLastIndexOf,
    "indexes_of*", MLStringIndexesOf,

    "begins_with*", MLStringBeginsWith,
    "ends_with*", MLStringEndsWith,

    "uppercased", MLStringUppercased,
    "lowercased", MLStringLowercased,
    "capitalized", MLStringCapitalized,

    "is_string?", MLStringIsString,

    "description", MLStringDescription,
    "equals*?", MLStringEquals,
    "hash", MLStringHash,
    "copy", MLStringCopy,
    "mutable_copy", MLStringMutableCopy,
    NULL
};


static var MLInlineStringMetaCreate(var class, var self, var command, var arguments, var options) {
    MLError("Can't create an inline string, inline strings can only be inlined");
    return null;
}


MLPointer MLInlineStringMetaDefaultMethods[] = {
    "create", MLInlineStringMetaCreate,
    NULL
};


static var MLInlineStringRetain(var class, var self, var command, var arguments, var options) {
    MLError("Can't retain an inline string, you have to copy it");
    return null;
}


static var MLInlineStringRetainCount(var class, var self, var command, var arguments, var options) {
    return N(-1);
}


static var MLInlineStringRelease(var class, var self, var command, var arguments, var options) {
    MLError("Can't release an inline string because it can't be retained in the first place");
    return null;
}


static var MLInlineStringAutorelease(var class, var self, var command, var arguments, var options) {
    MLError("Can't autorelease an inline string because it can't be retained in the first place");
    return null;
}


MLPointer MLInlineStringDefaultMethods[] = {
    "retain", MLInlineStringRetain,
    "retainCount", MLInlineStringRetainCount,
    "release", MLInlineStringRelease,
    "autorelease", MLInlineStringAutorelease,
};


static var MLMutableStringMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLStringMake(MLAllocate(MLStringSize), MLMutableString, 1, 0, 0, NULL);
}


MLPointer MLMutableStringMetaDefaultMethods[] = {
    NULL
};


static var MLMutableStringInitWithCapacity(var class, var self, var command, var arguments, var options) {
    var capacity = MLArgument(0);
    self = MLSuper(IS("init"), null, null);
    when (self) {
        MLIncreaseCapacity(self, capacity);
    }
    return self;
}


static var MLMutableStringCapacity(var class, var self, var command, var arguments, var options) {
    return N(that.capacity);
}


static var MLMutableStringIncreaseCapacity(var class, var self, var command, var arguments, var options) {
    const var capacity = MLArgument(0);

    MLInteger integerCapacity = MLIntegerFrom(capacity);
    if (integerCapacity < that.capacity) return self;
    if (integerCapacity < MLMutableStringMinCapacity) integerCapacity = MLMutableStringMinCapacity;
    integerCapacity = MLHelperRoundUpToPowerOfTwo(integerCapacity) - 1;

    that.capacity = integerCapacity;
    that.characters = MLResize(that.characters, sizeof(MLCharacter) * (integerCapacity + 1));

    return self;
}


static var MLMutableStringReplaceWith(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringPrepend(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringAppend(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableStringCopy(var class, var self, var command, var arguments, var options) {
    var copy = MLCreate(MLString);
    return MLInitWithString(copy, self);
}


static var MLMutableStringMutableCopy(var class, var self, var command, var arguments, var options) {
    var mutableCopy = MLCreate(MLMutableString);
    return MLInitWithString(mutableCopy, self);
}


MLPointer MLMutableStringDefaultMethods[] = {
    "init_with_capacity*", MLMutableStringInitWithCapacity,

    "capacity", MLMutableStringCapacity,
    "increase_capacity*", MLMutableStringIncreaseCapacity,

    "replace*with*", MLMutableStringReplaceWith,
    "prepend*", MLMutableStringPrepend,
    "append*", MLMutableStringAppend,

    "copy", MLMutableStringCopy,
    "mutable_copy", MLMutableStringMutableCopy,
    NULL
};
