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

#include "mutable-array.h"
#include "mutable-array-private.h"

#include "array.h"
#include "array-private.h"

#include "class.h"
#include "class-private.h"

#include "object.h"
#include "object-private.h"

#include "macros.h"
#include "math.h"


// --------------------------------------------------------------- Macros ------


#define this ((struct CRArray*)self.pointer)
#define that (*this)

#define CRMutableArrayThrowErrorIfNull() if (this == NULL) CRError("self is null")
#define CRMutableArrayThrowErrorIfNotMutableArray() if (that.class != CRMutableArray.pointer) CRError("self is not a mutable array")


// -------------------------------------------------- Constants & Globals ------


const var CRMutableArray = {&CRMutableArrayClass};


// ---------------------------------------------- Creating Mutable Arrays ------


var CRArrayCreateMutable() {
    var self = CRArrayCreate();
    that.class = &CRMutableArrayClass;
    return self;
}


var CRArrayCreateMutableWithCapacity(CRNatural capacity) {
    var self = CRReference(CRAllocate(sizeof(struct CRArray)));

    if (capacity < CRMutableArrayMinCapacity) capacity = CRMutableArrayMinCapacity;
    CRRoundUpToPowerOfTwo(capacity);

    that.class = &CRMutableArrayClass;
    that.retain_count = 1;
    that.capacity = capacity;
    that.count = 0;
    that.objects = CRAllocate(sizeof(var) * capacity);

    return self;
}


var CRArrayCreateMutableWithCArray(CRNatural count, var* objects) {
    var self = CRArrayCreateWithCArray(count, objects);
    that.class = &CRMutableArrayClass;
    return self;
}


// ------------------------------------------------------- Adding Objects ------


void CRArrayAdd(var self, var object) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    const CRNatural count = CRArrayCount(self);
    CRArrayIncreaseCapacityToAtLeast(self, count + 1);

    that.objects[count] = CRRetain(object);
    that.count = count + 1;
}


void CRArrayAddMany(var self, var objects) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    const CRNatural count = CRArrayCount(self);
    const CRNatural number_of_objects = CRArrayCount(objects);
    CRArrayIncreaseCapacityToAtLeast(self, count + number_of_objects);

    for (CRInteger i = 0; i < number_of_objects; i += 1) {
        const var object = CRArrayObjectAt(objects, i);
        that.objects[count + i] = CRRetain(object);
    }

    that.count = count + number_of_objects;
}


void CRArrayInsertAt(var self, var object, CRInteger index) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    if (index < 0 || index > that.count) CRError("Can't insert object at %ld, index out of bounds", index);

    CRArrayIncreaseCapacityToAtLeast(self, that.count + 1);
    CRArrayExpandAt(self, 1, index);

    that.objects[index] = CRRetain(object);
    that.count += 1;
}


void CRArrayInsertManyAt(var self, var objects, CRInteger index) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    if (index < 0 || index > that.count) CRError("Can't insert many objects at %ld, index out of bounds", index);

    const CRInteger number_of_objects = CRArrayCount(objects);
    CRArrayIncreaseCapacityToAtLeast(self, that.count + number_of_objects);
    CRArrayExpandAt(self, number_of_objects, index);

    for (CRInteger i = 0; i < number_of_objects; i += 1) {
        const var object = CRArrayObjectAt(objects, i);
        that.objects[index + i] = CRRetain(object);
    }

    that.count += number_of_objects;
}


// ----------------------------------------------------- Removing Objects ------


void CRArrayRemove(var self, var object) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();
    CRArrayRemoveMany(self, CRArray(object));
}


void CRArrayRemoveMany(var self, var objects) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    for (CRInteger index = 0; index < that.count; index += 1) {
        var objectAtIndex = CRArrayObjectAt(self, index);
        if (CRArrayContains(objects, objectAtIndex)) {
            CRRelease(objectAtIndex);
            CRArrayMarkSlotForRemovalAt(self, index);
        }
    }

    CRArrayClearSlotsMarkedForRemoval(self);
}


