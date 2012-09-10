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


const char* OK = "\x1B[0;32mOK\x1B[0m";
const char* FAILED = "\x1B[0;31mFAILED\x1B[0m";


void MLExpectToBeTrue(var object, char* message) {
    when (object) {
        printf("%s ... %s\n", message, OK);
    }
    else {
        printf("%s ... %s\n", message, FAILED);
    }
}


void MLExpectToBeFalse(var object, char* message) {
    unless (object) {
        printf("%s ... %s\n", message, OK);
    }
    else {
        printf("%s ... %s\n", message, FAILED);
    }
}


void MLExpectToEqual(var subject, var actual, char* message) {
    MLExpectToBeTrue(MLEquals(subject, actual), message);
}


void MLExpectToNotEqual(var subject, var actual, char* message) {
    MLExpectToBeFalse(MLEquals(subject, actual), message);
}


int main(int argumentsCount, char const* arguments[]) {
    MLTestMetal();
    MLTestObject();
    MLTestBlock();
    MLTestBoolean();
    MLTestNumber();
    MLTestWord();
    MLTestDate();
    MLTestData();
    MLTestArray();
    MLTestString();
    MLTestDictionary();
    MLTestPool();
    return 0;
}
