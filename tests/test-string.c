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

#include "test.h"


void CRTestStringMake() {
    const CRCharacter* characters = "Hello World!";
    const CRNatural characters_length = strlen(characters);
    const CRNatural64 characters_hash = CRDigest(strlen(characters), characters);
    const var string = CRString(characters);

    assert(CRStringLength(string) == characters_length);
    assert(CRStringCharacters(string) == characters);

    assert(string.payload.natural != 0);
    assert(string.payload.natural == characters_hash);
}


void CRTestStringCreate() {
    var string = CRStringCreate();
    CRAssert(CRStringLength(string) == 0, "Length of a string created with CRStringCreate() should be 0");
    CRAssert(CRStringCapacity(string) == 0, "Capacity of a string created with CRStringCreate() should be 0");
    CRAssert(CREquals(string, CRString("")), "A string created with CRStringCreate() should equal an empty string");
    CRAssert(CRStringIsMutable(string) == false, "A string created with CRStringCreate() should be not mutable");
}


void CRTestStringCreateWithCharacters() {
    const CRCharacter* characters = "Hello World!";
    const CRNatural characters_length = strlen(characters);
    const CRNatural64 characters_hash = CRDigest(strlen(characters), characters);
    const var string = CRStringCreateWithCharacters(characters);

    CRAssert(CRStringLength(string) == characters_length, "Length of a string created with characters should be same as strlen()");
    CRAssert(CRStringCapacity(string) == characters_length, "Capacity of a string created with characters should be its length");
    CRAssert(CRStringCharacters(string) != characters, "Capacity of a string created with characters should be copy the characters");
    CRAssert(strcmp(CRStringCharacters(string), characters) == 0, "The characters of the string should equal the characters it was created from");
    CRAssert(CRStringIsMutable(string) == false, "A string created with characters should be not mutable");

    assert(string.payload.natural != 0);
    assert(string.payload.natural == characters_hash);
}


// -----------------------------------------------------------------------------


void CRTestStringCapacity() {
    CRAssert(CRStringCapacity(CRString("")) == 0, "Stack allocated empty string has capacity = 0");
    CRAssert(CRStringCapacity(CRString("123")) == 3, "Stack allocated '123' string has capacity = 3");

    CRAssert(CRStringCapacity(CRStringCreateWithCharacters("")) == 0, "Regular empty string has capacity = 0");
    CRAssert(CRStringCapacity(CRStringCreateWithCharacters("123")) == 3, "Regular '123' string has capacity = 3");
}


void CRTestStringLength() {
    CRAssert(CRStringLength(CRString("")) == 0, "Stack allocated empty string has length = 0");
    CRAssert(CRStringLength(CRString("123")) == 3, "Stack allocated '123' string has length = 3");

    CRAssert(CRStringLength(CRStringCreateWithCharacters("")) == 0, "Regular empty string has length = 0");
    CRAssert(CRStringLength(CRStringCreateWithCharacters("123")) == 3, "Regular '123' string has length = 3");
}


void CRTestStringCharacters() {
    CRCharacter* characters = "Hello World!";
    var string = CRString(characters);
    assert(CRStringCharacters(string) == characters);
}


void CRTestStringIsEmpty() {
    CRAssert(CRStringIsEmpty(CRString("")) == true, "Empty string '' should be empty");
    CRAssert(CRStringIsEmpty(CRString("123")) == false, "String '123' should not be empty");
}


void CRTestStringIsMutable() {
    CRAssert(CRStringIsMutable(CRString("123")) == false, "A stack allocated non-mutable string should not be mutable");
    CRAssert(CRStringIsMutable(CRStringCreateWithCharacters("123")) == false, "A non-mutable string should not be mutable");
    CRAssert(CRStringIsMutable(CRStringCreateMutableWithCharacters("123")) == true, "A mutable string should be mutable");
}


// -----------------------------------------------------------------------------


