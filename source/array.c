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

#include "array.h"
#include "helper.h"


#define MLMutableArrayMinCapacity 16


#define meta MLClassStructure(self)
#define that MLArrayStructure(self)


static var MLArrayMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLArrayMake(MLAllocate(MLArraySize), MLArray, 1, 0, 0, NULL);
}


static var MLArrayMetaNewWithArray(var class, var self, var command, var arguments, var options) {
    var array = MLArgument(0);
    var new = MLCreate(self);
    return MLInitWithArray(new, array);
}


MLPointer MLArrayMetaDefaultMethods[] = {
    "create", MLArrayMetaCreate,
    "new-with-array*", MLArrayMetaNewWithArray,
    NULL
};


static var MLArrayInit(var class, var self, var command, var arguments, var options) {
    self = MLSuper(command, arguments, options);
    when (self) {
        that.count = 0;
        that.capacity = 0;
        that.objects = NULL;
    }
    return self;
}


static var MLArrayInitWithArray(var class, var self, var command, var arguments, var options) {
    var array = MLArgument(0);
    self = MLSuper(IS("init"), null, null);
    when (self) {
        const MLInteger count = MLIntegerFrom(MLCount(array));
        const MLInteger capacity = count;

        var *objects = MLAllocateAndClear(MLVariableSize * capacity);
        for (MLInteger index = 0; index < count; index += 1) {
            var object = MLArrayStructure(array).objects[index];
            objects[index] = object;
            MLRetain(object);
        }

        that.count = count;
        that.capacity = capacity;
        that.objects = objects;
    }
    return self;
}


static var MLArrayDestroy(var class, var self, var command, var arguments, var options) {
    for (MLInteger index = 0; index < that.count; index += 1) {
        MLRelease(that.objects[index]);
    }
    MLFree(that.objects);
    MLFree(&that);
    return null;
}


static var MLArrayCount(var class, var self, var command, var arguments, var options) {
    return N(that.count);
}


static var MLArrayContains(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var index = MLIndexOf(self, object);
    return B(MLIntegerFrom(index) >= 0);
}


static var MLArrayContainsAny(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    each (object, index, objects) {
        when (MLContains(self, object)) return yes;
    }
    return no;
}


static var MLArrayContainsAll(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    each (object, index, objects) {
        unless (MLContains(self, object)) return no;
    }
    return yes;
}


static var MLArrayIsEmpty(var class, var self, var command, var arguments, var options) {
    return B(that.count == 0);
}


static var MLArrayIsInline(var class, var self, var command, var arguments, var options) {
    return B(that.class == MLInlineArray.pointer);
}


static var MLArrayIsMutable(var class, var self, var command, var arguments, var options) {
    return B(that.class == MLMutableArray.pointer);
}


static var MLArrayAt(var class, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    MLInteger integerIndex = MLIntegerFrom(index);
    if (integerIndex < 0 || integerIndex >= that.count) return null;
    return that.objects[integerIndex];
}


static var MLArrayAtMany(var class, var self, var command, var arguments, var options) {
    var indexes = MLArgument(0);
    var objects = MLCreate(MLMutableArray);
    MLInitWithCapacity(objects, MLCount(indexes));
    each (index, indexOfIndex, indexes) {
        const MLInteger integerIndex = MLIntegerFrom(index);
        if (integerIndex < 0 || integerIndex >= that.count) continue;
        var object = MLAt(self, index);
        MLAdd(objects, object);
    }
    var copy = MLCopy(objects);
    MLRelease(objects);
    return MLAutorelease(copy);
}


static var MLArrayAtCount(var class, var self, var command, var arguments, var options) {
    // TODO: revisit this method to bahave properly when index or count is negative.
    var index = MLArgument(0);
    var count = MLArgument(1);
    var mutable = MLNewWithCapacity(MLMutableArray, count);
    MLInteger integerIndex = MLIntegerFrom(index);
    MLInteger integerCount = MLIntegerFrom(count);
    if (integerIndex < 0) MLError("Index < 0");
    if (integerIndex + integerCount > that.count) integerCount = that.count - integerIndex;
    for (MLInteger i = integerIndex; i < integerIndex + integerCount; i += 1) MLAdd(mutable, that.objects[i]);
    var copy = MLCopy(mutable);
    MLRelease(mutable);
    return MLAutorelease(copy);
}


