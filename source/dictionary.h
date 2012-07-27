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

#ifndef CR_DICTIONARY_H
#define CR_DICTIONARY_H

#include "core.h"
#include "object.h"

extern const var CRDictionary;

struct CRDictionary {
    struct CRClass* class;
    CRNatural retain_count;
    CRNatural mask;
    CRNatural count;
    var* entries;
};

#define CRDictionary(...) CRDictionaryMake(CRInline(sizeof(struct CRDictionary)), CRNumberOfVariables(__VA_ARGS__) / 2, CRPointerToVariables(__VA_ARGS__))
var CRDictionaryMake(struct CRDictionary* dictionary, CRNatural count, var* entries);

var CRDictionaryCreate();
var CRDictionaryCreateWithCArray(CRNatural count, var* entries);

var CRDictionaryCreateWithCArrayLinear(CRNatural count, var* entries);
var CRDictionaryCreateWithCArrayHashed(CRNatural count, var* entries);

CRNatural CRDictionaryMask(var self);
CRNatural CRDictionaryCapacity(var self);
CRNatural CRDictionaryCount(var self);
var* CRDictionaryEntries(var self);

bool CRDictionaryIsEmpty(var self);
bool CRDictionaryIsMutable(var self);
bool CRDictionaryIsLinear(var self);
bool CRDictionaryIsHashed(var self);

var CRDictionaryKeyAt(var self, CRInteger index);
var CRDictionaryObjectAt(var self, CRInteger index);

CRInteger CRDictionaryIndexOf(var self, var key);

var CRDictionaryGet(var self, var key);
var CRDictionaryGetMany(var self, var keys);

bool CRDictionaryContains(var self, var key);

#endif
