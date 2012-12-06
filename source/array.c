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
#include <string.h>


#define MLMutableArrayMinCapacity 16
#define MLMarking (void*)1

#define meta MLClassStructure(self)
#define that MLArrayStructure(self)


static int MLCompare(const void* left, const void* right) {
    const var leftObject = *(const var*)(left);
    const var rightObject = *(const var*)(right);
    const var ordering = MLCompareTo(leftObject, rightObject);
    return MLIntegerFrom(ordering);
}


static var MLMakeAutoreleasedCopyAndReleaseOriginal(var array) {
   var copy = MLCopy(array);
   MLRelease(array);
   return MLAutorelease(copy);
}


static var MLArrayMetaCreate(var context, var self, var command, var arguments) {
    return MLArrayMake(MLAllocate(MLArraySize), MLArray, 1, 0, 0, NULL);
}


static var MLArrayMetaNewWithArray(var context, var self, var command, var arguments) {
    var array = MLArgument(0);
    var new = MLCreate(self);
    return MLInitWithArray(new, array);
}


MLPointer MLArrayMetaDefaultMethods[] = {
    "create", MLArrayMetaCreate,
    "new-with-array*", MLArrayMetaNewWithArray,
    NULL
};


static var MLArrayInit(var context, var self, var command, var arguments) {
    self = MLSuper("init");
    when (self) {
        that.count = 0;
        that.capacity = 0;
        that.objects = NULL;
    }
    return self;
}


