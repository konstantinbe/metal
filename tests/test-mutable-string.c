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


void CRTestMutableStringCreateMutable() {
    var string = CRStringCreateMutable();
    CRAssert(CRStringLength(string) == 0, "Length of a string created with CRStringCreateMutable() should be 0");
    CRAssert(CRStringCapacity(string) >= 0, "Capacity of a string created with CRStringCreateMutable() should be at least 0");
    CRAssert(CREquals(string, CRString("")), "A string created with CRStringCreateMutable() should equal an empty string");
    CRAssert(CRStringIsMutable(string) == true, "A string created with CRStringCreateMutable() should be mutable");
}


void CRTestMutableStringCreateMutableWithCapacity() {
    var string = CRStringCreateMutableWithCapacity(10);
    CRAssert(CRStringLength(string) == 0, "Length of a mutable string created with capacity should be 0");
    CRAssert(CRStringCapacity(string) >= 10, "Capacity of a mutable string created with capacity should be at least the passed in capacity");
    CRAssert(CREquals(string, CRString("")), "A mutable string created with capacity should equal an empty string");
    CRAssert(CRStringIsMutable(string) == true, "A mutable string created with capacity should be mutable");
}


void CRTestMutableStringCreateMutableWithCharacters() {
    const CRCharacter* characters = "Hello World!";
    const CRNatural characters_length = strlen(characters);
    const CRNatural64 characters_hash = CRDigest(strlen(characters), characters);
    const var string = CRStringCreateMutableWithCharacters(characters);

    CRAssert(CRStringLength(string) == characters_length, "Length of a mutable string created with characters should be same as strlen()");
    CRAssert(CRStringCapacity(string) >= characters_length, "Capacity of a mutable string created with characters should be at least its length");
    CRAssert(CRStringCharacters(string) != characters, "Capacity of a mutable string created with characters should copy the characters");
    CRAssert(strcmp(CRStringCharacters(string), characters) == 0, "The characters of the string should equal the characters it was created from");
    CRAssert(CRStringIsMutable(string) == true, "A mutable string created with characters should be mutable");

    assert(string.payload.natural != 0);
    assert(string.payload.natural == (CRNatural64)string.pointer);
}


void CRTestMutableStringPrepend() {
    var hello_world = CRStringCreateMutableWithCharacters("");

    CRStringPrepend(hello_world, CRString(""));
    CRAssert(CREquals(hello_world, CRString("")), "Prepending an empty string to an empty string should result in an empty string");

    CRStringPrepend(hello_world, CRString(" World!"));
    CRAssert(CREquals(hello_world, CRString(" World!")), "Prepending string ' World!' to an empty string '' should result in ' World!'");

    CRStringPrepend(hello_world, CRString("Hello"));
    CRAssert(CREquals(hello_world, CRString("Hello World!")), "Prepending string 'Hello' to a non-empty string ' World!' should result in 'Hello World!'");

    CRStringAppend(hello_world, CRString(""));
    CRAssert(CREquals(hello_world, CRString("Hello World!")), "Prepending an empty string '' to a non-empty string 'Hello World!' should not change the string resulting in 'Hello World!'");
}


void CRTestMutableStringAppend() {
    var hello_world = CRStringCreateMutableWithCharacters("");

    CRStringAppend(hello_world, CRString(""));
    CRAssert(CREquals(hello_world, CRString("")), "Appending an empty string to an empty string should result in an empty string");

    CRStringAppend(hello_world, CRString("Hello"));
    CRAssert(CREquals(hello_world, CRString("Hello")), "Appending string 'Hello' to an empty string '' should result in 'Hello', but is: '%s'", CRStringCharacters(hello_world));

    CRStringAppend(hello_world, CRString(" World!"));
    CRAssert(CREquals(hello_world, CRString("Hello World!")), "Appending string ' World!' to a non-empty string 'Hello' should result in 'Hello World!'");

    CRStringAppend(hello_world, CRString(""));
    CRAssert(CREquals(hello_world, CRString("Hello World!")), "Appending an empty string '' to a non-empty string 'Hello World!' should not change the string resulting in 'Hello World!'");
}


void CRTestMutableStringClear() {
    var hello_world = CRStringCreateMutableWithCharacters("Hello World!");
    CRAssert(CREquals(hello_world, CRString("Hello World!")), "Before clearing, string should be 'Hello World!'");
    CRStringClear(hello_world);
    CRAssert(CREquals(hello_world, CRString("")), "After clearing, string should be an empty string ''");
    CRAssert(CRStringCharacters(hello_world)[0] == '\0', "After clearing, first character of string should be the null terminator '\\0'");
}


void CRTestMutableString() {
    CRTestMutableStringCreateMutable();
    CRTestMutableStringCreateMutableWithCapacity();
    CRTestMutableStringCreateMutableWithCharacters();

    CRTestMutableStringPrepend();
    CRTestMutableStringAppend();

    CRTestMutableStringClear();
}
