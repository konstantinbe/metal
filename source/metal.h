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

#ifndef ML_METAL_H
#define ML_METAL_H

#include <stdbool.h>
#include <stdlib.h>
#include <alloca.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <math.h>

#include "commands.h"

#define MLIntegerMin –9223372036854775808ll
#define MLIntegerMax 9223372036854775807ll

#define MLNaturalMin 0
#define MLNaturalMax 18446744073709551615ull

#define MLRetainCountMax 0x0fffffffull

#define MLDebug(message, ...) printf("[DEBUG] " message "\n", ## __VA_ARGS__)
#define MLInfo(message, ...) printf("[INFO] " message "\n", ## __VA_ARGS__)
#define MLWarning(message, ...) fprintf(stderr, "[WARNING] " message "\n", ## __VA_ARGS__)
#define MLError(message, ...) { fprintf(stderr, "[ERROR] %s:%d | %s() | " message "\n", __FILE__, __LINE__, __FUNCTION__, ## __VA_ARGS__); MLDereferenceNullPointer(); }
#define MLAssert(condition, message, ...) if (!(condition)) { MLError(message, ## __VA_ARGS__); }

#if DEBUG && !(defined __clang_analyzer__)
    #define MLDereferenceNullPointer() { MLInteger *pointer = NULL; *pointer = 0; }
#else
    #define MLDereferenceNullPointer() { /* Do nothing. */ }
#endif

#define MLInline(number_of_bytes) alloca(number_of_bytes)
#define MLInlineAndClear(number_of_bytes) memset(alloca(number_of_bytes), 0, number_of_bytes)
#define MLAllocate(number_of_bytes) malloc(number_of_bytes)
#define MLAllocateAndClear(number_of_bytes) calloc(1, number_of_bytes)
#define MLResize(pointer, number_of_bytes) realloc(pointer, number_of_bytes)
#define MLFree(pointer) free(pointer)

#define MLNumberOfVariables(...) (sizeof((var[]){null, __VA_ARGS__}) / MLVariableSize - 1)
#define MLPointerToVariables(...) ((var[]){null, __VA_ARGS__} + 1)
#define MLArgument(number) (MLIsObjectNull(arguments) ? null : ((struct MLArray*)arguments.pointer)->objects[number])

#define B(value) MLBooleanMake(value)
#define N(value) MLNumberMake(value)
#define W(value) MLWordMake(value)

#define A(...) MLAutorelease(MLCopy(IA(__VA_ARGS__)))
#define S(string) MLAutorelease(MLCopy(IS(string)))
#define D(...) MLAutorelease(MLCopy(ID(__VA_ARGS__)))

#define IA(...) MLArrayMake(MLInline(MLArraySize), MLInlineArray, MLRetainCountMax, MLNumberOfVariables(__VA_ARGS__), MLNumberOfVariables(__VA_ARGS__), MLPointerToVariables(__VA_ARGS__))
#define IS(string) MLStringMake(MLInline(MLStringSize), MLInlineString, MLRetainCountMax, sizeof(string) - 1, sizeof(string) - 1, string)
#define ID(...) MLDictionaryMake(MLInline(MLDictionarySize), MLRetainCountMax, 0, MLNumberOfVariables(__VA_ARGS__) / 2, MLPointerToVariables(__VA_ARGS__))

#define MA(...) MLAutorelease(MLMutableCopy(IA(__VA_ARGS__)))
#define MS(string) MLAutorelease(MLMutableCopy(IS(string)))
#define MD(...) MLAutorelease(MLMutableCopy(ID(__VA_ARGS__)))

#define MLObjectStructure(variable) (*(struct MLObject*)(variable.pointer))
#define MLClassStructure(variable) (*(struct MLClass*)(variable.pointer))
#define MLDataStructure(variable) (*(struct MLData*)(variable.pointer))
#define MLArrayStructure(variable) (*(struct MLArray*)(variable.pointer))
#define MLStringStructure(variable) (*(struct MLString*)(variable.pointer))
#define MLDictionaryStructure(variable) (*(struct MLDictionary*)(variable.pointer))
#define MLPoolStructure(variable) (*(struct MLPool*)(variable.pointer))

#define MLVariableSize sizeof(struct MLVariable)
#define MLObjectSize sizeof(struct MLObject)
#define MLClassSize sizeof(struct MLClass)
#define MLDataSize sizeof(struct MLData)
#define MLArraySize sizeof(struct MLArray)
#define MLStringSize sizeof(struct MLString)
#define MLDictionarySize sizeof(struct MLDictionary)
#define MLPoolSize sizeof(struct MLPool)

#define MLLoadWithPriority(priority) __attribute__((constructor(priority))) void
#define MLLoad MLLoadWithPriority(1000000)

#define MLSend(self, command, ...) MLDispatch(null, self, IS(command), IA(__VA_ARGS__))
#define MLSuper(command, ...) MLDispatch(MLClassStructure(context).superclass, self, IS(command), IA(__VA_ARGS__))

#define when(expression) if (MLIsObjectTruthy(expression))
#define unless(expression) if (!MLIsObjectTruthy(expression))

