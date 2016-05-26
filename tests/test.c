//
// Copyright (c) 2014 Konstantin Bender.
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
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// --------------------------------------------------- Constants & Macros ------

#define AssertRaises(message) for (void* performHandleBlock = MLPerformHandleBlockPush(); performHandleBlock != MLZero; ({ AssertNotNull(MLPerformHandleBlockHandle(performHandleBlock), message); true; }) && (performHandleBlock = MLPerformHandleBlockPop(performHandleBlock))) if (!setjmp(MLPerformHandleBlockPerform(performHandleBlock)))
#define AssertNotRaises(message) for (void* performHandleBlock = MLPerformHandleBlockPush(); performHandleBlock != MLZero; ({ AssertNull(MLPerformHandleBlockHandle(performHandleBlock), message); true; }) && (performHandleBlock = MLPerformHandleBlockPop(performHandleBlock))) if (!setjmp(MLPerformHandleBlockPerform(performHandleBlock)))

static const char* const WHITE = "\x1B[0;97m";
static const char* const RED = "\x1B[0;31m";
static const char* const GREEN = "\x1B[0;32m";
static const char* const RESET = "\x1B[0m";

// ------------------------------------------------------------ Variables ------

static long TotalNumberOfExamples = 0;
static long NumberOfFailedExamples = 0;
static char* FailedExamples[1000000];
static double TestBeganAt = 0;
static double TestEndedAt = 0;

// ---------------------------------------------------- Helper Functions -------

static void TestBegin();
static void TestEnd();

// --------------------------------------------------- Matcher Functions -------

static void AssertYes(MLVariable object, const char* message);
static void AssertNo(MLVariable object, const char* message);
static void AssertNull(MLVariable object, const char* message);
static void AssertNotNull(MLVariable object, const char* message);
static void AssertEquals(MLVariable subject, MLVariable actual, const char* message);
static void AssertNotEqual(MLVariable subject, MLVariable actual, const char* message);
static void AssertIdentical(MLVariable subject, MLVariable actual, const char* message);
static void AssertNotIdentical(MLVariable subject, MLVariable actual, const char* message);

// --------------------------------------------------------- Object Tests ------

static void TestObjectCreate() {
    MLVariable object = MLSend(MLObject, "create");
    AssertNotNull(object, "Object create calls allocate and returns the newly allocated object");
}

static void TestObjectDestroy() {
    // Destroy can't be properly tested because created objects are autoreleased.
}

static void TestObjectIsKindOf() {
    MLVariable object = MLSend(MLObject, "create");
    MLVariable number = MLNumber(5);
    AssertYes(MLSend(MLObject, "is-kind-of*", MLObject), "Object is-kind-of* returns yes when asking Object whether it is a kind of itself");
    AssertYes(MLSend(object, "is-kind-of*", object), "Object is-kind-of* returns yes when asking an object whether it is a kind of itself");
    AssertYes(MLSend(MLNumber, "is-kind-of*", MLObject), "Object is-kind-of* returns yes when asking Number whether it is a kind of Object");
    AssertYes(MLSend(object, "is-kind-of*", MLObject), "Object is-kind-of* returns yes when asking an object whether it is a kind of Object");
    AssertYes(MLSend(number, "is-kind-of*", MLNumber), "Object is-kind-of* returns yes when asking a number whether it is a kind of Number");
    AssertYes(MLSend(number, "is-kind-of*", MLObject), "Object is-kind-of* returns yes when asking a number whether it is a kind of Object");
    AssertNo(MLSend(MLObject, "is-kind-of*", object), "Object is-kind-of* returns mo when asking Object whether it is a kind of an object");
    AssertNo(MLSend(object, "is-kind-of*", MLNumber), "Object is-kind-of* returns no when asking an object whether it is a kind of Number");
    AssertNo(MLSend(number, "is-kind-of*", object), "Object is-kind-of* returns no when asking a number whether it is a kind of object");
}

static void TestObjectIsMutable() {
    MLVariable object1 = MLSend(MLObject, "create");
    MLVariable object2 = MLSend(MLObject, "create", MLString("mutable"), MLNo);
    MLVariable object3 = MLSend(MLObject, "create", MLString("mutable"), MLYes);
    AssertNo(MLSend(object1, "is-mutable"), "Object is-mutable returns MLNo (all objects are not mutable by default)");
    AssertNo(MLSend(object2, "is-mutable"), "Object is-mutable returns MLNo if option mutable = MLNo was passed to create");
    AssertYes(MLSend(object3, "is-mutable"), "Object is-mutable returns MLYes if option mutable = MLYes was passed to create");
}

