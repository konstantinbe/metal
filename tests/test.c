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


// --------------------------------------------------- Constants & Macros ------


#define AssertThrows(message) for (void* tryCatchBlock = TryCatchBlockPush(); tryCatchBlock != ZERO; ({ AssertNotNull(TryCatchBlockCatch(tryCatchBlock), message); true;}) && (tryCatchBlock = TryCatchBlockPop(tryCatchBlock))) if (!setjmp(TryCatchBlockTry(tryCatchBlock)))
#define AssertNotThrows(message) for (void* tryCatchBlock = TryCatchBlockPush(); tryCatchBlock != ZERO; ({ AssertNull(TryCatchBlockCatch(tryCatchBlock), message); true;}) && (tryCatchBlock = TryCatchBlockPop(tryCatchBlock))) if (!setjmp(TryCatchBlockTry(tryCatchBlock)))


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
static void AssertEqual(var subject, var actual, const char* message);
static void AssertNotEqual(var subject, var actual, const char* message);
static void AssertIdentical(var subject, var actual, const char* message);
static void AssertNotIdentical(var subject, var actual, const char* message);


// --------------------------------------------------------- Object Tests ------


static void TestObjectCreate() {
    var object = send(Object, "create");
    AssertNotNull(object, "Object create returns a newly created object");
    release(object);
}


static void TestObjectDestroy() {
    var object = send(Object, "create");
    AssertNull(send(object, "destroy"), "Object destroy destroys an object and returns null");
}


static void TestObjectInit() {
    var object1 = send(Object, "create");
    var object2 = send(object1, "init");
    AssertIdentical(object1, object2, "Object init does nothing and returns the object");
    release(object1);
}


static void TestObjectNew() {
    var object = send(Object, "new");
    AssertNotNull(object, "Object new returns a newly created an initialized object");
}


static void TestObjectIsKindOf() {
    var object = send(Object, "new");
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
    var object = send(Object, "new");
    var number = Number(5);
    AssertYes(send(object, "is-mutable"), "Object is-mutable returns yes (all objects are mutable by default)");

}


static void TestObjectRespondsTo() {
    AssertYes(send(Object, "responds-to*", String("equals*")), "Object responds-to* returns yes for an existing method");
    // TODO: enable this test after fully implementing the lookup() function.
    // AssertNo(send(Object, "responds-to*", String("winni-puh")), "Object responds-to* returns no for a non-existing method");
}


static void TestObjectAsString() {
    var object = send(Object, "new");
    AssertEqual(send(Object, "as-string"), String("Object"), "Object as-string returns 'Object' for Object");
    AssertEqual(send(object, "as-string"), String("<Object XXX>"), "Object as-string returns '<Object XXX>' for any direct instance of Object");
}


static void TestObjectHash() {
    var object = send(Object, "new");
    AssertEqual(send(object, "hash"), Number((natural)object), "Object hash returns its address as a number");
}


static void TestObjectEquals() {
    var object1 = send(Object, "new");
    var object2 = send(Object, "new");
    AssertYes(send(object1, "equals*", object1), "Object equals* returns yes for identical objects");
    AssertNo(send(object1, "equals*", object2), "Object equals* returns no when objects are not identical");
}


static void TestObjectCompare() {
    AssertThrows("Object compare* throws an exception by default, regular objects don't support comparing") send(Object, "compare*", null);
}


static void TestObjectCopy() {
    AssertThrows("Object copy throws an exception by default, regular objects don't support copying") send(Object, "copy", null);
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
    TestObjectInit();
    TestObjectNew();
    TestObjectIsKindOf();
    TestObjectIsMutable();
    TestObjectRespondsTo();
    TestObjectAsString();
    TestObjectHash();
    TestObjectEquals();
    TestObjectCompare();
    TestObjectCopy();
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
    AssertThrows("Boolean create throws an exception, creating booleans is not allowed") send(Boolean, "create");
}


static void TestBooleanDestroy() {
    AssertThrows("Boolean destroy throws an exception, destroying booleans is not allowed") send(Boolean, "destroy");
}


static void TestBooleanInit() {
    AssertThrows("Boolean init throws an exception, initializing booleans is not allowed") send(Boolean, "init");
}


static void TestBooleanAsString() {
    AssertEqual(send(Boolean, "as-string"), String("Boolean"), "Boolean as-string returns 'Boolean' for Boolean");
    AssertEqual(send(yes, "as-string"), String("yes"), "Boolean as-string returns 'yes' for yes");
    AssertEqual(send(no, "as-string"), String("no"), "Boolean as-string returns 'no' for no");
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
    AssertEqual(send(no, "compare*", no), Number(0), "Boolean compare* returns 0 when comparing no to no");
    AssertEqual(send(no, "compare*", yes), Number(-1), "Boolean compare* returns -1 when comparing no to yes");
    AssertEqual(send(yes, "compare*", no), Number(+1), "Boolean compare* returns +1 when comparing yes to no");
    AssertEqual(send(yes, "compare*", yes), Number(0), "Boolean compare* returns 0 when comparing yes to yes");
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
    TestBooleanInit();
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


static void TestNumberInit() {
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
    TestNumberCreate();
    TestNumberDestroy();
    TestNumberInit();
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


static void TestArray() {
    TestArrayEquals();
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
    // TODO: implement.
}


static void TestDictionary() {
    TestDictionaryEquals();
    // TODO: add more tests.
}


// ----------------------------------------------------------- Null Tests ------


static void TestNullCreate() {
    AssertThrows("Null create throws an exception, creating instances of Null is not allowed") send(null, "create");
}


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


static void AssertEqual(var subject, var actual, const char* message) {
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
