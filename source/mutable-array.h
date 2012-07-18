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
// THE SOFTWARE IS PROVvarED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef CR_MUTABLE_ARRAY_H
#define CR_MUTABLE_ARRAY_H

#include "core.h"
#include "array.h"

#define CRMutableArrayMinCapacity 32
extern const var CRMutableArray;

var CRArrayCreateMutable();
var CRArrayCreateMutableWithCapacity(CRNatural capacity);
var CRArrayCreateMutableWithCArray(CRNatural count, var* objects);

void CRArrayAdd(var self, var object);
void CRArrayAddMany(var self, var objects);

void CRArrayInsertAt(var self, var object, CRInteger index);
void CRArrayInsertManyAt(var self, var objects, CRInteger index);

void CRArrayRemove(var self, var object);
void CRArrayRemoveMany(var self, var objects);

void CRArrayRemoveAt(var self, CRInteger index);
void CRArrayRemoveManyAt(var self, var indexes);

void CRArrayClear(var self);
void CRArrayIncreaseCapacityToAtLeast(var self, CRNatural capacity);

#endif