static void TestObjectRespondsTo() {
    AssertYes(MLSend(MLObject, "responds-to*", MLString("equals*")), "Object responds-to* returns MLYes for an existing method");
    // TODO: enable this test after fully implementing the MLLookup() function.
    // AssertNo(MLSend(MLObject, "responds-to*", MLString("winni-puh")), "Object responds-to* returns MLNo for a non-existing method");
}

static void TestObjectAsString() {
    MLVariable object = MLSend(MLObject, "create");
    AssertEquals(MLSend(MLObject, "as-string"), MLString("Object"), "Object as-string returns 'Object' for Object");
    AssertEquals(MLSend(object, "as-string"), MLString("<Object XXX>"), "Object as-string returns '<Object XXX>' for any direct instance of Object");
}

static void TestObjectHash() {
    MLVariable object = MLSend(MLObject, "create");
    AssertEquals(MLSend(object, "hash"), MLNumber((MLNatural)object), "Object hash returns its address as a number");
}

static void TestObjectEquals() {
    MLVariable object1 = MLSend(MLObject, "create");
    MLVariable object2 = MLSend(MLObject, "create");
    AssertYes(MLSend(object1, "equals*", object1), "Object equals* returns MLYes for identical objects");
    AssertNo(MLSend(object1, "equals*", object2), "Object equals* returns MLNo when objects are not identical");
}

static void TestObjectSelf() {
    MLVariable object = MLSend(MLObject, "create");
    AssertIdentical(object, MLSend(object, "self"), "Object self returns itself");
}

static void TestObjectAddMethodBlock() {
    // TODO: implement.
}

static void TestObjectRemoveMethod() {
    // TODO: implement.
}

static void TestObjectProto() {
    // TODO: implement.
}

static void TestObjectSetProto() {
    // TODO: implement.
}

static void TestObjectWarn() {
    // TODO: implement.
}

static void TestObjectFail() {
    // TODO: implement.
}

static void TestObjectDebug() {
    // TODO: implement.
}

static void TestObject() {
    TestObjectCreate();
    TestObjectDestroy();
    TestObjectIsKindOf();
    TestObjectIsMutable();
    TestObjectRespondsTo();
    TestObjectAsString();
    TestObjectHash();
    TestObjectEquals();
    TestObjectAddMethodBlock();
    TestObjectRemoveMethod();
    TestObjectProto();
    TestObjectSetProto();
    TestObjectWarn();
    TestObjectFail();
    TestObjectDebug();
}

// -------------------------------------------------------- Boolean Tests ------

static void TestBooleanCreate() {
    AssertRaises("Boolean create raises an exception, initializing booleans is not allowed") MLSend(MLBoolean, "create");
}

static void TestBooleanDestroy() {
    AssertRaises("Boolean destroy raises an exception, destroying booleans is not allowed") MLSend(MLBoolean, "destroy");
}

static void TestBooleanAsString() {
    AssertEquals(MLSend(MLBoolean, "as-string"), MLString("Boolean"), "Boolean as-string returns 'Boolean' for Boolean");
    AssertEquals(MLSend(MLYes, "as-string"), MLString("yes"), "Boolean as-string returns 'MLYes' for MLYes");
    AssertEquals(MLSend(MLNo, "as-string"), MLString("no"), "Boolean as-string returns 'MLNo' for MLNo");
}

static void TestBooleanIsMutable() {
    AssertNo(MLSend(MLBoolean, "is-mutable"), "Boolean is-mutable returns MLNo for Boolean itself");
    AssertNo(MLSend(MLYes, "is-mutable"), "Boolean is-mutable returns MLNo for MLYes");
    AssertNo(MLSend(MLNo, "is-mutable"), "Boolean is-mutable returns MLNo for MLNo");
}

static void TestBooleanEquals() {
     AssertYes(MLSend(MLNo, "equals*", MLNo), "Boolean equals* returns MLYes when comparing MLNo to MLNo");
     AssertYes(MLSend(MLYes,"equals*", MLYes), "Boolean equals* returns MLYes when comparing MLYes to MLYes");
     AssertNo(MLSend(MLNo,"equals*", MLYes), "Boolean equals* returns MLNo when comparing MLNo to MLYes");
     AssertNo(MLSend(MLYes,"equals*", MLNo), "Boolean equals* returns MLNo when comparing MLYes to MLNo");
     AssertNo(MLSend(MLYes,"equals*", MLNumber(1)), "Boolean equals* returns MLNo when comparing a boolean to comparing to any other non-boolean object");
}

