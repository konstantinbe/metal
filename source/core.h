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

#ifndef CR_CORE_H
#define CR_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <alloca.h>
#include <limits.h>
#include <string.h>

// ------------------------------------------------------------ Constants ------

#define CRIntegerMin LONG_MIN
#define CRIntegerMax LONG_MAX

#define CRNaturalMin 0
#define CRNaturalMax ULONG_MAX

#define CRRetainCountMax 1073741823ul

// ---------------------------------------------------------------- Types ------

typedef double CRDecimal;
typedef long CRInteger;
typedef unsigned long CRNatural;

typedef unsigned char CRByte;
typedef char CRCharacter;

typedef bool CRBool;
typedef void* CRPointer;

typedef int32_t CRInteger32;
typedef uint32_t CRNatural32;

typedef int64_t CRInteger64;
typedef uint64_t CRNatural64;

typedef union CRPayload CRPayload;
typedef struct CRVariable CRVariable;
typedef CRVariable var;

typedef var (*CRImplementation)(var self, var command, var arguments);

typedef CRNatural64 (*CRHashCallback)(var object);
typedef bool (*CREqualsCallback)(var object1, var object2);

typedef var (*CRCopyCallback)(var object);
typedef var (*CRMutableCopyCallback)(var object);

typedef void (*CRDestroyCallback)(var object);
typedef var (*CRDescriptionCallback)(var object);

// ------------------------------------------------------------ Variables ------

union CRPayload {
    CRBool boolean;
    CRDecimal decimal;
    CRNatural64 natural;
    CRImplementation implementation;
    CRPointer pointer;
};

struct CRVariable {
    CRPointer pointer;
    CRPayload payload;
};

// --------------------------------------------------------- Declarations ------

struct CRCallbacks;
struct CRObject;
struct CRClass;

// ----------------------------------------------------------- Structures ------

struct CRObject {
    struct CRClass* class;
    CRNatural retain_count;
};

struct CRClass {
    struct CRClass* class;
    CRNatural retain_count;
    struct CRCallbacks* callbacks;
    var name;
    var class_methods;
    var instance_methods;
    var superclass;
    var subclasses;
};

struct CRCallbacks {
    CRHashCallback hash;
    CREqualsCallback equals;
    CRCopyCallback copy;
    CRMutableCopyCallback mutable_copy;
    CRDestroyCallback destroy;
    CRDescriptionCallback description;
};

// ------------------------------------------------------ Managing Memory ------

#define CRInline(number_of_bytes) alloca(number_of_bytes)
#define CRInlineAndClear(number_of_bytes) memset(alloca(number_of_bytes), 0, number_of_bytes)

#define CRAllocate(number_of_bytes) malloc(number_of_bytes)
#define CRAllocateAndClear(number_of_bytes) calloc(1, number_of_bytes)
#define CRFree(pointer) free(pointer)

#define CRResize(pointer, number_of_bytes) realloc(pointer, number_of_bytes)

// ------------------------------------------------------------ Functions ------

#define CRNumberOfVariables(...) (sizeof((var[]){null, __VA_ARGS__}) / sizeof(var) - 1)
#define CRPointerToVariables(...) ((var[]){null, __VA_ARGS__} + 1)

var CRRetain(var object);
CRNatural CRRetainCount(var object);
var CRRelease(var object);
var CRAutorelease(var object);

CRNatural64 CRHash(var object);
bool CREquals(var object1, var object2);

var CRCopy(var object);
var CRMutableCopy(var object);

void CRDestroy(var object);
var CRDescription(var object);

bool CRIsNull(var object);
bool CRIsInstanceOf(var object, var class);

// -----------------------------------------------------------------------------

#define null (var){0, {0}}
var CRReference(CRPointer pointer);

#define CRStructure(type, reference) (*((struct type*)reference.pointer))
#define CRArgument(number) (((struct CRArray*)arguments.pointer)->objects[number])

#define CRModule __attribute__((constructor)) static void CRModule()

#endif
