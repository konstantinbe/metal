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

#ifndef ML_METAL_H
#define ML_METAL_H

#include <float.h>
#include <setjmp.h>
#include <limits.h>
#include <stdbool.h>

#define ML_METAL_VERSION "x.x.x"

#define MLZero (void*)0ul
#define MLMore (void*)ULLONG_MAX

#define MLMetalHelperJoinJoin(x, y) x ## y
#define MLMetalHelperJoin(x, y) MLMetalHelperJoinJoin(x, y)
#define MLMetalHelperStringify(x) (((char*)(#x))[0] == '"' ? MLString(x) : (x))

#define MLLoad __attribute__((constructor(255))) static void MLMetalHelperJoin(__MetalLoadBlock, __COUNTER__)()
#define MLCollect for (void* collectBlock = MLCollectBlockPush(); collectBlock != MLZero; collectBlock = MLCollectBlockPop(collectBlock))

#define MLPerform for (void* performHandleBlock = MLPerformHandleBlockPush(); performHandleBlock != MLZero; performHandleBlock = MLPerformHandleBlockPop(performHandleBlock)) if (!setjmp(MLPerformHandleBlockPerform(performHandleBlock)))
#define MLHandle else for (MLVariable exception = MLPerformHandleBlockHandle(performHandleBlock); exception != MLNull; exception = MLNull)

#define MLSend(self, command, ...) ({MLVariable const selfToSend = (self); MLVariable const commandToSend = MLMetalHelperStringify(command); MLVariable superToSend = MLZero; MLCode const codeToCall = MLLookup(self, commandToSend, &superToSend); codeToCall(selfToSend, superToSend, commandToSend, ## __VA_ARGS__, MLZero);})
#define MLSuper(self, command, ...) ({MLVariable const selfToSend = (self); MLVariable const commandToSend = MLMetalHelperStringify(command); MLVariable superToSend = MLZero; MLCode const codeToCall = MLLookup(super, commandToSend, &superToSend); codeToCall(selfToSend, superToSend, commandToSend, ## __VA_ARGS__, MLZero);})

#define MLOption(name, initial) ({ MLVariable nameAsString = MLMetalHelperStringify(name); va_list list; va_start(list, options); MLVariable key = options; MLVariable value = MLZero; while (key != MLZero && key != (nameAsString)) { value = va_arg(list, MLVariable); key = va_arg(list, MLVariable); } va_end(list); key ? va_arg(list, MLVariable) : (initial); });
#define MLOptions(...) __VA_ARGS__

#define MLBoolean(boolean) ((boolean) ? MLYes : MLNo)
#define MLNumber(number) MLCollectBlockAdd(MLNumberUncollected(number))
#define MLBlock(code) MLCollectBlockAdd(MLBlockUncollected(code))
#define MLData(data) MLCollectBlockAdd(MLDataUncollected(data))
#define MLArray(...) MLCollectBlockAdd(MLArrayUncollected(__VA_ARGS__))
#define MLString(string) MLCollectBlockAdd(MLStringUncollected(string))
#define MLDictionary(...) MLCollectBlockAdd(MLDictionaryUncollected(__VA_ARGS__))

#define MLNumberUncollected(number) MLNumberMake((MLDecimal)(number))
#define MLBlockUncollected(code) MLBlockMake((void*)(code))
#define MLDataUncollected(data) MLDataMake(sizeof(data), (void*)(data))
#define MLArrayUncollected(...) MLArrayMake((sizeof((MLVariable[]){MLZero, ## __VA_ARGS__}) / sizeof(MLVariable)) - 1, ## __VA_ARGS__, MLZero)
#define MLStringUncollected(string) MLStringMake(sizeof(string), (string))
#define MLDictionaryUncollected(...) MLDictionaryMake((sizeof((MLVariable[]){MLZero, ## __VA_ARGS__}) / sizeof(MLVariable)) - 1, ## __VA_ARGS__, MLZero)

#define MLIntegerMax ((MLInteger)LONG_MAX)
#define MLIntegerMin ((MLInteger)LONG_MIN)

#define MLNaturalMax ((MLNatural)ULONG_MAX)
#define MLNaturalMin ((MLNatural)0ul)

#define MLDecimalMax ((MLDecimal)DBL_MAX)
#define MLDecimalMin ((MLDecimal)DBL_MIN)

typedef void* MLVariable;
typedef long MLInteger;
typedef unsigned long MLNatural;
typedef double MLDecimal;
typedef MLVariable (*MLCode)(MLVariable, MLVariable, ...);

extern MLVariable const MLObject;
extern MLVariable const MLBoolean;
extern MLVariable const MLNumber;
extern MLVariable const MLBlock;
extern MLVariable const MLData;
extern MLVariable const MLArray;
extern MLVariable const MLString;
extern MLVariable const MLDictionary;
extern MLVariable const MLException;

extern MLVariable const MLNull;
extern MLVariable const MLYes;
extern MLVariable const MLNo;

MLVariable MLBooleanMake(long boolean);
MLVariable MLNumberMake(double number);
MLVariable MLBlockMake(void* code);
MLVariable MLDataMake(long count, const void* bytes);
MLVariable MLArrayMake(long count, ...);
MLVariable MLStringMake(long length, const char* characters);
MLVariable MLDictionaryMake(long count, ...);

MLInteger MLIntegerFrom(MLVariable number);
MLNatural MLNaturalFrom(MLVariable number);
MLDecimal MLDecimalFrom(MLVariable number);

void* MLCollectBlockPush();
void* MLCollectBlockPop(void* collectBlock);
MLVariable MLCollectBlockAdd(MLVariable object);

void* MLPerformHandleBlockPush();
void* MLPerformHandleBlockPop(void* performHandleBlock);
void* MLPerformHandleBlockPerform(void* performHandleBlock);
MLVariable MLPerformHandleBlockHandle(void* performHandleBlock);

MLVariable MLImport(const char* name);
MLVariable MLExport(const char* name, void* code);

void* MLLookup(MLVariable object, MLVariable command, MLVariable* super);
void MLRaise(MLVariable exception);
void MLLog(MLVariable object);

#endif
