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
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// --------------------------------------------------- Constants & Macros ------


#define AssertThrows(message) for (void* tryCatchBlock = TryCatchBlockPush(); tryCatchBlock != ZERO; ({ AssertNotNull(TryCatchBlockCatch(tryCatchBlock), message); true; }) && (tryCatchBlock = TryCatchBlockPop(tryCatchBlock))) if (!setjmp(TryCatchBlockTry(tryCatchBlock)))
#define AssertNotThrows(message) for (void* tryCatchBlock = TryCatchBlockPush(); tryCatchBlock != ZERO; ({ AssertNull(TryCatchBlockCatch(tryCatchBlock), message); true; }) && (tryCatchBlock = TryCatchBlockPop(tryCatchBlock))) if (!setjmp(TryCatchBlockTry(tryCatchBlock)))


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


static void AssertYes(var object, const char* message);
static void AssertNo(var object, const char* message);
static void AssertNull(var object, const char* message);
static void AssertNotNull(var object, const char* message);
static void AssertEquals(var subject, var actual, const char* message);
static void AssertNotEqual(var subject, var actual, const char* message);
static void AssertIdentical(var subject, var actual, const char* message);
static void AssertNotIdentical(var subject, var actual, const char* message);


// --------------------------------------------------------- Object Tests ------


static void TestObjectCreate() {
    var object = send(Object, "create");
    AssertNotNull(object, "Object create calls allocate and returns the newly allocated object");
}


static void TestObjectDestroy() {
    // Destroy can't be properly tested because created objects are autoreleased.
}


static void TestObjectIsKindOf() {
    var object = send(Object, "create");
    var number = Number(5);
    AssertYes(send(Object, "is-kind-of*", Object), "Object is-kind-of* returns yes when asking Object whether it is a kind of itself");
    AssertYes(send(object, "is-kind-of*", object), "Object is-kind-of* returns yes when asking an object whether it is a kind of itself");
    AssertYes(send(Number, "is-kind-of*", Object), "Object is-kind-of* returns yes when asking Number whether it is a kind of Object");
    AssertYes(send(object, "is-kind-of*", Object), "Object is-kind-of* returns yes when asking an object whether it is a kind of Object");
    AssertYes(send(number, "is-kind-of*", Number), "Object is-kind-of* returns yes when asking a number whether it is a kind of Number");
    AssertYes(send(number, "is-kind-of*", Object), "Object is-kind-of* returns yes when asking a number whether it is a kind of Object");
    AssertNo(send(Object, "is-kind-of*", object), "Object is-kind-of* returns no when asking Object whether it is a kind of an object");
    AssertNo(send(object, "is-kind-of*", Number), "Object is-kind-of* returns no when asking an object whether it is a kind of Number");
    AssertNo(send(number, "is-kind-of*", object), "Object is-kind-of* returns no when asking a number whether it is a kind of object");
}


static void TestObjectIsMutable() {
    var object1 = send(Object, "create");
    var object2 = send(Object, "create", String("mutable"), no);
    var object3 = send(Object, "create", String("mutable"), yes);
    AssertNo(send(object1, "is-mutable"), "Object is-mutable returns no (all objects are not mutable by default)");
    AssertNo(send(object2, "is-mutable"), "Object is-mutable returns no if option mutable = no was passed to create");
    AssertYes(send(object3, "is-mutable"), "Object is-mutable returns yes if option mutable = yes was passed to create");
}


static void TestObjectRespondsTo() {
    AssertYes(send(Object, "responds-to*", String("equals*")), "Object responds-to* returns yes for an existing method");
    // TODO: enable this test after fully implementing the lookup() function.
    // AssertNo(send(Object, "responds-to*", String("winni-puh")), "Object responds-to* returns no for a non-existing method");
}


static void TestObjectAsString() {
    var object = send(Object, "create");
    AssertEquals(send(Object, "as-string"), String("Object"), "Object as-string returns 'Object' for Object");
    AssertEquals(send(object, "as-string"), String("<Object XXX>"), "Object as-string returns '<Object XXX>' for any direct instance of Object");
}


static void TestObjectHash() {
    var object = send(Object, "create");
    AssertEquals(send(object, "hash"), Number((natural)object), "Object hash returns its address as a number");
}


static void TestObjectEquals() {
    var object1 = send(Object, "create");
    var object2 = send(Object, "create");
    AssertYes(send(object1, "equals*", object1), "Object equals* returns yes for identical objects");
    AssertNo(send(object1, "equals*", object2), "Object equals* returns no when objects are not identical");
}


