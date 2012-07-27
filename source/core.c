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

#include "core.h"
#include "metal.h"


var CRReference(CRPointer pointer) {
    var reference = null;
    reference.pointer = pointer;
    reference.payload.natural = 0;
    return reference;
}


// -----------------------------------------------------------------------------


var CRRetain(var object) {
    struct CRObject* instance = object.pointer;
    if (instance == NULL) return object;

    const CRNatural retain_count = instance->retain_count;
    if (retain_count >= CRRetainCountMax) return object;

    instance->retain_count += 1;
    return object;
}


CRNatural CRRetainCount(var object) {
    struct CRObject* instance = object.pointer;
    if (instance == NULL) return 0;
    return instance->retain_count;
}


var CRRelease(var object) {
    struct CRObject* instance = object.pointer;
    if (instance == NULL) return null;

    const CRNatural retain_count = instance->retain_count;
    if (retain_count >= CRRetainCountMax) return object;
    if (retain_count == 0) CRError("Can't release an object with retain count 0, retain count is inconsistent");

    instance->retain_count -= 1;
    if (instance->retain_count > 0) return object;

    CRDestroy(object);
    return null;
}


var CRAutorelease(var object) {
    // TODO: implement.
    return object;
}


CRNatural64 CRHash(var object) {
    const struct CRObject* instance = object.pointer;
    if (instance == NULL) return 0;

    const CRNatural payload = object.payload.natural;
    if (payload != 0) return payload;

    const struct CRClass* class = instance->class;
    if (class == NULL) CRError("Can't compute hash for object because the class of object is null");

    const struct CRCallbacks* callbacks = class->callbacks;
    if (!callbacks) CRError("Can't compute hash for object because callbacks are not defined for its class");

    const CRHashCallback hash = class->callbacks->hash;
    if (!hash) CRError("Can't compute hash for object because CRHashCallback is not set for its class");

    return hash(object);
}


bool CREquals(var object1, var object2) {
    const struct CRObject* instance1 = object1.pointer;
    const struct CRObject* instance2 = object2.pointer;

    if (instance1 == NULL && instance2 == NULL) return true;
    if (instance1 == NULL) return false;
    if (instance2 == NULL) return false;

    const struct CRClass* class1 = instance1->class;
    const struct CRClass* class2 = instance2->class;

    if (class1 == NULL) CRError("Can't compare objects because the class of object1 is null");
    if (class2 == NULL) CRError("Can't compare objects because the class of object2 is null");

    const struct CRCallbacks* callbacks = class1->callbacks;
    if (!callbacks) CRError("Can't compare objects because callbacks for class are not defined");

    const CREqualsCallback equals = class1->callbacks->equals;
    if (!equals) CRError("Can't compare objects because CREqualsCallback for class is not set");

    return equals(object1, object2);
}


var CRCopy(var object) {
    const struct CRObject* instance = object.pointer;
    if (instance == NULL) return object;

    const struct CRClass* class = instance->class;
    if (class == NULL) CRError("Can't copy object because the class of object is null");

    const struct CRCallbacks* callbacks = class->callbacks;
    if (!callbacks) CRError("Can't copy object because callbacks are not defined for its class");

    const CRCopyCallback copy = class->callbacks->copy;
    if (!copy) CRError("Can't copy object because CRCopyCallback is not set for its class");

    return copy(object);
}


var CRMutableCopy(var object) {
    const struct CRObject* instance = object.pointer;
    if (instance == NULL) return object;

    const struct CRClass* class = instance->class;
    if (class == NULL) CRError("Can't mutable copy object because the class of object is null");

    const struct CRCallbacks* callbacks = class->callbacks;
    if (!callbacks) CRError("Can't mutable copy object because callbacks are not defined for its class");

    const CRMutableCopyCallback mutable_copy = class->callbacks->mutable_copy;
    if (!mutable_copy) CRError("Can't mutable copy object because CRMutableCopyCallback is not set for its class");

    return mutable_copy(object);
}


void CRDestroy(var object) {
    const struct CRObject* instance = object.pointer;
    if (instance == NULL) return;

    const CRNatural retain_count = instance->retain_count;
    if (retain_count > 0) CRWarning("Destroying object with ratain count > 0");

    const struct CRClass* class = instance->class;
    if (class == NULL) CRError("Can't destroy object because the class of object is null");

    const struct CRCallbacks* callbacks = class->callbacks;
    if (!callbacks) CRError("Can't destroy object because callbacks are not defined for its class");

    const CRDestroyCallback destroy = class->callbacks->destroy;
    if (!destroy) CRError("Can't destroy object because CRDestroyCallback is not set for its class");

    destroy(object);
}


var CRDescription(var object) {
    const struct CRObject* instance = object.pointer;
    if (instance == NULL) return CRStringCreateWithCharacters("null");

    const struct CRClass* class = instance->class;
    if (class == NULL) CRError("Can't return description for object because the class of object is null");

    const struct CRCallbacks* callbacks = class->callbacks;
    if (!callbacks) CRError("Can't return description for object because callbacks are not defined for its class");

    const CRDescriptionCallback description = class->callbacks->description;
    if (!description) CRError("Can't return description for object because CRDescriptionCallback is not set for its class");

    return description(object);
}


// -----------------------------------------------------------------------------


bool CRIsNull(var object) {
    return object.pointer == NULL;
}


bool CRIsInstanceOf(var object, var class) {
    if (object.pointer == NULL) return false;
    struct CRObject* object_to_be_checked = object.pointer;
    struct CRClass* class_of_object = object_to_be_checked->class;
    return class_of_object == class.pointer;
}