static var MLArrayIndexOf(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    for (MLInteger index = 0; index < that.count; index += 1) {
        var equals = MLEquals(that.objects[index], object);
        when (equals) return N(index);
    }
    return N(-1);
}


static var MLArrayLastIndexOf(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    for (MLInteger index = that.count - 1; index >= 0; index -= 1) {
        var equals = MLEquals(that.objects[index], object);
        when (equals) return N(index);
    }
    return N(-1);
}


static var MLArrayIndexesOf(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var indexes = MLNewWithCapacity(MLMutableArray, N(that.count));
    for (MLInteger index = 0; index < that.count; index += 1) {
        var equals = MLEquals(that.objects[index], object);
        when (equals) MLAdd(indexes, N(index));
    }
    var copy = MLCopy(indexes);
    MLRelease(indexes);
    return MLAutorelease(copy);
}


static var MLArrayFirst(var class, var self, var command, var arguments, var options) {
    if (that.count == 0) return null;
    return that.objects[0];
}


static var MLArrayFirstCount(var class, var self, var command, var arguments, var options) {
    var count = MLArgument(0);
    if (MLDecimalFrom(count) >= that.count) return MLAutorelease(MLCopy(self));
    var index = N(0);
    return MLAtCount(self, index, count);
}


static var MLArrayLast(var class, var self, var command, var arguments, var options) {
    if (that.count == 0) return null;
    return that.objects[that.count - 1];
}


static var MLArrayLastCount(var class, var self, var command, var arguments, var options) {
    var count = MLArgument(0);
    const MLDecimal decimalCount = MLDecimalFrom(count);
    if (decimalCount >= that.count) return MLAutorelease(MLCopy(self));
    var index = N(that.count - decimalCount);
    return MLAtCount(self, index, count);
}


static var MLArraySecond(var class, var self, var command, var arguments, var options) {
    if (that.count < 2) return null;
    return that.objects[1];
}


static var MLArrayThird(var class, var self, var command, var arguments, var options) {
    if (that.count < 3) return null;
    return that.objects[2];
}


static var MLArrayRest(var class, var self, var command, var arguments, var options) {
    return MLWithoutAt(self, N(0));
}


static var MLArrayWith(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    return MLWithMany(self, IA(object));
}


static var MLArrayWithMany(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var mutable = MLMutableCopy(self);
    var index = N(that.count);
    MLInsertManyAt(mutable, objects, index);
    var copy = MLCopy(mutable);
    MLRelease(mutable);
    return MLAutorelease(copy);
}


static var MLArrayWithAt(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var index = MLArgument(1);
    return MLWithManyAt(self, IA(object), index);
}


static var MLArrayWithManyAt(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var index = MLArgument(1);
    var mutable = MLMutableCopy(self);
    MLInsertManyAt(mutable, objects, index);
    var copy = MLCopy(mutable);
    MLRelease(mutable);
    return MLAutorelease(copy);
}


static var MLArrayWithBefore(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var before = MLArgument(1);
    return MLWithManyBefore(self, IA(object), before);
}


static var MLArrayWithManyBefore(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var before = MLArgument(1);
    var index = MLIndexOf(self, before);
    if (MLIntegerFrom(index) < 0) index = N(that.count);
    return MLWithManyAt(self, objects, index);
}


static var MLArrayWithAfter(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var after = MLArgument(1);
    return MLWithManyAfter(self, IA(object), after);
}


static var MLArrayWithManyAfter(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var after = MLArgument(1);
    var index = MLLastIndexOf(self, after);
    if (MLIntegerFrom(index) < 0) index = N(that.count);
    return MLWithManyAt(self, objects, index);
}


static var MLArrayWithReplacing(var class, var self, var command, var arguments, var options) {
    var replacement = MLArgument(0);
    var object = MLArgument(1);
    return MLWithManyReplacing(self, IA(replacement), object);
}