static void TestObjectSelf() {
    var object = send(Object, "create");
    AssertIdentical(object, send(object, "self"), "Object self returns itself");
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


static void TestObjectInfo() {
    // TODO: implement.
}


static void TestObjectWarning() {
    // TODO: implement.
}


static void TestObjectError() {
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
    TestObjectInfo();
    TestObjectWarning();
    TestObjectError();
    TestObjectDebug();
}


// -------------------------------------------------------- Boolean Tests ------


static void TestBooleanCreate() {
    AssertThrows("Boolean create throws an exception, initializing booleans is not allowed") send(Boolean, "create");
}


static void TestBooleanDestroy() {
    AssertThrows("Boolean destroy throws an exception, destroying booleans is not allowed") send(Boolean, "destroy");
}


static void TestBooleanAsString() {
    AssertEquals(send(Boolean, "as-string"), String("Boolean"), "Boolean as-string returns 'Boolean' for Boolean");
    AssertEquals(send(yes, "as-string"), String("yes"), "Boolean as-string returns 'yes' for yes");
    AssertEquals(send(no, "as-string"), String("no"), "Boolean as-string returns 'no' for no");
}


static void TestBooleanIsMutable() {
    AssertNo(send(Boolean, "is-mutable"), "Boolean is-mutable returns no for Boolean itself");
    AssertNo(send(yes, "is-mutable"), "Boolean is-mutable returns no for yes");
    AssertNo(send(no, "is-mutable"), "Boolean is-mutable returns no for no");
}


static void TestBooleanEquals() {
     AssertYes(send(no, "equals*", no), "Boolean equals* returns yes when comparing no to no");
     AssertYes(send(yes,"equals*", yes), "Boolean equals* returns yes when comparing yes to yes");
     AssertNo(send(no,"equals*", yes), "Boolean equals* returns no when comparing no to yes");
     AssertNo(send(yes,"equals*", no), "Boolean equals* returns no when comparing yes to no");
     AssertNo(send(yes,"equals*", Number(1)), "Boolean equals* returns no when comparing a boolean to comparing to any other non-boolean object");
}


static void TestBooleanCompare() {
    AssertEquals(send(no, "compare*", no), Number(0), "Boolean compare* returns 0 when comparing no to no");
    AssertEquals(send(no, "compare*", yes), Number(-1), "Boolean compare* returns -1 when comparing no to yes");
    AssertEquals(send(yes, "compare*", no), Number(+1), "Boolean compare* returns +1 when comparing yes to no");
    AssertEquals(send(yes, "compare*", yes), Number(0), "Boolean compare* returns 0 when comparing yes to yes");
    AssertNull(send(Boolean, "compare*", no), "Boolean compare* returns null when comparing Boolean to no");
    AssertNull(send(Boolean, "compare*", yes), "Boolean compare* returns null when comparing Boolean to yes");
    AssertNull(send(Boolean, "compare*", Number(0)), "Boolean compare* returns null when comparing Boolean to anything (here a number)");
    AssertNull(send(no, "compare*", Boolean), "Boolean compare* returns null when comparing no to Boolean");
    AssertNull(send(yes, "compare*", Boolean), "Boolean compare* returns null when comparing yes to Boolean");
    AssertNull(send(no, "compare*", Number(0)), "Boolean compare* returns null when comparing no to anything (here a number)");
    AssertNull(send(yes, "compare*", Number(0)), "Boolean compare* returns null when comparing yes to anything (here a number)");
}


static void TestBooleanCopy() {
    AssertIdentical(send(no, "copy"), no, "Boolean copy returns the exact same instance (here: no)");
    AssertIdentical(send(yes, "copy"), yes, "Boolean copy returns the exact same instance (here: yes)");
    AssertIdentical(send(Boolean, "copy"), Boolean, "Boolean copy returns the exact same instance (here: Boolean)");
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
    AssertYes(send(Number(1), "equals*", Number(1)), "Number equals* returns yes for same integer numbers");
    AssertNo(send(Number(1), "equals*", Number(2)), "Number equals* returns no for different numbers");
    AssertYes(send(Number(1.1111), "equals*", Number(1.1111)), "Number equals* returns yes for same decimal numbers");
    AssertNo(send(Number(1.1112), "equals*", Number(1.1111)), "Number equals* returns no for decimal numbers differing only by a fraction");
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
    var data1 = Data("12345");
    var data2 = Data("12345");
    var data3 = Data("12346");
    var data4 = Data("123456");
    var string = String("12345");
    AssertYes(send(data1, "equals*", data1), "Data equals* returns yes when comparing identical data objects");
    AssertYes(send(data1, "equals*", data2), "Data equals* returns yes when comparing equal data objects");
    AssertNo(send(data1, "equals*", data3), "Data equals* returns no when comparing data objects with equal length but different content");
    AssertNo(send(data1, "equals*", data4), "Data equals* returns no when comparing data objects with different length and different content");
    AssertNo(send(data1, "equals*", string), "Data equals* returns no when comparing a data object with a string containing the same data");
    AssertNo(send(data1, "equals*", yes), "Data equals* returns no when comparing a data object to a boolean (here: yes)");
    AssertNo(send(data1, "equals*", no), "Data equals* returns no when comparing a data object to a boolean (here: no)");
    AssertNo(send(data1, "equals*", Number(9)), "Data equals* returns no when comparing a data object to a number (here: 9)");
}


static void TestData() {
    TestDataEquals();
    // TODO: add more tests.
}


// ---------------------------------------------------------- Array Tests ------


static void TestArrayEquals () {
    var array1 = Array(Number(1), Number(2), Number(3));
    var array2 = Array(Number(1), Number(2), Number(3));
    var array3 = Array(Number(1), Number(2), Number(4));
    var array4 = Array(Number(1), Number(2), Number(3), Number(4));
    AssertYes(send(array1, "equals*", array1), "Array equals* returns yes when comparing identical arrays");
    AssertYes(send(array1, "equals*", array2), "Array equals* returns yes when comparing arrays containing the same objects");
    AssertNo(send(array1, "equals*", array3), "Array equals* returns no when comparing arrays with equal length but different objects");
    AssertNo(send(array1, "equals*", array4), "Array equals* returns no when comparing arrays with different length and different objects");
    AssertNo(send(array1, "equals*", String("1, 2, 3")), "Array equals* returns no when comparing a array object with a string (here: '1, 2, 3')");
    AssertNo(send(array1, "equals*", yes), "Array equals* returns no when comparing a array object to a boolean (here: yes)");
    AssertNo(send(array1, "equals*", no), "Array equals* returns no when comparing a array object to a boolean (here: no)");
    AssertNo(send(array1, "equals*", Number(9)), "Array equals* returns no when comparing a array object to a number (here: 9)");
}


static void TestArrayCount() {
    var array1 = Array();
    var array2 = Array(Number(1));
    var array3 = Array(Number(1), Number(2), Number(3));
    AssertEquals(send(array1, "count"), Number(0), "Array count returns X for an array with X objects (here: X = 0)");
    AssertEquals(send(array2, "count"), Number(1), "Array count returns X for an array with X objects (here: X = 1)");
    AssertEquals(send(array3, "count"), Number(3), "Array count returns X for an array with X objects (here: X = 3)");
}


static void TestArrayReplaceAtCountWith() {
    var array1 = Array(Number(1), Number(2), Number(3), Number(4), more);
    var array2 = Array(more);
    var array3 = Array(Number(3), Number(4), Number(5), more);
    var array4 = Array(Number(6), Number(7), Number(8), Number(9), more);

    AssertEquals(send(array1, "replace-at*count*with*", Number(1), Number(2), Array(Number(5), Number(6), Number(7))), Array(Number(1), Number(5), Number(6), Number(7), Number(4)), "Array replace-at*count*with* replaces `count` objects starting at `index` with `objects`");
    AssertEquals(send(array2, "replace-at*count*with*", Number(0), Number(0), Array()), Array(), "Array replace-at*count*with* doesn't change the array when `count` is 0 and `index` is valid");
    // TODO: check that non-mutable arrays throw an exception when trying to mutate.
}


static void TestArray() {
    TestArrayEquals();
    TestArrayCount();
    TestArrayReplaceAtCountWith();
    // TODO: add more tests.
}


// --------------------------------------------------------- String Tests ------


static void TestStringEquals() {
    var string1 = String("12345");
    var string2 = String("12345");
    var string3 = String("12346");
    var string4 = String("123456");
    AssertYes(send(string1, "equals*", string1), "String equals* returns yes when comparing identical strings");
    AssertYes(send(string1, "equals*", string2), "String equals* returns yes when comparing equal strings");
    AssertNo(send(string1, "equals*", string3), "String equals* returns no when comparing strings with equal length but different content");
    AssertNo(send(string1, "equals*", string4), "String equals* returns no when comparing strings with different length and different content");
    AssertNo(send(string1, "equals*", Data("12345")), "String equals* returns no when comparing a string with a data object containing the same data");
    AssertNo(send(string1, "equals*", yes), "String equals* returns no when comparing a string object to a boolean (here: yes)");
    AssertNo(send(string1, "equals*", no), "String equals* returns no when comparing a string object to a boolean (here: no)");
    AssertNo(send(string1, "equals*", Number(9)), "String equals* returns no when comparing a string object to a number (here: 9)");
}


static void TestString() {
    TestStringEquals();
    // TODO: add more tests.
}


// ----------------------------------------------------- Dictionary Tests ------


static void TestDictionaryEquals() {
    var dictionary1 = Dictionary(String("one"), Number(1), String("two"), Number(2));
    var dictionary2 = Dictionary(String("one"), Number(1), String("two"), Number(2));
    var dictionary3 = Dictionary(String("one"), Number(1), String("three"), Number(3));
}


static void TestDictionaryCount() {
    var dictionary1 = Dictionary();
    var dictionary2 = Dictionary(String("one"), Number(1));
    var dictionary3 = Dictionary(String("one"), Number(1), String("two"), Number(2), String("three"), Number(3));
    AssertEquals(send(dictionary1, "count"), Number(0), "Dictionary count returns X for a dictionary with X key/value pairs (here: X = 0)");
    AssertEquals(send(dictionary2, "count"), Number(1), "Dictionary count returns X for a dictionary with X key/value pairs (here: X = 1)");
    AssertEquals(send(dictionary3, "count"), Number(3), "Dictionary count returns X for a dictionary with X key/value pairs (here: X = 3)");
}


static void TestDictionaryGet() {
    var dictionary = Dictionary(String("one"), Number(1), String("two"), Number(2));
    AssertEquals(send(dictionary, "get*", String("one")), Number(1), "Dictionary get* returns the value for the given key (here: key = 'one', value = 1)");
    AssertEquals(send(dictionary, "get*", String("two")), Number(2), "Dictionary get* returns the value for the given key (here: key = 'two', value = 2)");
    AssertNull(send(dictionary, "get*", String("three")), "Dictionary get* returns null if the given key is not in the dictionary (here: key = 'three')");
}


static void TestDictionarySetTo() {
    var dictionary = Dictionary(more);
    send(dictionary, "set*to*", String("one"), Number(1));
    AssertEquals(send(dictionary, "get*", String("one")), Number(1), "Dictionary set*to* sets the entry for `key` to the passed `value` (here: key = 'one', value = 1)");
    // TODO: check that non-mutable dictionaries throw an exception when trying to mutate.
}


static void TestDictionaryRemove() {
    var dictionary = Dictionary(String("one"), Number(1), String("two"), Number(2), more);
    send(dictionary, "remove*", String("one"));
    AssertNull(send(dictionary, "get*", String("one")), "Dictionary remove* removes the entry for `key` (here: key = 'one', value = 1)");
    // TODO: check that non-mutable dictionaries throw an exception when trying to mutate.
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
    AssertYes(send(null, "equals*", null), "Null equals* returns yes when comparing null to null");
    AssertNo(send(null, "equals*", yes), "Null equals* returns no when comparing null to any other object (here: yes)");
    AssertNo(send(null, "equals*", no), "Null equals* returns no when comparing null to any other object (here: no)");
    AssertNo(send(null, "equals*", Number(9)), "Null equals* returns no when comparing null to any other object (here: 9)");
}


static void TestNull() {
    TestNullEquals();
    // TODO: add more tests.
}


// ------------------------------------------------ Import & Export Tests ------


static integer ImportExportTestObjectCount = 0;


static var ImportExportTestObject() {
    ImportExportTestObjectCount += 1;
    return Number(123456789);
}


static void TestImportAndExport() {
    const char* name = "ImportExport.TestObject";
    var const objectBeforeImport = import(name);
    export(name, ImportExportTestObject);
    var const objectAfterImport = import(name);

    AssertNull(objectBeforeImport, "Importing an object that hasn't been exported yet should return null");
    AssertEquals(objectAfterImport, Number(123456789), "Importing an object that has been exported should return the exported object");
    AssertYes(Boolean(ImportExportTestObjectCount == 1), "When importing an exported object for the first time, its loading function should be called once");

    var const objectAfterSecondImport = import(name);
    AssertYes(Boolean(ImportExportTestObjectCount == 1), "When importing an exported object multiple times, its loading function should be called only once");
    AssertEquals(objectAfterImport, objectAfterSecondImport, "When importing an exported object multiple times, the same object should be returned every time");

    // TODO: test cycle detection.
}


// ---------------------------------------------------------------- Main -------


int main(int argumentsCount, char const* arguments[]) {
    collect {
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
        TestImportAndExport();
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


static void AssertYes(var object, const char* message) {
    TotalNumberOfExamples += 1;
    if (object == yes) {
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


static void AssertNo(var object, const char* message) {
    if (object == no) return AssertYes(yes, message);
    if (object == yes) return AssertYes(no, message);
    AssertYes(object, message);
}


static void AssertNull(var object, const char* message) {
    AssertYes(object == null ? yes : no, message);
}


static void AssertNotNull(var object, const char* message) {
    AssertNo(object == null ? yes : no, message);
}


static void AssertEquals(var subject, var actual, const char* message) {
    AssertYes(send(subject, "equals*", actual), message);
}


static void AssertNotEqual(var subject, var actual, const char* message) {
    AssertNo(send(subject, "equals*", actual), message);
}


static void AssertIdentical(var subject, var actual, const char* message) {
    AssertYes(Boolean(subject == actual), message);
}


static void AssertNotIdentical(var subject, var actual, const char* message) {
    AssertNo(Boolean(subject == actual), message);
}