void CRArrayRemoveAt(var self, CRInteger index) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    if (index < 0 || index >= that.count) CRError("Can't remove object at %ld, index out of bounds", index);

    CRRelease(that.objects[index]);
    CRArrayContractAt(self, 1, index);
    that.count -= 1;
}


void CRArrayRemoveManyAt(var self, var indexes) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    const CRInteger count = CRArrayCount(self);
    const CRInteger number_of_indexes = CRArrayCount(indexes);

    // mark slots for removal, release objects
    for (CRInteger i = 0; i < number_of_indexes; i += 1) {
        CRInteger index = (CRInteger)CRArrayObjectAt(indexes, i).payload.decimal;
        if (index < 0 || index >= count) CRError("Can't remove object at %ld, index out of bounds", index);

        CRRelease(that.objects[index]);
        CRArrayMarkSlotForRemovalAt(self, index);
    }

    // clear marked slots
    CRArrayClearSlotsMarkedForRemoval(self);
}


void CRArrayClear(var self) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    const CRNatural count = CRArrayCount(self);
    for (CRInteger i = 0; i < count; i += 1) {
        CRRelease(that.objects[i]);
        that.objects[i] = null;
    }

    that.count = 0;
}


// ---------------------------------------------------------------- Other ------


void CRArrayIncreaseCapacityToAtLeast(var self, CRNatural capacity) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    if (capacity <= CRArrayCapacity(self)) return;
    if (capacity < CRMutableArrayMinCapacity) capacity = CRMutableArrayMinCapacity;

    CRRoundUpToPowerOfTwo(capacity);

    that.capacity = capacity;
    that.objects = CRResize(CRArrayObjects(self), sizeof(var) * capacity);
}


// -------------------------------------------------------------- Private ------


struct CRClass CRMutableArrayClass = {.class = &CRMutableArrayMetaClass, .callbacks = &CRMutableArrayCallbacks};
struct CRClass CRMutableArrayMetaClass = {.class = &CRMutableArrayMetaClass, .callbacks = &CRMutableArrayMetaCallbacks};


struct CRCallbacks CRMutableArrayCallbacks = {
    &CRArrayHash,
    &CRArrayEquals,
    &CRArrayCopy,
    &CRArrayMutableCopy,
    &CRArrayDestroy,
    &CRArrayDescription
};


struct CRCallbacks CRMutableArrayMetaCallbacks = {
    &CRClassHash,
    &CRClassEquals,
    &CRClassCopy,
    &CRClassMutableCopy,
    &CRClassDestroy,
    &CRClassDescription
};


// -----------------------------------------------------------------------------


static void* CRArrayRemovalMarking = &CRArrayRemovalMarking;


void CRArrayExpandAt(var self, CRInteger count, CRInteger index) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    for (CRInteger i = CRArrayCount(self) - 1; i >= index; i -= 1) {
        that.objects[i + count] = that.objects[i];
    }
}


void CRArrayContractAt(var self, CRInteger count, CRInteger index) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    for (CRInteger i = index + count; i < CRArrayCount(self); i += 1) {
        that.objects[i - count] = that.objects[i];
    }
}


void CRArrayMarkSlotForRemovalAt(var self, CRInteger index) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    that.objects[index].pointer = CRArrayRemovalMarking;
    that.objects[index].payload.natural = 0;
}


bool CRArrayIsSlotMarkedForRemovalAt(var self, CRInteger index) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();
    return that.objects[index].pointer == CRArrayRemovalMarking;
}


void CRArrayClearSlotsMarkedForRemoval(var self) {
    CRMutableArrayThrowErrorIfNull();
    CRMutableArrayThrowErrorIfNotMutableArray();

    const CRInteger count = that.count;
    CRInteger number_of_kept_objects = 0;

    // rearrange array by leaving out objects marked for removal
    for (CRInteger index = 0; index < count; index += 1) {
        if (CRArrayIsSlotMarkedForRemovalAt(self, index)) continue;
        that.objects[number_of_kept_objects] = that.objects[index];
        number_of_kept_objects += 1;
    }

    // undefinedify empty slots
    for (CRInteger index = number_of_kept_objects; index < count; index += 1) {
        that.objects[index] = null;
    }

    that.count = number_of_kept_objects;
}