static void TestBooleanCompare() {
    AssertEquals(MLSend(MLNo, "compare*", MLNo), MLNumber(0), "Boolean compare* returns 0 when comparing MLNo to MLNo");
    AssertEquals(MLSend(MLNo, "compare*", MLYes), MLNumber(-1), "Boolean compare* returns -1 when comparing MLNo to MLYes");
    AssertEquals(MLSend(MLYes, "compare*", MLNo), MLNumber(+1), "Boolean compare* returns +1 when comparing MLYes to MLNo");
    AssertEquals(MLSend(MLYes, "compare*", MLYes), MLNumber(0), "Boolean compare* returns 0 when comparing MLYes to MLYes");
    AssertNull(MLSend(MLBoolean, "compare*", MLNo), "Boolean compare* returns MLNull when comparing Boolean to MLNo");
    AssertNull(MLSend(MLBoolean, "compare*", MLYes), "Boolean compare* returns MLNull when comparing Boolean to MLYes");
    AssertNull(MLSend(MLBoolean, "compare*", MLNumber(0)), "Boolean compare* returns MLNull when comparing Boolean to anything (here a number)");
    AssertNull(MLSend(MLNo, "compare*", MLBoolean), "Boolean compare* returns MLNull when comparing MLNo to Boolean");
    AssertNull(MLSend(MLYes, "compare*", MLBoolean), "Boolean compare* returns MLNull when comparing MLYes to Boolean");
    AssertNull(MLSend(MLNo, "compare*", MLNumber(0)), "Boolean compare* returns MLNull when comparing MLNo to anything (here a number)");
    AssertNull(MLSend(MLYes, "compare*", MLNumber(0)), "Boolean compare* returns MLNull when comparing MLYes to anything (here a number)");
}

static void TestBooleanCopy() {
    AssertIdentical(MLSend(MLNo, "copy"), MLNo, "Boolean copy returns the exact same instance (here: MLNo)");
    AssertIdentical(MLSend(MLYes, "copy"), MLYes, "Boolean copy returns the exact same instance (here: MLYes)");
    AssertIdentical(MLSend(MLBoolean, "copy"), MLBoolean, "Boolean copy returns the exact same instance (here: Boolean)");
}

static void TestBoolean() {
    TestBooleanCreate();
    TestBooleanDestroy();
    TestBooleanAsString();
    TestBooleanIsMutable();
    TestBooleanEquals();
    TestBooleanCompare();
    TestBooleanCopy();
}

// --------------------------------------------------------- Number Tests ------

static void TestNumberCreate() {
    // TODO: implement.
}

static void TestNumberDestroy() {
    // TODO: implement.
}

static void TestNumberAsString() {
    // TODO: implement.
}

static void TestNumberIsMutable() {
    // TODO: implement.
}

static void TestNumberHash() {
    // TODO: implement.
}

static void TestNumberEquals() {
    AssertYes(MLSend(MLNumber(1), "equals*", MLNumber(1)), "Number equals* returns MLYes for same MLInteger numbers");
    AssertNo(MLSend(MLNumber(1), "equals*", MLNumber(2)), "Number equals* returns MLNo for different numbers");
    AssertYes(MLSend(MLNumber(1.1111), "equals*", MLNumber(1.1111)), "Number equals* returns MLYes for same MLDecimal numbers");
    AssertNo(MLSend(MLNumber(1.1112), "equals*", MLNumber(1.1111)), "Number equals* returns MLNo for MLDecimal numbers differing only by a fraction");
}

static void TestNumberCompare() {
    // TODO: implement.
}

static void TestNumberCopy() {
    // TODO: implement.
}

static void TestNumber() {
    TestNumberDestroy();
    TestNumberCreate();
    TestNumberAsString();
    TestNumberIsMutable();
    TestNumberEquals();
    TestNumberCompare();
    TestNumberCopy();
}

// ---------------------------------------------------------- Block Tests ------

static void TestBlockEquals() {
    // TODO: implement.
}

static void TestBlock() {
    TestBlockEquals();
}

// ----------------------------------------------------------- Data Tests ------

