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

#ifndef CR_ARRAY_H
#define CR_ARRAY_H

#include "core.h"
#include "object.h"

extern const var CRArray;

struct CRArray {
    struct CRClass* class;
    CRNatural retain_count;
    CRNatural capacity;
    CRNatural count;
    var* objects;
};

#define CRArray(...) CRArrayMake(CRInline(sizeof(struct CRArray)), CRNumberOfVariables(__VA_ARGS__), CRPointerToVariables(__VA_ARGS__))
var CRArrayMake(struct CRArray* array, CRNatural count, var* objects);

var CRArrayCreate();
var CRArrayCreateWithCArray(CRNatural count, var* objects);

CRNatural CRArrayCapacity(var self);
CRNatural CRArrayCount(var self);
var* CRArrayObjects(var self);

bool CRArrayIsEmpty(var self);
bool CRArrayIsFull(var self);
bool CRArrayIsMutable(var self);

var CRArrayObjectAt(var self, CRInteger index);
CRInteger CRArrayIndexOf(var self, var object);
bool CRArrayContains(var self, var object);

#endif
