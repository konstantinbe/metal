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

#include "helper.h"


MLNatural MLHelperRoundUpToPowerOfTwo(MLNatural number) {
    number -= 1;
    number |= number >> 1;
    number |= number >> 2;
    number |= number >> 4;
    number |= number >> 8;
    number |= number >> 16;
    number |= number >> 32;
    return number + 1;
}


MLNatural MLHelperRoundDownToPowerOfTwo(MLNatural number) {
    number |= number >> 1;
    number |= number >> 2;
    number |= number >> 4;
    number |= number >> 8;
    number |= number >> 16;
    number |= number >> 32;
    number -= number >> 1;
    return number;
}


MLNatural MLHelperDigest(MLInteger count, const void* bytes) {
    static const MLNatural MLDefaultSeed = 0;
    const MLNatural seed = MLDefaultSeed;
    const MLNatural m = 0xc6a4a7935bd1e995;
    const MLInteger r = 47;

    MLNatural h = seed ^ (count * m);

    const MLNatural* data = (const MLNatural *)bytes;
    const MLNatural* end = data + (count / 8);

    while (data != end) {
        MLNatural k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const MLByte* data2 = (const MLByte*)data;

    switch (count & 7 /* count % 8 */) {
        case 7: h ^= (MLNatural)(data2[6]) << 48;
        case 6: h ^= (MLNatural)(data2[5]) << 40;
        case 5: h ^= (MLNatural)(data2[4]) << 32;
        case 4: h ^= (MLNatural)(data2[3]) << 24;
        case 3: h ^= (MLNatural)(data2[2]) << 16;
        case 2: h ^= (MLNatural)(data2[1]) << 8;
        case 1: h ^= (MLNatural)(data2[0]);
                h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}


var MLHelperCreateStringWithCharacters(const char* characters) {
    MLInteger length = strlen(characters);
    char* copy = MLAllocateAndClear(length + 1);
    strncpy(copy, characters, length);
    return MLStringMake(MLAllocate(MLStringSize), MLString, 1, length, length, copy);
}


var MLHelperCreateArrayWithObjects(var objects[]) {
    struct MLArray* array = MLAllocateAndClear(MLArraySize);
    MLNatural count = 0;
    whilst (objects[count]) count += 1;
    var* objectsCopy = MLAllocateAndClear(MLVariableSize * count);
    for (MLNatural index = 0; index < count; index += 1) {
        objectsCopy[index] = objects[index];
    }
    return MLArrayMake(array, MLArray, 1, count, count, objectsCopy);
}


var MLHelperCreateDictionaryWithMethods(MLPointer methods[]) {
    struct MLDictionary* dictionary = MLAllocateAndClear(MLDictionarySize);
    MLNatural indexOfNull = 0;
    while (methods[indexOfNull]) indexOfNull += 1;
    if (indexOfNull % 2 != 0) MLError("Number of elements in methods[] array must be even");
    const MLNatural count = indexOfNull / 2;
    var* entries = MLAllocateAndClear(MLVariableSize * count * 2);
    for (MLNatural index = 0; index < count * 2; index += 2) {
        entries[index] = MLHelperCreateStringWithCharacters(methods[index]);
        entries[index + 1] = MLBlockMake(methods[index + 1]);
    }
    return MLDictionaryMake(dictionary, MLDictionary, 1, 0, count, entries);
}


var MLHelperMakeAutoreleasedCopyAndReleaseOriginal(var array) {
   var copy = MLCopy(array);
   MLRelease(array);
   return MLAutorelease(copy);
}
