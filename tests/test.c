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


static const char* WHITE = "\x1B[0;97m";
static const char* RED = "\x1B[0;31m";
static const char* GREEN = "\x1B[0;32m";
static const char* RESET = "\x1B[0m";


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
}


static void TestObjectDestroy() {
    var object = send(Object, "create");
    AssertNull(send(object, "destroy"), "Object destroy destroys an object and returns null");
}


static void TestObjectInit() {
    var object1 = send(Object, "create");
    var object2 = send(object1, "init");
    AssertIdentical(object1, object2, "Object init does nothing and returns the object");
}


static void TestObjectNew() {
    var object = send(Object, "new");
    AssertNotNull(object, "Object new returns a newly created an initialized object");
}


static void TestObjectIsKindOf() {
    // TODO: implement.
}


static void TestObjectIsMutable() {
    // TODO: implement.
}


static void TestObjectRespondsTo() {
    // TODO: implement.
}


static void TestObjectDescription() {
    // TODO: implement.
}


static void TestObjectHash() {
    // TODO: implement.
}


static void TestObjectEquals() {
    var object1 = send(Object, "new");
    var object2 = send(Object, "new");
    AssertYes(send(object1, "equals*", object1), "Object equals* returns yes for identical objects");
    AssertNo(send(object1, "equals*", object2), "Object equals* returns no when objects are not identical");
}


static void TestObjectCompare() {
    // TODO: implement.
}


static void TestObjectCopy() {
    // TODO: implement.
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


static void TestObject() {
    TestObjectCreate();
    TestObjectDestroy();
    TestObjectInit();
    TestObjectNew();
    TestObjectIsKindOf();
    TestObjectIsMutable();
    TestObjectRespondsTo();
    TestObjectDescription();
    TestObjectHash();
    TestObjectEquals();
    TestObjectCompare();
    TestObjectCopy();
    TestObjectAddMethodBlock();
    TestObjectRemoveMethod();
    TestObjectProto();
    TestObjectSetProto();
}


// -------------------------------------------------------- Boolean Tests ------


static void TestBooleanCreate() {
    // TODO: implement.
}


static void TestBooleanDestroy() {
    // TODO: implement.
}


static void TestBooleanInit() {
    // TODO: implement.
}


static void TestBooleanDescription() {
    // TODO: implement.
}


static void TestBooleanIsMutable() {
    // TODO: implement.
}


static void TestBooleanEquals() {
     AssertYes(send(no, "equals*", no), "Boolean equals* returns yes when comparing no to no");
     AssertYes(send(yes,"equals*", yes), "Boolean equals* returns yes when comparing yes to yes");
     AssertNo(send(no,"equals*", yes), "Boolean equals* returns no when comparing no to yes");
     AssertNo(send(yes,"equals*", no), "Boolean equals* returns no when comparing yes to no");
     AssertNo(send(yes,"equals*", Number(1)), "Boolean equals* returns no when comparing a boolean to comparing to any other non-boolean object");
}


static void TestBooleanCompare() {
    // TODO: implement.
}


static void TestBooleanCopy() {
    // TODO: implement.
}


static void TestBoolean() {
    TestBooleanCreate();
    TestBooleanDestroy();
    TestBooleanInit();
    TestBooleanDescription();
    TestBooleanIsMutable();
    TestBooleanEquals();
    TestBooleanCompare();
    TestBooleanCopy();
}


// --------------------------------------------------------- Number Tests ------


static void TestNumberEquals() {
    AssertYes(send(Number(1), "equals*", Number(1)), "Number equals* returns yes for same integer numbers");
    AssertNo(send(Number(1), "equals*", Number(2)), "Number equals* returns no for different numbers");
    AssertYes(send(Number(1.1111), "equals*", Number(1.1111)), "Number equals* returns yes for same decimal numbers");
    AssertNo(send(Number(1.1112), "equals*", Number(1.1111)), "Number equals* returns no for decimal numbers differing only by a fraction");
}


static void TestNumber() {
    TestNumberEquals();
    // TODO: add more tests.
}


// ---------------------------------------------------------- Block Tests ------


static void TestBlockEquals() {
    // TODO: implement.
}


static void TestBlock() {
    // TODO: add more tests.
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
    const char *color = RESET;
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
