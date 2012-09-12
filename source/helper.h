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

#ifndef ML_HELPER_H
#define ML_HELPER_H

#include "metal.h"

#define MLHelperMax(value1, value2) ((value1) > (value2) ? (value1) : (value2))
#define MLHelperMin(value1, value2) ((value1) < (value2) ? (value1) : (value2))

#define MLHelperRotateLeft(value, count) ((value << count) | (value >> (sizeof(value) * 8 - count)))
#define MLHelperRotateRight(value, count) ((value >> count) | (value << (sizeof(value) * 8 - count)))

MLNatural MLHelperRoundUpToPowerOfTwo(MLNatural number);
MLNatural MLHelperRoundDownToPowerOfTwo(MLNatural number);

MLNatural MLHelperDigest(MLInteger count, const void* bytes);

var MLHelperCreateStringWithCharacters(const char* characters);
var MLHelperCreateArrayWithObjects(var objects[]);
var MLHelperCreateDictionaryWithMethods(MLPointer methods[]);

#endif