void CRTestStringCharacterAt() {
    var string = CRString("Hello World!");
    assert(CRStringCharacterAt(string, 0) == 'H');
    assert(CRStringCharacterAt(string, 1) == 'e');
    assert(CRStringCharacterAt(string, 2) == 'l');
    assert(CRStringCharacterAt(string, 3) == 'l');
    assert(CRStringCharacterAt(string, 4) == 'o');
    assert(CRStringCharacterAt(string, 5) == ' ');
    assert(CRStringCharacterAt(string, 6) == 'W');
    assert(CRStringCharacterAt(string, 7) == 'o');
    assert(CRStringCharacterAt(string, 8) == 'r');
    assert(CRStringCharacterAt(string, 9) == 'l');
    assert(CRStringCharacterAt(string, 10) == 'd');
    assert(CRStringCharacterAt(string, 11) == '!');
}


void CRTestStringIndexOf() {
    const var string = CRString("Hello World!");
    CRAssert(CRStringIndexOf(string, CRString("")) == 0, "Index of empty string should always be 0");

    CRAssert(CRStringIndexOf(string, CRString("H")) == 0, "Index of 'H' in 'Hello World!' should be 0");
    CRAssert(CRStringIndexOf(string, CRString("Hello")) == 0, "Index of 'Hello' in 'Hello World!' should be 0");
    CRAssert(CRStringIndexOf(string, CRString("Hello World!")) == 0, "Index of 'Hello World!' in 'Hello World!' should be 0");

    CRAssert(CRStringIndexOf(string, CRString("o")) == 4, "Index of 'o' in 'Hello World!' should be 4");
    CRAssert(CRStringIndexOf(string, CRString("lo")) == 3, "Index of 'lo' in 'Hello World!' should be 3");
    CRAssert(CRStringIndexOf(string, CRString("lo Wo")) == 3, "Index of 'lo Wo' in 'Hello World!' should be 3");

    CRAssert(CRStringIndexOf(string, CRString("!")) == 11, "Index of '!' in 'Hello World!' should be 11");
    CRAssert(CRStringIndexOf(string, CRString("World!")) == 6, "Index of 'World!' in 'Hello World!' should be 6");

    CRAssert(CRStringIndexOf(string, CRString("x")) == -1, "Index of 'x' in 'Hello World!' should be -1 (not found)");
    CRAssert(CRStringIndexOf(string, CRString("world!")) == -1, "Index of 'world!' in 'Hello World!' should be -1 (not found) because 'w' is lowercase");
}


void CRTestStringContains() {
    const var string = CRString("Hello World!");

    CRAssert(CRStringContains(CRString(""), CRString("")) == true, "Empty string '' should cointain the empty string ''");
    CRAssert(CRStringContains(string, CRString("")) == true, "Any string should cointain the empty string ''");

    CRAssert(CRStringContains(string, CRString("H")) == true, "String 'Hello World!' should cointain the string 'H'");
    CRAssert(CRStringContains(string, CRString("Hello")) == true, "String 'Hello World!' should cointain the string 'Hello'");
    CRAssert(CRStringContains(string, CRString("Hello World!")) == true, "String 'Hello World!' should cointain the string 'Hello World!'");

    CRAssert(CRStringContains(string, CRString("o")) == true, "String 'Hello World!' should cointain the string 'o'");
    CRAssert(CRStringContains(string, CRString("lo")) == true, "String 'Hello World!' should cointain the string 'lo'");

    CRAssert(CRStringContains(string, CRString("!")) == true, "String 'Hello World!' should cointain the string '!'");
    CRAssert(CRStringContains(string, CRString("World!")) == true, "String 'Hello World!' should cointain the string 'World!'");

    CRAssert(CRStringContains(string, CRString("x")) == false, "String 'Hello World!' should not cointain the string 'x'");
    CRAssert(CRStringContains(string, CRString("world!")) == false, "String 'Hello World!' should not cointain the string 'world!' (case sensitive)");
}


void CRTestStringBeginsWith() {
    var string = CRString("Hello World!");
    CRAssert(CRStringBeginsWith(CRString(""), CRString("")) == true, "Empty string '' should begin with the empty string ''");

    CRAssert(CRStringBeginsWith(string, CRString("")) == true, "Any string should begin with the empty string ''");
    CRAssert(CRStringBeginsWith(string, CRString("Hello")) == true, "String 'Hello World!' should begin with the string 'Hello'");
    CRAssert(CRStringBeginsWith(string, CRString("Hello World!")) == true, "String 'Hello World!' should begin with the string 'Hello World!'");

}