static void TestDataEquals() {
    MLVariable data1 = MLData("12345");
    MLVariable data2 = MLData("12345");
    MLVariable data3 = MLData("12346");
    MLVariable data4 = MLData("123456");
    MLVariable string = MLString("12345");
    AssertYes(MLSend(data1, "equals*", data1), "Data equals* returns MLYes when comparing identical data objects");
    AssertYes(MLSend(data1, "equals*", data2), "Data equals* returns MLYes when comparing equal data objects");
    AssertNo(MLSend(data1, "equals*", data3), "Data equals* returns MLNo when comparing data objects with equal length but different content");
    AssertNo(MLSend(data1, "equals*", data4), "Data equals* returns MLNo when comparing data objects with different length and different content");
    AssertNo(MLSend(data1, "equals*", string), "Data equals* returns MLNo when comparing a data object with a string containing the same data");
    AssertNo(MLSend(data1, "equals*", MLYes), "Data equals* returns MLNo when comparing a data object to a boolean (here: MLYes)");
    AssertNo(MLSend(data1, "equals*", MLNo), "Data equals* returns MLNo when comparing a data object to a boolean (here: MLNo)");
    AssertNo(MLSend(data1, "equals*", MLNumber(9)), "Data equals* returns MLNo when comparing a data object to a number (here: 9)");
}

static void TestData() {
    TestDataEquals();
    // TODO: add more tests.
}

// ---------------------------------------------------------- Array Tests ------

static void TestArrayEquals () {
    MLVariable array1 = MLArray(MLNumber(1), MLNumber(2), MLNumber(3));
    MLVariable array2 = MLArray(MLNumber(1), MLNumber(2), MLNumber(3));
    MLVariable array3 = MLArray(MLNumber(1), MLNumber(2), MLNumber(4));
    MLVariable array4 = MLArray(MLNumber(1), MLNumber(2), MLNumber(3), MLNumber(4));
    AssertYes(MLSend(array1, "equals*", array1), "Array equals* returns MLYes when comparing identical arrays");
    AssertYes(MLSend(array1, "equals*", array2), "Array equals* returns MLYes when comparing arrays containing the same objects");
    AssertNo(MLSend(array1, "equals*", array3), "Array equals* returns MLNo when comparing arrays with equal length but different objects");
    AssertNo(MLSend(array1, "equals*", array4), "Array equals* returns MLNo when comparing arrays with different length and different objects");
    AssertNo(MLSend(array1, "equals*", MLString("1, 2, 3")), "Array equals* returns MLNo when comparing a array object with a string (here: '1, 2, 3')");
    AssertNo(MLSend(array1, "equals*", MLYes), "Array equals* returns MLNo when comparing a array object to a boolean (here: MLYes)");
    AssertNo(MLSend(array1, "equals*", MLNo), "Array equals* returns MLNo when comparing a array object to a boolean (here: MLNo)");
    AssertNo(MLSend(array1, "equals*", MLNumber(9)), "Array equals* returns MLNo when comparing a array object to a number (here: 9)");
}

static void TestArrayCount() {
    MLVariable array1 = MLArray();
    MLVariable array2 = MLArray(MLNumber(1));
    MLVariable array3 = MLArray(MLNumber(1), MLNumber(2), MLNumber(3));
    AssertEquals(MLSend(array1, "count"), MLNumber(0), "Array count returns X for an array with X objects (here: X = 0)");
    AssertEquals(MLSend(array2, "count"), MLNumber(1), "Array count returns X for an array with X objects (here: X = 1)");
    AssertEquals(MLSend(array3, "count"), MLNumber(3), "Array count returns X for an array with X objects (here: X = 3)");
}

static void TestArrayReplaceAtCountWith() {
    MLVariable array1 = MLArray(MLNumber(1), MLNumber(2), MLNumber(3), MLNumber(4), MLMore);
    MLVariable array2 = MLArray(MLMore);
    MLVariable array3 = MLArray(MLNumber(3), MLNumber(4), MLNumber(5), MLMore);
    MLVariable array4 = MLArray(MLNumber(6), MLNumber(7), MLNumber(8), MLNumber(9), MLMore);

    AssertEquals(MLSend(array1, "replace-at*count*with*", MLNumber(1), MLNumber(2), MLArray(MLNumber(5), MLNumber(6), MLNumber(7))), MLArray(MLNumber(1), MLNumber(5), MLNumber(6), MLNumber(7), MLNumber(4)), "Array replace-at*count*with* replaces `count` objects starting at `index` with `objects`");
    AssertEquals(MLSend(array2, "replace-at*count*with*", MLNumber(0), MLNumber(0), MLArray()), MLArray(), "Array replace-at*count*with* doesn't change the array when `count` is 0 and `index` is valid");
    // TODO: check that non-mutable arrays raise an exception when trying to mutate.
}

