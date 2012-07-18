//
// Copyright (c) 2010 Konstantin Bender.
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

#ifndef CR_TEST_H
#define CR_TEST_H

#include <assert.h>
#include <stdio.h>
#include <metal/metal.h>

// ----------------------------------------------------------- Assertions ------

#define CRAssertEquals(object1, object2, ...) CRAssert(CREquals(object1, object2), __VA_ARGS__)

// ------------------------------------------------------ General Matcher ------

#define CRExpectObjectToBeIdenticalTo(object1, object2) CRAssert(object1.pointer == object2.pointer, "%s should be identical to %s", CRStringCharacters(CRDescription(object1)), CRStringCharacters(CRDescription(object2)))
#define CRExpectObjectNotToBeIdenticalTo(object1, object2) CRAssert(object1.pointer != object2.pointer, "%s should not be identical to %s", CRStringCharacters(CRDescription(object1)), CRStringCharacters(CRDescription(object2)))

#define CRExpectObjectToBeEqualTo(object1, object2) CRAssert(CREquals(object1, object2), "%s should be equal to %s", CRStringCharacters(CRDescription(object1)), CRStringCharacters(CRDescription(object2)))
#define CRExpectObjectNotToBeEqualTo(object1, object2) CRAssert(!CREquals(object1, object2), "%s should not be equal to %s", CRStringCharacters(CRDescription(object1)), CRStringCharacters(CRDescription(object2)))

// -------------------------------------------------------- Array Matcher ------

#define CRExpectArrayToHaveCount(array, count) CRAssert(CRArrayCount(array) == count, "%s should have count %lu", CRStringCharacters(CRDescription(array)), CRArrayCount(array))
#define CRExpectArrayNotToHaveCount(array, count) CRAssert(CRArrayCount(array) != count, "%s should not have count %lu", CRStringCharacters(CRDescription(array)), CRArrayCount(array))

#define CRExpectArrayToContain(array, object) CRAssert(CRArrayContains(array, object), "%s should contain %s", CRStringCharacters(CRDescription(array)), CRStringCharacters(CRDescription(object)))
#define CRExpectArrayNotToContain(array, object) CRAssert(CRArrayContains(array, object), "%s should not contain %s", CRStringCharacters(CRDescription(array)), CRStringCharacters(CRDescription(object)))

// ---------------------------------------------------------------- Tests ------

void CRTestCore();
void CRTestDigest();
void CRTestMath();

void CRTestObject();
void CRTestClass();

void CRTestBoolean();
void CRTestNumber();
void CRTestWord();
void CRTestMethod();
void CRTestDate();

void CRTestString();
void CRTestArray();
void CRTestDictionary();

void CRTestMutableString();
void CRTestMutableArray();
void CRTestMutableDictionary();

void CRTestAutoreleasePool();
void CRTestRuntime();

#endif