void CRTestStringEndsWith() {
    var string = CRString("Hello World!");
    CRAssert(CRStringEndsWith(CRString(""), CRString("")) == true, "Empty string '' should end with the empty string ''");

    CRAssert(CRStringEndsWith(string, CRString("")) == true, "Any string should end with the empty string ''");
    CRAssert(CRStringEndsWith(string, CRString("World!")) == true, "String 'Hello World!' should end with the string 'World!'");
    CRAssert(CRStringEndsWith(string, CRString("Hello World!")) == true, "String 'Hello World!' should end with the string 'Hello World!'");
}


// -----------------------------------------------------------------------------


void CRTestStringHash() {
    const CRCharacter* find_method_for = "find_method*for*";
    const CRCharacter* method_not_found_for = "method*not_found_for*";

    const CRNatural find_method_for_length = strlen(find_method_for);
    const CRNatural method_not_found_for_length = strlen(method_not_found_for);

    CRNatural64 find_method_for_hash = 0xe922bccb387e52bfull;
    CRNatural64 method_not_found_for_hash = 0xcd42b3fddaaa6e97ull;

    CRAssert(CRHash(CRString(find_method_for)) == find_method_for_hash, "Hash of 'find_method*for*' should be 0xe922bccb387e52bfull");
    CRAssert(CRHash(CRString(method_not_found_for)) == method_not_found_for_hash, "Hash of 'method*not_found_for*' should be 0xcd42b3fddaaa6e97ull");

    var method_not_found_for_string = CRString(method_not_found_for);
    method_not_found_for_string.payload.natural = 0;
    CRAssert(CRHash(method_not_found_for_string) == method_not_found_for_hash, "If payload of a string variable is 0, the hash should be computed on the fly");
}


void CRTestStringEquals() {
    CRAssert(CREquals(CRString(""), CRString("")), "an empty string should equal empty string");
    CRAssert(CREquals(CRString("Hello World!"), CRString("Hello World!")), "exactly equal strings should be equal");
    CRAssert(!CREquals(CRString("Hello World!"), CRString("hello World!")), "Strings differing only in capitalization should not be equal");
}


void CRTestStringCopy() {
    var string = CRStringCreateWithCharacters("Hello World!");

    const CRNatural string_retain_count_before_copying = CRRetainCount(string);
    var copy_of_string = CRCopy(string);
    const CRNatural string_retain_count_after_copying = CRRetainCount(string);

    CRAssert(string.pointer == copy_of_string.pointer, "Copy of a non-mutable string should be the same instance");
    CRAssert(string_retain_count_after_copying == string_retain_count_before_copying + 1, "Copy of a non-mutable string should be the same instance retained");
}


void CRTestStringMutableCopy() {
    var string = CRStringCreateWithCharacters("Hello World!");
    var copy_of_string = CRMutableCopy(string);

    CRAssert(string.pointer != copy_of_string.pointer, "Mutable copy of a non-mutable string should not be the same instance");
    CRAssert(CREquals(string, copy_of_string), "Mutable copy of a non-mutable string should equal the copied string");
    CRAssert(CRStringIsMutable(copy_of_string), "Mutable copy of a non-mutable string should be mutable");
}


void CRTestStringDestroy() {
    var string = CRStringCreateWithCharacters("123");
    CRRelease(string);
}


void CRTestStringDescription() {
    CRExpectObjectToBeEqualTo(CRDescription(CRString("1234")), CRString("1234"));
}


// -----------------------------------------------------------------------------


void CRTestString() {
    CRTestStringMake();
    CRTestStringCreate();
    CRTestStringCreateWithCharacters();

    CRTestStringCapacity();
    CRTestStringLength();
    CRTestStringCharacters();
    CRTestStringIsEmpty();
    CRTestStringIsMutable();

    CRTestStringCharacterAt();
    CRTestStringIndexOf();
    CRTestStringContains();
    CRTestStringBeginsWith();
    CRTestStringEndsWith();

    CRTestStringHash();
    CRTestStringEquals();
    CRTestStringCopy();
    CRTestStringMutableCopy();
    CRTestStringDestroy();
    CRTestStringDescription();
}
