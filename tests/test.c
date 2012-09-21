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
#include "time.h"


static const char* WHITE = "\x1B[0;97m";
static const char* RED = "\x1B[0;31m";
static const char* GREEN = "\x1B[0;32m";
static const char* RESET = "\x1B[0m";


static MLInteger MLTotalNumberOfExamples = 0;
static MLInteger MLNumberOfFailedExamples = 0;
static char* MLFailedExamples[1000000];
static time_t MLTestBeganAt = 0;
static time_t MLTestEndedAt = 0;


void MLTestBegin() {
    printf("\n");
    time(&MLTestBeganAt);
}


int MLTestEnd() {
    const char *color = RESET;
    if (MLNumberOfFailedExamples <= 0) color = GREEN;
    if (MLNumberOfFailedExamples >= 1) color = RED;

    time(&MLTestEndedAt);
    const double duration = difftime(MLTestEndedAt, MLTestBeganAt);

    printf("\n\n");
    if (MLNumberOfFailedExamples > 0) {
        for (int i = 0; i < MLNumberOfFailedExamples; i += 1) printf("%s%s%s", WHITE, MLFailedExamples[i], RESET);
        printf("\n");
    }

    printf("Finished in %.1f seconds\n", duration);
    printf("%lld examples, %s%lld failures%s\n\n", MLTotalNumberOfExamples, color, MLNumberOfFailedExamples,  RESET);

    if (MLNumberOfFailedExamples > 0) return 1;
    return 0;
}


void MLAssertTrue(var object, char* message) {
    MLTotalNumberOfExamples += 1;
    when (object) {
        printf(".");
    }
    else {
        char* description = MLAllocate(sizeof(char) * strlen(message) + 1000);
        sprintf(description, "%s ... %sFAILED%s\n", message, RED, RESET);
        MLFailedExamples[MLNumberOfFailedExamples] = description;
        MLNumberOfFailedExamples += 1;
        printf("\x1B[0;31mF\x1B[0m");
    }
    if (MLTotalNumberOfExamples % 50 == 0) printf("\n");
}


void MLAssertFalse(var object, char* message) {
    MLTotalNumberOfExamples += 1;
    unless (object) {
        printf(".");
    }
    else {
        char* description = MLAllocate(sizeof(char) * strlen(message) + 1000);
        sprintf(description, "%s ... %sFAILED%s\n", message, RED, RESET);
        MLFailedExamples[MLNumberOfFailedExamples] = description;
        MLNumberOfFailedExamples += 1;
        printf("\x1B[0;31mF\x1B[0m");
    }
    if (MLTotalNumberOfExamples % 50 == 0) printf("\n");
}


void MLAssertNull(var object, char* message) {
    MLAssertTrue(B(MLIsNull(object)), message);
}


void MLAssertNotNull(var object, char* message) {
    MLAssertTrue(B(MLIsNotNull(object)), message);
}


void MLAssertEquals(var subject, var actual, char* message) {
    MLAssertTrue(MLEquals(subject, actual), message);
}


void MLAssertNotEquals(var subject, var actual, char* message) {
    MLAssertFalse(MLEquals(subject, actual), message);
}


int main(int argumentsCount, char const* arguments[]) {
    MLTestBegin();
    MLTestMetal();
    MLTestObject();
    MLTestNull();
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
    return MLTestEnd();
}
