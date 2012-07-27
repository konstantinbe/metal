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

#include "test.h"


void CRTestPoolCreate() {
    var autorelease_pool = CRPoolCreate();
    CRAssert(CRRetainCount(autorelease_pool) == 1, "Retain count of a newly created autorelease pool should be 1");
    CRAssert(CREquals(CRPoolObjects(autorelease_pool), CRArray()), "Objects of a newly created pool should be an empty array");
    CRRelease(autorelease_pool);
}


void CRTestPoolAdd() {
    var autorelease_pool = CRPoolCreate();
    var string1 = CRStringCreate();
    var string2 = CRStringCreate();

    CRRetain(string1);
    CRRetain(string2);

    CRPoolAdd(autorelease_pool, string1);
    CRPoolAdd(autorelease_pool, string2);

    CRAssert(CREquals(CRPoolObjects(autorelease_pool), CRArray(string1, string2)), "Adding objects to the autorelease pool should add to the objects array");
    CRRelease(autorelease_pool);

    CRRelease(string1);
    CRRelease(string2);
}


void CRTestPoolDrain() {
    var autorelease_pool = CRPoolCreate();
    var string1 = CRStringCreate();
    var string2 = CRStringCreate();

    CRRetain(string1);
    CRRetain(string2);

    CRPoolAdd(autorelease_pool, string1);
    CRPoolAdd(autorelease_pool, string2);

    const CRNatural retain_count1 = CRRetainCount(string1);
    const CRNatural retain_count2 = CRRetainCount(string2);

    CRPoolDrain(autorelease_pool);
    CRAssert(CRRetainCount(string1) == retain_count1 - 1, "Draining a releasepool should release its objects");
    CRAssert(CRRetainCount(string2) == retain_count2 - 1, "Draining a releasepool should release its objects");
    CRAssert(CREquals(CRPoolObjects(autorelease_pool), CRArray()), "Draining a releasepool should remove all objects");

    CRRelease(autorelease_pool);
    CRRelease(string1);
    CRRelease(string2);
}


// -----------------------------------------------------------------------------


void CRTestPoolDestroy() {
    var autorelease_pool = CRPoolCreate();
    var string1 = CRStringCreate();
    var string2 = CRStringCreate();

    CRRetain(string1);
    CRRetain(string2);

    CRPoolAdd(autorelease_pool, string1);
    CRPoolAdd(autorelease_pool, string2);

    const CRNatural retain_count1 = CRRetainCount(string1);
    const CRNatural retain_count2 = CRRetainCount(string2);

    CRRelease(autorelease_pool);
    CRAssert(CRRetainCount(string1) == retain_count1 - 1, "Releasing an autorelease pool should drain it before destroying");
    CRAssert(CRRetainCount(string2) == retain_count1 - 1, "Releasing an autorelease pool should drain it before destroying");

    CRRelease(string1);
    CRRelease(string2);
}


void CRTestPoolDescription() {
    var autorelease_pool = CRPoolCreate();
    var string1 = CRStringCreateWithCharacters("String 1");
    var string2 = CRStringCreateWithCharacters("String 2");

    CRPoolAdd(autorelease_pool, string1);
    CRPoolAdd(autorelease_pool, string2);

    var description = CRDescription(autorelease_pool);
    var description_of_contained_objects = CRDescription(CRPoolObjects(autorelease_pool));

    CRAssertEquals(description, description_of_contained_objects, "Description of an autorelease pool should equal the description of the contained objects array");
    CRRelease(autorelease_pool);
}


// -----------------------------------------------------------------------------


void CRTestPool() {
    CRTestPoolCreate();
    CRTestPoolAdd();
    CRTestPoolDrain();

    CRTestPoolDestroy();
    CRTestPoolDescription();
}