#define each(object, index, array) for (var object = null, index = N(0), _count = MLCount(array); index.payload.decimal < _count.payload.decimal && (object = MLAt(array, index)).pointer != MLReference; index.payload.decimal += 1)
#define every(key, value, dictionary) // TODO: define.

#define whilst(expression) while (MLIsObjectTruthy(expression))
#define until(expression) while (!MLIsObjectTruthy(expression))

#define collect for (var pool = MLNew(MLPool); MLIsObjectTruthy(pool); pool = MLDrain(pool))
#define try for (MLTryCatchBlockStackPush(); !setjmp(MLTryCatchBlockStackTop()->destination); MLTryCatchBlockStackPop())
#define catch else for (var exception = MLTryCatchBlockStackPop()->exception, executed = no; !MLBoolFrom(executed); executed = yes)
#define throw(name) { if (MLTryCatchBlockStackTop() == NULL) MLError("Exception thrown but not catched"); MLTryCatchBlockStackTop()->exception = S(name); longjmp(MLTryCatchBlockStackTop()->destination, 1); }

typedef double MLDecimal;
typedef long long MLInteger;
typedef unsigned long long MLNatural;
typedef bool MLBool;
typedef unsigned char MLByte;
typedef void* MLPointer;

typedef union MLPayload MLPayload;
typedef struct MLVariable MLVariable;
typedef MLVariable var;

typedef var (*MLCode)(var context, var self, var command, var arguments);

union MLPayload {
    MLBool boolean;
    MLDecimal decimal;
    MLNatural natural;
    MLCode code;
    MLPointer pointer;
};

struct MLVariable {
    MLPointer pointer;
    MLPayload payload;
};

struct MLObject {
    struct MLClass* class;
    MLInteger retainCount;
};

struct MLClass {
    struct MLClass* class;
    MLInteger retainCount;
    var name;
    var superclass;
    var subclasses;
    var methods;
};

struct MLData {
    struct MLClass* class;
    MLInteger retainCount;
    MLNatural capacity;
    MLNatural count;
    MLByte* bytes;
};

struct MLArray {
    struct MLClass* class;
    MLInteger retainCount;
    MLNatural capacity;
    MLNatural count;
    var* objects;
};

struct MLString {
    struct MLClass* class;
    MLInteger retainCount;
    MLNatural capacity;
    MLNatural length;
    char* characters;
};

struct MLDictionary {
    struct MLClass* class;
    MLInteger retainCount;
    MLNatural mask;
    MLNatural count;
    var* entries;
};

struct MLPool {
    struct MLClass* class;
    MLInteger retainCount;
    var previousPool;
    var objects;
};

struct MLTryCatchBlock {
    bool executed;
    jmp_buf destination;
    var exception;
};

extern const var MLObject;
extern const var MLNull;
extern const var MLBlock;
extern const var MLBoolean;
extern const var MLNumber;
extern const var MLWord;
extern const var MLDate;
extern const var MLData;
extern const var MLArray;
extern const var MLString;
extern const var MLDictionary;
extern const var MLInlineData;
extern const var MLInlineArray;
extern const var MLInlineString;
extern const var MLInlineDictionary;
extern const var MLMutableData;
extern const var MLMutableArray;
extern const var MLMutableString;
extern const var MLMutableDictionary;
extern const var MLPool;

extern const var null;
extern const var yes;
extern const var no;

var MLReference(MLPointer pointer);

MLBool MLIsObjectNull(var object);
MLBool MLIsObjectTruthy(var object);

MLBool MLBoolFrom(var boolean);
MLInteger MLIntegerFrom(var number);
MLDecimal MLDecimalFrom(var number);
MLNatural MLNaturalFrom(var number);

struct MLTryCatchBlock* MLTryCatchBlockStackPush();
struct MLTryCatchBlock* MLTryCatchBlockStackTop();
struct MLTryCatchBlock* MLTryCatchBlockStackPop();

var MLBlockMake(MLCode code);
var MLBooleanMake(MLBool boolean);
var MLNumberMake(MLDecimal value);
var MLWordMake(MLNatural value);
var MLDateMake(MLDecimal seconds);
var MLDataMake(struct MLData* data, var class, MLInteger retainCount, MLInteger capacity, MLInteger count, MLByte* bytes);
var MLArrayMake(struct MLArray* array, var class, MLInteger retainCount, MLInteger capacity, MLInteger count, var* objects);
var MLStringMake(struct MLString* string, var class, MLInteger retainCount, MLInteger capacity, MLInteger count, char* characters);
var MLDictionaryMake(struct MLDictionary* dictionary, var class, MLInteger retainCount, MLInteger mask, MLInteger count, var* entries);
var MLPoolMake(struct MLPool* pool, var class, MLInteger retainCount, var previousPool, var objects);

var MLRequire(const char* name);
var MLModule(const char* name);
var MLImport(var module, const char* name);
var MLExport(var module, const char* name, MLCode code);
var MLDefine(const char* name, var superclass);
var MLMethod(var class, const char* command, MLCode code);

var MLLookup(var class, var command, var* foundInClass);
var MLDispatch(var context, var self, var command, var arguments);

#endif