static var MLArrayWithManyReplacing(var class, var self, var command, var arguments, var options) {
    var replacements = MLArgument(0);
    var object = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLArrayWithReplacingAt(var class, var self, var command, var arguments, var options) {
    var replacement = MLArgument(0);
    var index = MLArgument(1);
    return MLWithManyReplacingAt(self, IA(replacement), index);
}


static var MLArrayWithManyReplacingAt(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var index = MLArgument(1);
    var mutable = MLMutableCopy(self);
    MLReplaceAtWithMany(mutable, index, objects);
    var copy = MLCopy(mutable);
    MLRelease(mutable);
    return MLAutorelease(copy);
}


static var MLArrayWithReplacingAtCount(var class, var self, var command, var arguments, var options) {
    var replacement = MLArgument(0);
    var index = MLArgument(1);
    var count = MLArgument(2);
    return MLWithManyReplacingAtCount(self, IA(replacement), index, count);
}


static var MLArrayWithManyReplacingAtCount(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var index = MLArgument(1);
    var count = MLArgument(2);
    var mutable = MLMutableCopy(self);
    MLReplaceAtCountWithMany(mutable, index, count, objects);
    var copy = MLCopy(mutable);
    MLRelease(mutable);
    return MLAutorelease(copy);
}


static var MLArrayWithout(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    return MLWithoutMany(self, IA(object));
}


static var MLArrayWithoutMany(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var mutable = MLMutableCopy(self);
    MLRemoveMany(mutable, objects);
    var copy = MLCopy(mutable);
    MLRelease(mutable);
    return MLAutorelease(copy);
}


static var MLArrayWithoutAt(var class, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    return MLWithoutAtCount(self, index, N(1));
}


static var MLArrayWithoutAtMany(var class, var self, var command, var arguments, var options) {
    var indexes = MLArgument(0);
    var mutable = MLMutableCopy(self);
    MLRemoveAtMany(mutable, indexes);
    var copy = MLCopy(mutable);
    MLRelease(mutable);
    return MLAutorelease(copy);
}


static var MLArrayWithoutAtCount(var class, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    var count = MLArgument(1);
    var mutable = MLMutableCopy(self);
    MLRemoveAtCount(mutable, index, count);
    var copy = MLCopy(mutable);
    MLRelease(mutable);
    return MLAutorelease(copy);
}


static var MLArrayReversed(var class, var self, var command, var arguments, var options) {
    var mutable = MLNewWithCapacity(MLMutableArray, N(that.count));
    for (MLInteger index = that.count - 1; index >= 0; index -= 1) MLAdd(mutable, that.objects[index]);
    var copy = MLCopy(mutable);
    MLRelease(mutable);
    return MLAutorelease(copy);
}


static var MLArraySorted(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLArrayPluck(var class, var self, var command, var arguments, var options) {
    var key = MLArgument(0);
    MLError("TODO: implement.");
    return null;
}


static var MLArrayMin(var class, var self, var command, var arguments, var options) {
    var min = MLFirst(self);
    each (object, index, self) {
        when (MLIsLessThan(object, min)) min = object;
    }
    return min;
}


static var MLArrayMax(var class, var self, var command, var arguments, var options) {
    var max = MLFirst(self);
    each (object, index, self) {
        when (MLIsGreaterThan(object, max)) max = object;
    }
    return max;
}


static var MLArrayIsArray(var class, var self, var command, var arguments, var options) {
    return yes;
}


static var MLArrayDescription(var class, var self, var command, var arguments, var options) {
    MLWarning("TODO: implement method -description for arrays");
    return null;
}


static var MLArrayEquals(var class, var self, var command, var arguments, var options) {
    var array = MLArgument(0);
    unless (MLIsArray(array)) return no;

    const MLInteger count = MLArrayStructure(array).count;
    if (count != that.count) return no;

    for (MLInteger index = 0; index < count; index += 1) {
        var object1 = that.objects[index];
        var object2 = MLArrayStructure(array).objects[index];

        const bool isNull1 = MLIsNull(object1);
        const bool isNull2 = MLIsNull(object2);
        if (isNull1 != isNull2) return no;

        unless (MLEquals(object1, object2)) return no;
    }
    return yes;
}


static var MLArrayCopy(var class, var self, var command, var arguments, var options) {
    return MLRetain(self);
}


static var MLArrayMutableCopy(var class, var self, var command, var arguments, var options) {
    var mutableCopy = MLCreate(MLMutableArray);
    return MLInitWithArray(mutableCopy, self);
}


MLPointer MLArrayDefaultMethods[] = {
    "init", MLArrayInit,
    "init-with-array*", MLArrayInitWithArray,
    "destroy", MLArrayDestroy,

    "count", MLArrayCount,

    "contains*?", MLArrayContains,
    "contains-all*?", MLArrayContainsAll,
    "contains-any*?", MLArrayContainsAny,

    "is-empty?", MLArrayIsEmpty,
    "is-inline?", MLArrayIsInline,
    "is-mutable?", MLArrayIsMutable,

    "at*", MLArrayAt,
    "at-many*", MLArrayAtMany,
    "at*count*", MLArrayAtCount,

    "index-of*", MLArrayIndexOf,
    "last-index-of*", MLArrayLastIndexOf,
    "indexes-of*", MLArrayIndexesOf,

    "first", MLArrayFirst,
    "first*", MLArrayFirstCount,

    "last", MLArrayLast,
    "last*", MLArrayLastCount,

    "second", MLArraySecond,
    "third", MLArrayThird,
    "rest", MLArrayRest,

    "with*", MLArrayWith,
    "with-many*", MLArrayWithMany,

    "with*at*", MLArrayWithAt,
    "with-many*at*", MLArrayWithManyAt,

    "with*before*", MLArrayWithBefore,
    "with-many*before*", MLArrayWithManyBefore,

    "with*after*", MLArrayWithAfter,
    "with-many*after*", MLArrayWithManyAfter,

    "with*replacing*", MLArrayWithReplacing,
    "with-many*replacing*", MLArrayWithManyReplacing,

    "with*replacing_at*", MLArrayWithReplacingAt,
    "with-many*replacing_at*", MLArrayWithManyReplacingAt,

    "with*replacing_at*count*", MLArrayWithReplacingAtCount,
    "with-many*replacing_at*count*", MLArrayWithManyReplacingAtCount,

    "without*", MLArrayWithout,
    "without-many*", MLArrayWithoutMany,

    "without-at*", MLArrayWithoutAt,
    "without-at-many*", MLArrayWithoutAtMany,
    "without-at*count*", MLArrayWithoutAtCount,

    "reversed", MLArrayReversed,
    "sorted", MLArraySorted,

    "pluck*", MLArrayPluck,

    "min", MLArrayMin,
    "max", MLArrayMax,

    "is-array?", MLArrayIsArray,

    "description", MLArrayDescription,
    "equals*?", MLArrayEquals,

    "copy", MLArrayCopy,
    "mutable-copy", MLArrayMutableCopy,

    NULL
};


static var MLInlineArrayMetaCreate(var class, var self, var command, var arguments, var options) {
    MLError("Can't create inline array, inline arrays can only be inlined");
    return null;
}


MLPointer MLInlineArrayMetaDefaultMethods[] = {
    "create", MLInlineArrayMetaCreate,
    NULL
};


static var MLInlineArrayCopy(var class, var self, var command, var arguments, var options) {
    return MLNewWithArray(MLArray, self);
}


static var MLInlineArrayMutableCopy(var class, var self, var command, var arguments, var options) {
    return MLNewWithArray(MLMutableArray, self);
}


static var MLInlineArrayRetain(var class, var self, var command, var arguments, var options) {
    MLError("Can't retain an inline array, you have to copy it");
    return null;
}


static var MLInlineArrayRetainCount(var class, var self, var command, var arguments, var options) {
    return N(-1);
}


static var MLInlineArrayRelease(var class, var self, var command, var arguments, var options) {
    MLError("Can't release an inline array because it can't be retained in the first place");
    return null;
}


static var MLInlineArrayAutorelease(var class, var self, var command, var arguments, var options) {
    MLError("Can't autorelease an inline array because it can't be retained in the first place");
    return null;
}


MLPointer MLInlineArrayDefaultMethods[] = {
    "copy", MLInlineArrayCopy,
    "mutable-copy", MLInlineArrayMutableCopy,
    "retain", MLInlineArrayRetain,
    "retainCount", MLInlineArrayRetainCount,
    "release", MLInlineArrayRelease,
    "autorelease", MLInlineArrayAutorelease,
};


static var MLMutableArrayMetaCreate(var class, var self, var command, var arguments, var options) {
    return MLArrayMake(MLAllocate(MLArraySize), MLMutableArray, 1, 0, 0, NULL);
}


static var MLMutableArrayMetaNewWithCapacity(var class, var self, var command, var arguments, var options) {
    var capacity = MLArgument(0);
    var new = MLCreate(self);
    return MLInitWithCapacity(new, capacity);
}


MLPointer MLMutableArrayMetaDefaultMethods[] = {
    "create", MLMutableArrayMetaCreate,
    "new-with-capacity*", MLMutableArrayMetaNewWithCapacity,
    NULL
};


static var MLMutableArrayInitWithCapacity(var class, var self, var command, var arguments, var options) {
    var capacity = MLArgument(0);
    self = MLSuper(IS("init"), null, null);
    when (self) {
        MLIncreaseCapacity(self, capacity);
    }
    return self;
}


static var MLMutableArrayCapacity(var class, var self, var command, var arguments, var options) {
    return N(that.capacity);
}


static var MLMutableArrayIncreaseCapacity(var class, var self, var command, var arguments, var options) {
    var capacity = MLArgument(0);

    MLInteger integerCapacity = MLIntegerFrom(capacity);
    if (integerCapacity < that.capacity) return self;
    if (integerCapacity < MLMutableArrayMinCapacity) integerCapacity = MLMutableArrayMinCapacity;

    integerCapacity = MLHelperRoundUpToPowerOfTwo(integerCapacity);

    that.capacity = integerCapacity;
    that.objects = MLResize(that.objects, MLVariableSize * integerCapacity);

    return self;
}


static var MLMutableArrayAdd(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    if (that.capacity <= that.count) MLIncreaseCapacity(self, N(that.count + 1));
    that.objects[that.count] = MLRetain(object);
    that.count += 1;
    return self;
}


static var MLMutableArrayAddMany(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var objectsCount = MLCount(objects);
    const MLInteger requiredCapacity = that.count + MLIntegerFrom(objectsCount);
    if (that.capacity < requiredCapacity) MLIncreaseCapacity(self, N(requiredCapacity));
    each (object, index, objects) {
        that.objects[that.count] = MLRetain(object);
        that.count += 1;
    }
    return self;
}


static var MLMutableArrayInsert(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    return MLInsertManyAt(self, IA(object), N(0));
}


static var MLMutableArrayInsertMany(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    return MLInsertManyAt(self, objects, N(0));
}


static var MLMutableArrayInsertAt(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var index = MLArgument(1);
    return MLInsertManyAt(self, IA(object), index);
}


static var MLMutableArrayInsertManyAt(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var index = MLArgument(1);

    const MLInteger integerIndex = MLIntegerFrom(index);
    const bool isIndexOutOfBounds = integerIndex < 0 || integerIndex > that.count;
    if (isIndexOutOfBounds) MLError("Can't insert many objects at index, index is out of bounds");

    const MLInteger numberOfObjects = MLIntegerFrom(MLCount(objects));
    const MLInteger capacity = that.count + numberOfObjects;
    MLIncreaseCapacity(self, N(capacity));

    for (MLInteger i = that.count - 1; i >= integerIndex; i -= 1) {
        that.objects[i + numberOfObjects] = that.objects[i];
    }

    for (MLInteger j = 0; j < numberOfObjects; j += 1) {
        const var object = MLAt(objects, N(j));
        that.objects[j + integerIndex] = object;
        MLRetain(object);
    }

    that.count = capacity;
    return self;
}


static var MLMutableArrayInsertBefore(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var before = MLArgument(1);
    var index = MLIndexOf(self, before);
    if (MLIntegerFrom(index) < 0) index = N(0);
    return MLInsertManyAt(self, IA(object), index);
}


static var MLMutableArrayInsertManyBefore(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var before = MLArgument(1);
    var index = MLIndexOf(self, before);
    if (MLIntegerFrom(index) < 0) index = N(0);
    return MLInsertManyAt(self, objects, index);
}


static var MLMutableArrayInsertAfter(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var after = MLArgument(1);
    const var index = MLLastIndexOf(self, after);
    const MLInteger integerIndex = MLIntegerFrom(index);
    const MLInteger insertionIndex = integerIndex >= 0 ? integerIndex + 1 : that.count;
    return MLInsertManyAt(self, IA(object), N(insertionIndex));
}


static var MLMutableArrayInsertManyAfter(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var after = MLArgument(1);
    var index = MLIndexOf(self, after);
    if (MLIntegerFrom(index) < 0) index = N(that.count);
    return MLInsertManyAt(self, objects, index);
}


static var MLMutableArrayReplaceWith(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var replacement = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayReplaceWithMany(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var replacements = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayReplaceAtWith(var class, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    var replacement = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayReplaceAtWithMany(var class, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    var replacements = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayReplaceAtCountWith(var class, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    var count = MLArgument(1);
    var replacement = MLArgument(2);
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayReplaceAtCountWithMany(var class, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    var count = MLArgument(1);
    var replacements = MLArgument(2);
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayRemove(var class, var self, var command, var arguments, var options) {
    var object = MLArgument(0);
    var indexes = MLIndexesOf(self, object);
    return MLRemoveAtMany(self, indexes);
}


static var MLMutableArrayRemoveMany(var class, var self, var command, var arguments, var options) {
    var objects = MLArgument(0);
    var indexes = MLNewWithCapacity(MLMutableArray, N(that.count));
    each (object, index, objects) {
        var indexesOfObject = MLIndexesOf(self, object);
        MLAddMany(indexes, indexesOfObject);
    }
    MLRemoveAtMany(self, indexes);
    MLRelease(indexes);
    return self;
}


static var MLMutableArrayRemoveAt(var class, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayRemoveAtMany(var class, var self, var command, var arguments, var options) {
    var indexes = MLArgument(0);
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayRemoveAtCount(var class, var self, var command, var arguments, var options) {
    var index = MLArgument(0);
    var count = MLArgument(1);
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayRemoveAll(var class, var self, var command, var arguments, var options) {
    for (MLInteger index = 0; index < that.count; index += 1) {
        MLRelease(that.objects[index]);
        that.objects[index] = null;
    }
    that.count = 0;
    return self;
}


static var MLMutableArrayReverse(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArraySort(var class, var self, var command, var arguments, var options) {
    MLError("TODO: implement.");
    return null;
}


static var MLMutableArrayIsMutable(var class, var self, var command, var arguments, var options) {
    return yes;
}


MLPointer MLMutableArrayDefaultMethods[] = {
    "init-with-capacity*", MLMutableArrayInitWithCapacity,

    "capacity", MLMutableArrayCapacity,
    "increase-capacity*", MLMutableArrayIncreaseCapacity,

    "add*", MLMutableArrayAdd,
    "add-many*", MLMutableArrayAddMany,

    "insert*", MLMutableArrayInsert,
    "insert-many*", MLMutableArrayInsertMany,

    "insert*at*", MLMutableArrayInsertAt,
    "insert-many*at*", MLMutableArrayInsertManyAt,

    "insert*before*", MLMutableArrayInsertBefore,
    "insert-many*before*", MLMutableArrayInsertManyBefore,

    "insert*after*", MLMutableArrayInsertAfter,
    "insert-many*after*", MLMutableArrayInsertManyAfter,

    "replace*with*", MLMutableArrayReplaceWith,
    "replace*with_many*", MLMutableArrayReplaceWithMany,

    "replace-at*with*", MLMutableArrayReplaceAtWith,
    "replace-at*with_many*", MLMutableArrayReplaceAtWithMany,

    "replace-at*count*with*", MLMutableArrayReplaceAtCountWith,
    "replace-at*count*with_many*", MLMutableArrayReplaceAtCountWithMany,

    "remove*", MLMutableArrayRemove,
    "remove-many*", MLMutableArrayRemoveMany,

    "remove-at*", MLMutableArrayRemoveAt,
    "remove-at-many*", MLMutableArrayRemoveAtMany,
    "remove-at*count*", MLMutableArrayRemoveAtCount,
    "remove-all", MLMutableArrayRemoveAll,

    "reverse", MLMutableArrayReverse,
    "sort", MLMutableArraySort,

    "is-mutable?", MLMutableArrayIsMutable,
    NULL
};