static var MLArrayInitWithArray(var context, var self, var command, var arguments) {
    var array = MLArgument(0);
    self = MLSuper("init");
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


static var MLArrayDestroy(var context, var self, var command, var arguments) {
    for (MLInteger index = 0; index < that.count; index += 1) {
        MLRelease(that.objects[index]);
    }
    MLFree(that.objects);
    MLFree(&that);
    return null;
}


static var MLArrayCount(var context, var self, var command, var arguments) {
    return N(that.count);
}


static var MLArrayContains(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var index = MLIndexOf(self, object);
    return B(MLIntegerFrom(index) >= 0);
}


static var MLArrayContainsAny(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    each (object, index, objects) {
        when (MLContains(self, object)) return yes;
    }
    return no;
}


static var MLArrayContainsAll(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    each (object, index, objects) {
        unless (MLContains(self, object)) return no;
    }
    return yes;
}


static var MLArrayIsEmpty(var context, var self, var command, var arguments) {
    return B(that.count == 0);
}


static var MLArrayIsInline(var context, var self, var command, var arguments) {
    return B(that.class == MLInlineArray.pointer);
}


static var MLArrayIsMutable(var context, var self, var command, var arguments) {
    return B(that.class == MLMutableArray.pointer);
}


static var MLArrayAt(var context, var self, var command, var arguments) {
    var index = MLArgument(0);
    MLInteger integerIndex = MLIntegerFrom(index);
    if (integerIndex < 0 || integerIndex >= that.count)
        throw("index-out-of-bounds");
    return that.objects[integerIndex];
}


static var MLArrayAtMany(var context, var self, var command, var arguments) {
    var indexes = MLArgument(0);
    var mutable = MLCreate(MLMutableArray);
    MLInitWithCapacity(mutable, MLCount(indexes));
    each (index, indexOfIndex, indexes) {
        const MLInteger integerIndex = MLIntegerFrom(index);
        if (integerIndex < 0 || integerIndex >= that.count) {
            MLRelease(mutable);
            throw("index-out-of-bounds");
        }
        var object = MLAt(self, index);
        MLAdd(mutable, object);
    }
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayAtCount(var context, var self, var command, var arguments) {
    // TODO: revisit this method to bahave properly when index or count is negative.
    var index = MLArgument(0);
    var count = MLArgument(1);
    var mutable = MLNewWithCapacity(MLMutableArray, count);
    MLInteger integerIndex = MLIntegerFrom(index);
    MLInteger integerCount = MLIntegerFrom(count);
    if (integerIndex < 0 || integerIndex >= that.count) {
        MLRelease(mutable);
        throw("index-out-of-bounds");
    }
    if (integerIndex + integerCount > that.count) integerCount = that.count - integerIndex;
    for (MLInteger i = integerIndex; i < integerIndex + integerCount; i += 1) MLAdd(mutable, that.objects[i]);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayIndexOf(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    for (MLInteger index = 0; index < that.count; index += 1) {
        var equals = MLEquals(that.objects[index], object);
        when (equals) return N(index);
    }
    return N(-1);
}


static var MLArrayLastIndexOf(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    for (MLInteger index = that.count - 1; index >= 0; index -= 1) {
        var equals = MLEquals(that.objects[index], object);
        when (equals) return N(index);
    }
    return N(-1);
}


static var MLArrayIndexesOf(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var indexes = MLNewWithCapacity(MLMutableArray, N(that.count));
    for (MLInteger index = 0; index < that.count; index += 1) {
        var equals = MLEquals(that.objects[index], object);
        when (equals) MLAdd(indexes, N(index));
    }
    return MLMakeAutoreleasedCopyAndReleaseOriginal(indexes);
}


static var MLArrayFirst(var context, var self, var command, var arguments) {
    if (that.count == 0) return null;
    return that.objects[0];
}


static var MLArrayFirstCount(var context, var self, var command, var arguments) {
    var count = MLArgument(0);
    const MLInteger integerCount = MLIntegerFrom(count);
    if (that.count == 0 || integerCount == 0) return A();
    if (that.count <= integerCount) return MLAutorelease(MLCopy(self));
    var index = N(0);
    return MLAtCount(self, index, count);
}


static var MLArrayLast(var context, var self, var command, var arguments) {
    if (that.count == 0) return null;
    return that.objects[that.count - 1];
}


static var MLArrayLastCount(var context, var self, var command, var arguments) {
    var count = MLArgument(0);
    const MLInteger integerCount = MLIntegerFrom(count);
    if (that.count == 0 || integerCount == 0) return A();
    if (that.count <= integerCount) return MLAutorelease(MLCopy(self));
    var index = N(that.count - integerCount);
    return MLAtCount(self, index, count);
}


static var MLArraySecond(var context, var self, var command, var arguments) {
    if (that.count < 2) return null;
    return that.objects[1];
}


static var MLArrayThird(var context, var self, var command, var arguments) {
    if (that.count < 3) return null;
    return that.objects[2];
}


static var MLArrayRest(var context, var self, var command, var arguments) {
    return MLWithoutAt(self, N(0));
}


static var MLArrayWith(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    return MLWithMany(self, IA(object));
}


static var MLArrayWithMany(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var mutable = MLMutableCopy(self);
    var index = N(that.count);
    MLInsertManyAt(mutable, objects, index);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayWithAt(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var index = MLArgument(1);
    return MLWithManyAt(self, IA(object), index);
}


static var MLArrayWithManyAt(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var index = MLArgument(1);
    var mutable = MLMutableCopy(self);
    MLInsertManyAt(mutable, objects, index);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayWithBefore(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var before = MLArgument(1);
    return MLWithManyBefore(self, IA(object), before);
}


static var MLArrayWithManyBefore(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var before = MLArgument(1);
    var index = MLIndexOf(self, before);
    if (MLIntegerFrom(index) < 0) index = N(0);
    return MLWithManyAt(self, objects, index);
}


static var MLArrayWithAfter(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var after = MLArgument(1);
    return MLWithManyAfter(self, IA(object), after);
}


static var MLArrayWithManyAfter(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var after = MLArgument(1);
    var lastIndex = MLLastIndexOf(self, after);
    const MLInteger integerLastIndex = MLIntegerFrom(lastIndex);
    const MLInteger integerInsertionIndex = integerLastIndex >= 0 ? integerLastIndex + 1 : that.count;
    var insertionIndex = N(integerInsertionIndex);
    return MLWithManyAt(self, objects, insertionIndex);
}


static var MLArrayWithReplacing(var context, var self, var command, var arguments) {
    var replacement = MLArgument(0);
    var object = MLArgument(1);
    return MLWithManyReplacing(self, IA(replacement), object);
}


static var MLArrayWithManyReplacing(var context, var self, var command, var arguments) {
    var replacements = MLArgument(0);
    var object = MLArgument(1);
    var mutable = MLMutableCopy(self);
    MLReplaceWithMany(mutable, object, replacements);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayWithReplacingAt(var context, var self, var command, var arguments) {
    var replacement = MLArgument(0);
    var index = MLArgument(1);
    return MLWithManyReplacingAt(self, IA(replacement), index);
}


static var MLArrayWithManyReplacingAt(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var index = MLArgument(1);
    var mutable = MLMutableCopy(self);
    MLReplaceAtWithMany(mutable, index, objects);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayWithReplacingAtCount(var context, var self, var command, var arguments) {
    var replacement = MLArgument(0);
    var index = MLArgument(1);
    var count = MLArgument(2);
    return MLWithManyReplacingAtCount(self, IA(replacement), index, count);
}


static var MLArrayWithManyReplacingAtCount(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var index = MLArgument(1);
    var count = MLArgument(2);
    var mutable = MLMutableCopy(self);
    MLReplaceAtCountWithMany(mutable, index, count, objects);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayWithout(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    return MLWithoutMany(self, IA(object));
}


static var MLArrayWithoutMany(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var mutable = MLMutableCopy(self);
    MLRemoveMany(mutable, objects);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayWithoutAt(var context, var self, var command, var arguments) {
    var index = MLArgument(0);
    return MLWithoutAtCount(self, index, N(1));
}


static var MLArrayWithoutAtMany(var context, var self, var command, var arguments) {
    var indexes = MLArgument(0);
    var mutable = MLMutableCopy(self);
    MLRemoveAtMany(mutable, indexes);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayWithoutAtCount(var context, var self, var command, var arguments) {
    var index = MLArgument(0);
    var count = MLArgument(1);
    var mutable = MLMutableCopy(self);
    MLRemoveAtCount(mutable, index, count);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayReversed(var context, var self, var command, var arguments) {
    var mutable = MLMutableCopy(self);
    MLReverse(mutable);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArraySorted(var context, var self, var command, var arguments) {
    var mutable = MLMutableCopy(self);
    MLSort(mutable);
    return MLMakeAutoreleasedCopyAndReleaseOriginal(mutable);
}


static var MLArrayPluck(var context, var self, var command, var arguments) {
    var key = MLArgument(0);
    var values = MLNewWithCapacity(MLMutableArray, N(that.count));
    each (object, index, self) {
        var value = MLSendArguments(object, key, null);
        MLAdd(values, value);
    }
    return MLMakeAutoreleasedCopyAndReleaseOriginal(values);
}


static var MLArrayMin(var context, var self, var command, var arguments) {
    var min = MLFirst(self);
    each (object, index, self) {
        when (MLIsLessThan(object, min)) min = object;
    }
    return min;
}


static var MLArrayMax(var context, var self, var command, var arguments) {
    var max = MLFirst(self);
    each (object, index, self) {
        when (MLIsGreaterThan(object, max)) max = object;
    }
    return max;
}


static var MLArrayIsArray(var context, var self, var command, var arguments) {
    return yes;
}


static var MLArrayDescription(var context, var self, var command, var arguments) {
    MLWarning("TODO: implement method -description for arrays");
    return null;
}


static var MLArrayEquals(var context, var self, var command, var arguments) {
    var array = MLArgument(0);
    unless (MLIsArray(array)) return no;

    const MLInteger count = MLArrayStructure(array).count;
    if (count != that.count) return no;

    for (MLInteger index = 0; index < count; index += 1) {
        var object1 = that.objects[index];
        var object2 = MLArrayStructure(array).objects[index];
        unless (MLEquals(object1, object2)) return no;
    }

    return yes;
}


static var MLArrayCopy(var context, var self, var command, var arguments) {
    return MLRetain(self);
}


static var MLArrayMutableCopy(var context, var self, var command, var arguments) {
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

    "with*replacing-at*", MLArrayWithReplacingAt,
    "with-many*replacing-at*", MLArrayWithManyReplacingAt,

    "with*replacing-at*count*", MLArrayWithReplacingAtCount,
    "with-many*replacing-at*count*", MLArrayWithManyReplacingAtCount,

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


static var MLInlineArrayMetaCreate(var context, var self, var command, var arguments) {
    MLError("Can't create inline array, inline arrays can only be inlined");
    return null;
}


MLPointer MLInlineArrayMetaDefaultMethods[] = {
    "create", MLInlineArrayMetaCreate,
    NULL
};


static var MLInlineArrayCopy(var context, var self, var command, var arguments) {
    return MLNewWithArray(MLArray, self);
}


static var MLInlineArrayMutableCopy(var context, var self, var command, var arguments) {
    return MLNewWithArray(MLMutableArray, self);
}


static var MLInlineArrayRetain(var context, var self, var command, var arguments) {
    MLError("Can't retain an inline array, you have to copy it");
    return null;
}


static var MLInlineArrayRetainCount(var context, var self, var command, var arguments) {
    return N(-1);
}


static var MLInlineArrayRelease(var context, var self, var command, var arguments) {
    MLError("Can't release an inline array because it can't be retained in the first place");
    return null;
}


static var MLInlineArrayAutorelease(var context, var self, var command, var arguments) {
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

    NULL,
};


static var MLMutableArrayMetaCreate(var context, var self, var command, var arguments) {
    return MLArrayMake(MLAllocate(MLArraySize), MLMutableArray, 1, 0, 0, NULL);
}


static var MLMutableArrayMetaNewWithCapacity(var context, var self, var command, var arguments) {
    var capacity = MLArgument(0);
    var new = MLCreate(self);
    return MLInitWithCapacity(new, capacity);
}


static var MLMutableArrayMetaWithCapacity(var context, var self, var command, var arguments) {
    var capacity = MLArgument(0);
    var new = MLCreate(self);
    MLInitWithCapacity(new, capacity);
    return MLAutorelease(new);
}


MLPointer MLMutableArrayMetaDefaultMethods[] = {
    "create", MLMutableArrayMetaCreate,
    "new-with-capacity*", MLMutableArrayMetaNewWithCapacity,
    "with-capacity*", MLMutableArrayMetaWithCapacity,
    NULL
};


static var MLMutableArrayInitWithCapacity(var context, var self, var command, var arguments) {
    var capacity = MLArgument(0);
    self = MLSuper("init");
    when (self) {
        MLIncreaseCapacity(self, capacity);
    }
    return self;
}


static var MLMutableArrayCapacity(var context, var self, var command, var arguments) {
    return N(that.capacity);
}


static var MLMutableArrayIncreaseCapacity(var context, var self, var command, var arguments) {
    var capacity = MLArgument(0);

    MLInteger integerCapacity = MLIntegerFrom(capacity);
    if (integerCapacity < that.capacity) return self;
    if (integerCapacity < MLMutableArrayMinCapacity) integerCapacity = MLMutableArrayMinCapacity;

    integerCapacity = MLHelperRoundUpToPowerOfTwo(integerCapacity);

    that.capacity = integerCapacity;
    that.objects = MLResize(that.objects, MLVariableSize * integerCapacity);

    return self;
}


static var MLMutableArrayAdd(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    if (that.capacity <= that.count) MLIncreaseCapacity(self, N(that.count + 1));
    that.objects[that.count] = MLRetain(object);
    that.count += 1;
    return self;
}


static var MLMutableArrayAddMany(var context, var self, var command, var arguments) {
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


static var MLMutableArrayInsert(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    return MLInsertManyAt(self, IA(object), N(0));
}


static var MLMutableArrayInsertMany(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    return MLInsertManyAt(self, objects, N(0));
}


static var MLMutableArrayInsertAt(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var index = MLArgument(1);
    return MLInsertManyAt(self, IA(object), index);
}


static var MLMutableArrayInsertManyAt(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var index = MLArgument(1);

    const MLInteger integerIndex = MLIntegerFrom(index);
    if (integerIndex < 0 || integerIndex > that.count) throw("index-out-of-bounds");

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


static var MLMutableArrayInsertBefore(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var before = MLArgument(1);
    return MLInsertManyBefore(self, IA(object), before);
}


static var MLMutableArrayInsertManyBefore(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var before = MLArgument(1);
    var index = MLIndexOf(self, before);
    if (MLIntegerFrom(index) < 0) index = N(0);
    return MLInsertManyAt(self, objects, index);
}


static var MLMutableArrayInsertAfter(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var after = MLArgument(1);
    return MLInsertManyAfter(self, IA(object), after);
}


static var MLMutableArrayInsertManyAfter(var context, var self, var command, var arguments) {
    var objects = MLArgument(0);
    var after = MLArgument(1);
    const var index = MLLastIndexOf(self, after);
    const MLInteger integerIndex = MLIntegerFrom(index);
    const MLInteger insertionIndex = integerIndex >= 0 ? integerIndex + 1 : that.count;
    return MLInsertManyAt(self, objects, N(insertionIndex));
}


static var MLMutableArrayReplaceWith(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var replacement = MLArgument(1);
    return MLReplaceWithMany(self, object, IA(replacement));
}


static var MLMutableArrayReplaceWithMany(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var replacements = MLArgument(1);
    var mutable = MLNewWithCapacity(MLMutableArray, MLCount(self));

    each (currentObject, indexOfCurrentObject, self) {
        var shouldReplaceCurrent = MLEquals(currentObject, object);
        when (shouldReplaceCurrent) {
            MLAddMany(mutable, replacements);
        } else {
            MLAdd(mutable, currentObject);
        }
    }

    MLRemoveAll(self);
    MLAddMany(self, mutable);
    MLRelease(mutable);

    return self;
}


static var MLMutableArrayReplaceAtWith(var context, var self, var command, var arguments) {
    var index = MLArgument(0);
    var replacement = MLArgument(1);
    return MLReplaceAtWithMany(self, index, IA(replacement));
}


static var MLMutableArrayReplaceAtWithMany(var context, var self, var command, var arguments) {
    var index = MLArgument(0);
    var replacements = MLArgument(1);
    MLRemoveAt(self, index);
    MLInsertManyAt(self, replacements, index);
    return self;
}


static var MLMutableArrayReplaceAtCountWith(var context, var self, var command, var arguments) {
    var index = MLArgument(0);
    var count = MLArgument(1);
    var replacement = MLArgument(2);
    return MLReplaceAtCountWithMany(self, index, count, IA(replacement));
}


static var MLMutableArrayReplaceAtCountWithMany(var context, var self, var command, var arguments) {
    var index = MLArgument(0);
    var count = MLArgument(1);
    var replacements = MLArgument(2);
    MLRemoveAtCount(self, index, count);
    MLInsertManyAt(self, replacements, index);
    return self;
}


static var MLMutableArrayRemove(var context, var self, var command, var arguments) {
    var object = MLArgument(0);
    var indexes = MLIndexesOf(self, object);
    return MLRemoveAtMany(self, indexes);
}


static var MLMutableArrayRemoveMany(var context, var self, var command, var arguments) {
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


static var MLMutableArrayRemoveAt(var context, var self, var command, var arguments) {
    var index = MLArgument(0);
    return MLRemoveAtMany(self, IA(index));
}


static var MLMutableArrayRemoveAtMany(var context, var self, var command, var arguments) {
    var indexes = MLArgument(0);
    MLInteger count = 0;

    each (i, j, indexes) {
        MLInteger index = MLIntegerFrom(i);
        if (index < 0 || index >= that.count) throw("index-out-of-bounds");
        var object = that.objects[index];
        if (object.pointer != MLMarking) MLAutorelease(object);
        that.objects[index].pointer = MLMarking;
    }

    for (MLInteger index = 0; index < that.count; index += 1) {
        if (that.objects[index].pointer == MLMarking) continue;
        that.objects[count] = that.objects[index];
        count += 1;
    }

    for (MLInteger index = count; index < that.count; index += 1) {
        that.objects[index] = null;
    }

    that.count = count;
    return self;
}


static var MLMutableArrayRemoveAtCount(var context, var self, var command, var arguments) {
    var index = MLArgument(0);
    var count = MLArgument(1);

    MLInteger integerIndex = MLIntegerFrom(index);
    MLInteger integerCount = MLIntegerFrom(count);

    MLAssert(integerCount >= 0, "Can't remove objects at index, count is < 0");

    if (integerIndex < 0) {
        integerCount += integerIndex;
        integerIndex = 0;
    }

    if (integerCount <= 0) {
        return self;
    }

    if (integerIndex + integerCount > that.count) {
        integerCount = that.count - integerIndex;
    }

    for (MLInteger i = integerIndex; i < integerIndex + integerCount; i += 1) {
        MLAutorelease(that.objects[i]);
    }

    for (MLInteger j = integerIndex + integerCount; j < that.count; j += 1) {
        const MLInteger i = j - integerCount;
        that.objects[j - integerCount] = that.objects[j];
        that.objects[j] = null;
    }

    that.count -= integerCount;
    return self;
}


static var MLMutableArrayRemoveAll(var context, var self, var command, var arguments) {
    for (MLInteger index = 0; index < that.count; index += 1) {
        MLRelease(that.objects[index]);
        that.objects[index] = null;
    }
    that.count = 0;
    return self;
}


static var MLMutableArrayReverse(var context, var self, var command, var arguments) {
    const MLInteger center = that.count / 2;
    for (MLInteger left = 0; left < center; left += 1) {
        const MLInteger right = that.count - left - 1;
        const var leftObject = that.objects[left];
        const var rightObject = that.objects[right];
        that.objects[left] = rightObject;
        that.objects[right] = leftObject;
    }
    return self;
}


static var MLMutableArraySort(var context, var self, var command, var arguments) {
    qsort(that.objects, that.count, sizeof(var), MLCompare);
    return self;
}


static var MLMutableArrayIsMutable(var context, var self, var command, var arguments) {
    return yes;
}


static var MLMutableArrayCopy(var context, var self, var command, var arguments) {
    var copy = MLCreate(MLArray);
    return MLInitWithArray(copy, self);
}


static var MLMutableArrayMutableCopy(var context, var self, var command, var arguments) {
    var mutableCopy = MLCreate(MLMutableArray);
    return MLInitWithArray(mutableCopy, self);
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
    "replace*with-many*", MLMutableArrayReplaceWithMany,

    "replace-at*with*", MLMutableArrayReplaceAtWith,
    "replace-at*with-many*", MLMutableArrayReplaceAtWithMany,

    "replace-at*count*with*", MLMutableArrayReplaceAtCountWith,
    "replace-at*count*with-many*", MLMutableArrayReplaceAtCountWithMany,

    "remove*", MLMutableArrayRemove,
    "remove-many*", MLMutableArrayRemoveMany,

    "remove-at*", MLMutableArrayRemoveAt,
    "remove-at-many*", MLMutableArrayRemoveAtMany,
    "remove-at*count*", MLMutableArrayRemoveAtCount,
    "remove-all", MLMutableArrayRemoveAll,

    "reverse", MLMutableArrayReverse,
    "sort", MLMutableArraySort,

    "is-mutable?", MLMutableArrayIsMutable,

    "copy", MLMutableArrayCopy,
    "mutable-copy", MLMutableArrayMutableCopy,

    NULL
};