static void TestArray() {
    TestArrayEquals();
    TestArrayCount();
    TestArrayReplaceAtCountWith();
    // TODO: add more tests.
}

// --------------------------------------------------------- String Tests ------

static void TestStringEquals() {
    MLVariable string1 = MLString("12345");
    MLVariable string2 = MLString("12345");
    MLVariable string3 = MLString("12346");
    MLVariable string4 = MLString("123456");
    AssertYes(MLSend(string1, "equals*", string1), "String equals* returns MLYes when comparing identical strings");
    AssertYes(MLSend(string1, "equals*", string2), "String equals* returns MLYes when comparing equal strings");
    AssertNo(MLSend(string1, "equals*", string3), "String equals* returns MLNo when comparing strings with equal length but different content");
    AssertNo(MLSend(string1, "equals*", string4), "String equals* returns MLNo when comparing strings with different length and different content");
    AssertNo(MLSend(string1, "equals*", MLData("12345")), "String equals* returns MLNo when comparing a string with a data object containing the same data");
    AssertNo(MLSend(string1, "equals*", MLYes), "String equals* returns MLNo when comparing a string object to a boolean (here: MLYes)");
    AssertNo(MLSend(string1, "equals*", MLNo), "String equals* returns MLNo when comparing a string object to a boolean (here: MLNo)");
    AssertNo(MLSend(string1, "equals*", MLNumber(9)), "String equals* returns MLNo when comparing a string object to a number (here: 9)");
}

static void TestString() {
    TestStringEquals();
    // TODO: add more tests.
}

// ----------------------------------------------------- Dictionary Tests ------

static void TestDictionaryEquals() {
    MLVariable dictionary1 = MLDictionary(MLString("one"), MLNumber(1), MLString("two"), MLNumber(2));
    MLVariable dictionary2 = MLDictionary(MLString("one"), MLNumber(1), MLString("two"), MLNumber(2));
    MLVariable dictionary3 = MLDictionary(MLString("one"), MLNumber(1), MLString("three"), MLNumber(3));
}

static void TestDictionaryCount() {
    MLVariable dictionary1 = MLDictionary();
    MLVariable dictionary2 = MLDictionary(MLString("one"), MLNumber(1));
    MLVariable dictionary3 = MLDictionary(MLString("one"), MLNumber(1), MLString("two"), MLNumber(2), MLString("three"), MLNumber(3));
    AssertEquals(MLSend(dictionary1, "count"), MLNumber(0), "Dictionary count returns X for a dictionary with X key/value pairs (here: X = 0)");
    AssertEquals(MLSend(dictionary2, "count"), MLNumber(1), "Dictionary count returns X for a dictionary with X key/value pairs (here: X = 1)");
    AssertEquals(MLSend(dictionary3, "count"), MLNumber(3), "Dictionary count returns X for a dictionary with X key/value pairs (here: X = 3)");
}

static void TestDictionaryGet() {
    MLVariable dictionary = MLDictionary(MLString("one"), MLNumber(1), MLString("two"), MLNumber(2));
    AssertEquals(MLSend(dictionary, "get*", MLString("one")), MLNumber(1), "Dictionary get* returns the value for the given key (here: key = 'one', value = 1)");
    AssertEquals(MLSend(dictionary, "get*", MLString("two")), MLNumber(2), "Dictionary get* returns the value for the given key (here: key = 'two', value = 2)");
    AssertNull(MLSend(dictionary, "get*", MLString("three")), "Dictionary get* returns MLNull if the given key is not in the dictionary (here: key = 'three')");
}

static void TestDictionarySetTo() {
    MLVariable dictionary = MLDictionary(MLMore);
    MLSend(dictionary, "set*to*", MLString("one"), MLNumber(1));
    AssertEquals(MLSend(dictionary, "get*", MLString("one")), MLNumber(1), "Dictionary set*to* sets the entry for `key` to the passed `value` (here: key = 'one', value = 1)");
    // TODO: check that non-mutable dictionaries raise an exception when trying to mutate.
}

