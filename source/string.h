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

#ifndef CR_STRING_H
#define CR_STRING_H

#include "core.h"
#include "object.h"

extern const var CRString;

struct CRString {
    struct CRClass* class;
    CRNatural retain_count;
    CRNatural capacity;
    CRNatural length;
    char* characters;
};

#define CRString(string) CRStringMake(CRInline(sizeof(struct CRString)), string)
var CRStringMake(struct CRString* string, const CRCharacter* characters);

var CRStringCreate();
var CRStringCreateWithCharacters(const CRCharacter* characters);

CRNatural CRStringCapacity(var self);
CRNatural CRStringLength(var self);
CRCharacter* CRStringCharacters(var self);

bool CRStringIsEmpty(var self);
bool CRStringIsFull(var self);
bool CRStringIsMutable(var self);

CRCharacter CRStringCharacterAt(var self, CRInteger index);
CRInteger CRStringIndexOf(var self, var string);

bool CRStringContains(var self, var string);
bool CRStringBeginsWith(var self, var string);
bool CRStringEndsWith(var self, var string);

#endif
