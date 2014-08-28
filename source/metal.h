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

#ifndef METAL_H
#define METAL_H

#include <float.h>
#include <setjmp.h>
#include <limits.h>
#include <stdbool.h>

#define ZERO (void*)0ul
#define MORE (void*)ULLONG_MAX

#define zero (var)0ul
#define more (var)ULLONG_MAX

#define metalHelperJoinJoin(x, y) x ## y
#define metalHelperJoin(x, y) metalHelperJoinJoin(x, y)
#define metalHelperStringify(x) (((char*)(#x))[0] == '"' ? String(x) : (x))

#define load __attribute__((constructor(255))) static void metalHelperJoin(__MetalLoadBlock, __COUNTER__)()

#define collect for (void* collectBlock = CollectBlockPush(); collectBlock != ZERO; collectBlock = CollectBlockPop(collectBlock))
#define try for (void* tryCatchBlock = TryCatchBlockPush(); tryCatchBlock != ZERO; tryCatchBlock = TryCatchBlockPop(tryCatchBlock)) if (!setjmp(TryCatchBlockTry(tryCatchBlock)))
#define catch else for (var exception = TryCatchBlockCatch(tryCatchBlock); exception != null; exception = null)

#define send(self, command, ...) ({var const selfToSend = (self); var const commandToSend = metalHelperStringify(command); var superToSend = zero; Code const codeToCall = lookup(self, commandToSend, &superToSend); codeToCall(selfToSend, superToSend, commandToSend, ## __VA_ARGS__, zero);})
#define super(self, command, ...) ({var const selfToSend = (self); var const commandToSend = metalHelperStringify(command); var superToSend = zero; Code const codeToCall = lookup(super, commandToSend, &superToSend); codeToCall(selfToSend, superToSend, commandToSend, ## __VA_ARGS__, zero);})

#define option(name, initial) ({ var nameAsString = metalHelperStringify(name); va_list list; va_start(list, options); var key = options; var value = zero; while (key != zero && key != (nameAsString)) { value = va_arg(list, var); key = va_arg(list, var); } va_end(list); key ? va_arg(list, var) : (initial); });
#define options(...) __VA_ARGS__

#define Boolean(boolean) ((boolean) ? yes : no)
#define Number(number) CollectBlockAdd(NUMBER(number))
#define Block(code) CollectBlockAdd(BLOCK(code))
#define Data(data) CollectBlockAdd(DATA(data))
#define Array(...) CollectBlockAdd(ARRAY(__VA_ARGS__))
#define String(string) CollectBlockAdd(STRING(string))
#define Dictionary(...) CollectBlockAdd(DICTIONARY(__VA_ARGS__))

#define NUMBER(number) NumberMake((decimal)(number))
#define BLOCK(code) BlockMake((void*)(code))
#define DATA(data) DataMake(sizeof(data), (void*)(data))
#define ARRAY(...) ArrayMake((sizeof((var[]){zero, ## __VA_ARGS__}) / sizeof(var)) - 1, ## __VA_ARGS__, zero)
#define STRING(string) StringMake(sizeof(string), (string))
#define DICTIONARY(...) DictionaryMake((sizeof((var[]){zero, ## __VA_ARGS__}) / sizeof(var)) - 1, ## __VA_ARGS__, zero)

#define INTEGER_MAX ((integer)LONG_MAX)
#define INTEGER_MIN ((integer)LONG_MIN)

#define NATURAL_MAX ((natural)ULONG_MAX)
#define NATURAL_MIN ((natural)0ul)

#define DECIMAL_MAX ((decimal)DBL_MAX)
#define DECIMAL_MIN ((decimal)DBL_MIN)

typedef void* var;
typedef long integer;
typedef unsigned long natural;
typedef double decimal;

typedef var (*Code)(var, var, ...);

extern var const Object;
extern var const Boolean;
extern var const Number;
extern var const Block;
extern var const Data;
extern var const Array;
extern var const String;
extern var const Dictionary;

extern var const null;
extern var const yes;
extern var const no;

var BooleanMake(long boolean);
var NumberMake(double number);
var BlockMake(void* code);
var DataMake(long count, const void* bytes);
var ArrayMake(long count, ...);
var StringMake(long length, const char* characters);
var DictionaryMake(long count, ...);

integer IntegerFrom(var number);
natural NaturalFrom(var number);
decimal DecimalFrom(var number);

void* CollectBlockPush();
void* CollectBlockPop(void* collectBlock);
var CollectBlockAdd(var object);

void* TryCatchBlockPush();
void* TryCatchBlockPop(void* tryCatchBlock);
void* TryCatchBlockTry(void* tryCatchBlock);
var TryCatchBlockCatch(void* tryCatchBlock);

var import(const char* name);
var export(const char* name, void* code);
void* lookup(var object, var command, var* super);
void throw(var exception);
void inspect(var object);

#endif