static void TestDictionaryRemove() {
    MLVariable dictionary = MLDictionary(MLString("one"), MLNumber(1), MLString("two"), MLNumber(2), MLMore);
    MLSend(dictionary, "remove*", MLString("one"));
    AssertNull(MLSend(dictionary, "get*", MLString("one")), "Dictionary remove* removes the entry for `key` (here: key = 'one', value = 1)");
    // TODO: check that non-mutable dictionaries raise an exception when trying to mutate.
}

static void TestDictionary() {
    TestDictionaryEquals();
    TestDictionaryCount();
    TestDictionaryGet();
    TestDictionarySetTo();
    TestDictionaryRemove();
    // TODO: add more tests.
}

// ----------------------------------------------------------- Null Tests ------

static void TestNullEquals() {
    AssertYes(MLSend(MLNull, "equals*", MLNull), "Null equals* returns MLYes when comparing MLNull to MLNull");
    AssertNo(MLSend(MLNull, "equals*", MLYes), "Null equals* returns MLNo when comparing MLNull to any other object (here: MLYes)");
    AssertNo(MLSend(MLNull, "equals*", MLNo), "Null equals* returns MLNo when comparing MLNull to any other object (here: MLNo)");
    AssertNo(MLSend(MLNull, "equals*", MLNumber(9)), "Null equals* returns MLNo when comparing MLNull to any other object (here: 9)");
}

static void TestNull() {
    TestNullEquals();
    // TODO: add more tests.
}

// ---------------------------------------------------------------- Main -------

int main(int argumentsCount, char const* arguments[]) {
    MLCollect {
        TestBegin();
        TestObject();
        TestBoolean();
        TestNumber();
        TestBlock();
        TestData();
        TestArray();
        TestString();
        TestDictionary();
        TestNull();
        TestEnd();
    }
    return NumberOfFailedExamples > 0 ? 1 : 0;
}

// ------------------------------------------------------------- Private -------

static void TestBegin() {
    printf("\n");
    TestBeganAt = (double)clock();
}

static void TestEnd() {
    const char* color = RESET;
    if (NumberOfFailedExamples <= 0) color = GREEN;
    if (NumberOfFailedExamples >= 1) color = RED;

    TestEndedAt = (double)clock();
    const double duration = (TestEndedAt - TestBeganAt) / (double)CLOCKS_PER_SEC;

    printf("\n\n");
    if (NumberOfFailedExamples > 0) {
        for (int i = 0; i < NumberOfFailedExamples; i += 1) printf("%s%s%s", WHITE, FailedExamples[i], RESET);
        printf("\n");
    }

    printf("Finished in %.3f seconds\n", duration);
    printf("%ld examples, %s%ld failures%s\n\n", TotalNumberOfExamples, color, NumberOfFailedExamples,  RESET);
}

static void AssertYes(MLVariable object, const char* message) {
    TotalNumberOfExamples += 1;
    if (object == MLYes) {
         printf(".");
    }
    else {
        char* description = malloc(sizeof(char) * strlen(message) + 1000);
        sprintf(description, "%s ... %sFAILED%s\n", message, RED, RESET);
        FailedExamples[NumberOfFailedExamples] = description;
        NumberOfFailedExamples += 1;
        printf("%sF%s", RED, RESET);
    }
    if (TotalNumberOfExamples % 50 == 0) printf("\n");
}

static void AssertNo(MLVariable object, const char* message) {
    if (object == MLNo) return AssertYes(MLYes, message);
    if (object == MLYes) return AssertYes(MLNo, message);
    AssertYes(object, message);
}

static void AssertNull(MLVariable object, const char* message) {
    AssertYes(object == MLNull ? MLYes : MLNo, message);
}

static void AssertNotNull(MLVariable object, const char* message) {
    AssertNo(object == MLNull ? MLYes : MLNo, message);
}

static void AssertEquals(MLVariable subject, MLVariable actual, const char* message) {
    AssertYes(MLSend(subject, "equals*", actual), message);
}

static void AssertNotEqual(MLVariable subject, MLVariable actual, const char* message) {
    AssertNo(MLSend(subject, "equals*", actual), message);
}

static void AssertIdentical(MLVariable subject, MLVariable actual, const char* message) {
    AssertYes(MLBoolean(subject == actual), message);
}

static void AssertNotIdentical(MLVariable subject, MLVariable actual, const char* message) {
    AssertNo(MLBoolean(subject == actual), message);
}
