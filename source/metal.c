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

#include "metal.h"
#include "helper.h"

#include "object.h"
#include "null.h"
#include "block.h"

#include "boolean.h"
#include "number.h"
#include "word.h"
#include "date.h"

#include "data.h"
#include "array.h"
#include "string.h"
#include "dictionary.h"

#include "pool.h"


static struct MLTryCatchBlock MLTryCatchBlockStack[1000];
static MLInteger MLTryCatchBlockStackCount = 0;


static struct MLClass MLObjectClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLNullClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLBlockClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLBooleanClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLNumberClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLWordClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLDateClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLDataClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLArrayClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLStringClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLDictionaryClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLInlineDataClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLInlineArrayClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLInlineStringClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLInlineDictionaryClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLMutableDataClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLMutableArrayClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLMutableStringClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLMutableDictionaryClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLPoolClass = {.retainCount = MLRetainCountMax};


static struct MLClass MLObjectMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLNullMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLBlockMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLBooleanMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLNumberMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLWordMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLDateMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLDataMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLArrayMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLStringMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLDictionaryMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLInlineDataMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLInlineArrayMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLInlineStringMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLInlineDictionaryMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLMutableDataMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLMutableArrayMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLMutableStringMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLMutableDictionaryMetaClass = {.retainCount = MLRetainCountMax};
static struct MLClass MLPoolMetaClass = {.retainCount = MLRetainCountMax};


static struct MLObject MLNullProxy = {&MLNullClass, MLRetainCountMax};
static struct MLObject MLBlockProxy = {&MLBlockClass, MLRetainCountMax};
static struct MLObject MLBooleanProxy = {&MLBooleanClass, MLRetainCountMax};
static struct MLObject MLNumberProxy = {&MLNumberClass, MLRetainCountMax};
static struct MLObject MLWordProxy = {&MLWordClass, MLRetainCountMax};
static struct MLObject MLDateProxy = {&MLDateClass, MLRetainCountMax};


const var MLObject = {&MLObjectClass};
const var MLNull = {&MLNullClass};
const var MLBlock = {&MLBlockClass};
const var MLBoolean = {&MLBooleanClass};
const var MLNumber = {&MLNumberClass};
const var MLWord = {&MLWordClass};
const var MLDate = {&MLDateClass};
const var MLData = {&MLDataClass};
const var MLArray = {&MLArrayClass};
const var MLString = {&MLStringClass};
const var MLDictionary = {&MLDictionaryClass};
const var MLInlineData = {&MLInlineDataClass};
const var MLInlineArray = {&MLInlineArrayClass};
const var MLInlineString = {&MLInlineStringClass};
const var MLInlineDictionary = {&MLInlineDictionaryClass};
const var MLMutableData = {&MLMutableDataClass};
const var MLMutableArray = {&MLMutableArrayClass};
const var MLMutableString = {&MLMutableStringClass};
const var MLMutableDictionary = {&MLMutableDictionaryClass};
const var MLPool = {&MLPoolClass};


const var MLObjectMeta = {&MLObjectMetaClass};
const var MLNullMeta = {&MLNullMetaClass};
const var MLBlockMeta = {&MLBlockMetaClass};
const var MLBooleanMeta = {&MLBooleanMetaClass};
const var MLNumberMeta = {&MLNumberMetaClass};
const var MLWordMeta = {&MLWordMetaClass};
const var MLDateMeta = {&MLDateMetaClass};
const var MLDataMeta = {&MLDataMetaClass};
const var MLArrayMeta = {&MLArrayMetaClass};
const var MLStringMeta = {&MLStringMetaClass};
const var MLDictionaryMeta = {&MLDictionaryMetaClass};
const var MLInlineDataMeta = {&MLInlineDataMetaClass};
const var MLInlineArrayMeta = {&MLInlineArrayMetaClass};
const var MLInlineStringMeta = {&MLInlineStringMetaClass};
const var MLInlineDictionaryMeta = {&MLInlineDictionaryMetaClass};
const var MLMutableDataMeta = {&MLMutableDataMetaClass};
const var MLMutableArrayMeta = {&MLMutableArrayMetaClass};
const var MLMutableStringMeta = {&MLMutableStringMetaClass};
const var MLMutableDictionaryMeta = {&MLMutableDictionaryMetaClass};
const var MLPoolMeta = {&MLPoolMetaClass};


const var null = {&MLNullProxy, {0}};
const var yes = {&MLBooleanProxy, {true}};
const var no = {&MLBooleanProxy, {false}};


var MLReference(MLPointer pointer) {
    var reference = null;
    reference.pointer = pointer;
    reference.payload.natural = 0;
    return reference;
}


MLBool MLIsObjectNull(var object) {
    if (object.pointer == &MLNullProxy) return true;
    return false;
}


MLBool MLIsObjectTruthy(var object) {
    if (object.pointer == &MLNullProxy) return false;
    if (MLObjectStructure(object).class == &MLBooleanClass) return object.payload.boolean;
    return true;
}


MLBool MLBoolFrom(var boolean) {
    if (boolean.pointer != &MLBooleanProxy) MLError("Can't convert boolean to bool, object is not a boolean");
    return (MLBool)boolean.payload.boolean;
}


MLInteger MLIntegerFrom(var number) {
    if (number.pointer != &MLNumberProxy) MLError("Can't convert number to integer, object is not a number");
    return (MLInteger)number.payload.decimal;
}


MLDecimal MLDecimalFrom(var number) {
    if (number.pointer != &MLNumberProxy) MLError("Can't convert number to decimal, object is not a number");
    return number.payload.decimal;
}


MLNatural MLNaturalFrom(var word) {
    if (word.pointer != &MLWordProxy) MLError("Can't convert word to natural, object is not a word");
    return word.payload.natural;
}


struct MLTryCatchBlock* MLTryCatchBlockStackPush() {
    const MLInteger index = MLTryCatchBlockStackCount;
    MLTryCatchBlockStackCount += 1;
    MLTryCatchBlockStack[index].executed = false;
    MLTryCatchBlockStack[index].exception = null;
    return &MLTryCatchBlockStack[index];
}


struct MLTryCatchBlock* MLTryCatchBlockStackTop() {
    if (MLTryCatchBlockStackCount <= 0) return NULL;
    return &MLTryCatchBlockStack[MLTryCatchBlockStackCount - 1];
}


struct MLTryCatchBlock* MLTryCatchBlockStackPop() {
    if (MLTryCatchBlockStackCount <= 0) MLError("Can't pop exception from try catch block stack, stack is empty");
    MLTryCatchBlockStackCount -= 1;
    return &MLTryCatchBlockStack[MLTryCatchBlockStackCount];
}


var MLBlockMake(MLCode code) {
    var block = null;
    block.pointer = &MLBlockProxy;
    block.payload.code = code;
    return block;
}


var MLBooleanMake(MLBool boolean) {
    return boolean ? yes : no;
}


var MLNumberMake(MLDecimal value) {
    var number = null;
    number.pointer = &MLNumberProxy;
    number.payload.decimal = value;
    return number;
}


var MLWordMake(MLNatural value) {
    var word = null;
    word.pointer = &MLWordProxy;
    word.payload.natural = value;
    return word;
}


var MLDateMake(MLDecimal seconds) {
    var date = null;
    date.pointer = &MLDateProxy;
    date.payload.decimal = seconds;
    return date;
}


var MLDataMake(struct MLData* data, var class, MLInteger retainCount, MLInteger capacity, MLInteger count, MLByte* bytes) {
    data->class = class.pointer;
    data->retainCount = retainCount;
    data->capacity = capacity;
    data->count = count;
    data->bytes = bytes;
    return MLReference(data);
}


var MLArrayMake(struct MLArray* array, var class, MLInteger retainCount, MLInteger capacity, MLInteger count, var* objects) {
    array->class = class.pointer;
    array->retainCount = retainCount;
    array->capacity = capacity;
    array->count = count;
    array->objects = objects;
    return MLReference(array);
}


var MLStringMake(struct MLString* string, var class, MLInteger retainCount, MLInteger capacity, MLInteger count, char* characters) {
    const MLNatural hash = MLHelperDigest(count, characters);
    string->class = class.pointer;
    string->retainCount = retainCount;
    string->capacity = capacity;
    string->count = count;
    string->characters = characters;
    var reference = MLReference(string);
    reference.payload.natural = hash;
    return reference;
}


var MLDictionaryMake(struct MLDictionary* dictionary, var class, MLInteger retainCount, MLInteger mask, MLInteger count, var* entries) {
    dictionary->class = class.pointer;
    dictionary->retainCount = retainCount;
    dictionary->mask = mask;
    dictionary->count = count;
    dictionary->entries = entries;
    return MLReference(dictionary);
}


var MLPoolMake(struct MLPool* pool, var class, MLInteger retainCount, var previousPool, var objects) {
    pool->class = class.pointer;
    pool->retainCount = retainCount;
    pool->previousPool = previousPool;
    pool->objects = objects;
    return MLReference(pool);
}


var MLRequire(const char* name) {
    // TODO: implement.
    return null;
}


var MLModule(const char* name) {
    // TODO: implement.
    return null;
}


var MLImport(var module, const char* name) {
    // TODO: implement.
    return null;
}


var MLExport(var module, const char* name, MLCode code) {
    // TODO: implement.
    return null;
}


var MLDefine(const char* name) {
    // TODO: implement.
    return null;
}


var MLExtends(var class, var superclass) {
    // TODO: implement.
    return null;
}


var MLResponds(var class, const char* command, MLCode code) {
    // TODO: implement.
    return null;
}


var MLLookup(var class, var command, var* foundInClass) {
    unless (class) return null;
    var methods = MLClassStructure(class).methods;

    const MLBool isCommandAString = !MLIsObjectNull(command) && MLObjectStructure(command).class == &MLStringClass;
    const MLBool isCommandAnInlineString = !MLIsObjectNull(command) && MLObjectStructure(command).class == &MLInlineStringClass;
    const MLBool isMethodsADictionary = !MLIsObjectNull(methods) && MLObjectStructure(methods).class == &MLDictionaryClass;
    const MLBool isMethodsAMutableDictionary = !MLIsObjectNull(methods) && MLObjectStructure(methods).class == &MLMutableDictionaryClass;

    MLAssert(isCommandAString || isCommandAnInlineString, "Command should be a string");
    MLAssert(isMethodsADictionary || isMethodsAMutableDictionary, "Methods should be a dictionary");

    const MLNatural mask = MLDictionaryStructure(methods).mask;
    const MLNatural count = MLDictionaryStructure(methods).count;
    var* entries = MLDictionaryStructure(methods).entries;
    *foundInClass = null;

    for (MLNatural index = 0; index < 2 * MLHelperMax(mask + 1, count); index += 2) {
        var key = entries[index];
        var value = entries[index + 1];

        if (!MLIsObjectNull(key)) {
            const MLBool isKeyAString = !MLIsObjectNull(key) && MLObjectStructure(key).class == &MLStringClass;
            const MLBool isValueABlock = !MLIsObjectNull(value) && MLObjectStructure(value).class == &MLBlockClass;

            MLAssert(isKeyAString, "Key should be a string");
            MLAssert(isValueABlock, "Value should be a block");

            const MLBool found = strcmp(MLStringStructure(key).characters, MLStringStructure(command).characters) == 0;
            if (found) {
                *foundInClass = class;
                return value;
            }
        }
    }

    // TODO: optimize by converting recursive call of MLLookup to an iterative
    // version.
    var superclass = MLClassStructure(class).superclass;
    return MLLookup(superclass, command, foundInClass);
}


var MLDispatch(var context, var self, var command, var arguments, var options) {
    unless (context) context = (var){.pointer = MLObjectStructure(self).class, .payload.natural = 0};

    // TODO: implement fast cache lookup.

    var foundInClass = null;

    var method = MLLookup(context, command, &foundInClass);
    when (method) {
        // TODO: cache this method and its context.
        return method.payload.code(foundInClass, self, command, arguments, options);
    }

    var fallback = MLLookup(context, IS("perform*arguments*options*"), &foundInClass);
    when (fallback) {
        // TODO: cache this method and its context.
        return fallback.payload.code(foundInClass, self, IS("perform*arguments*options*"), IA(command, arguments, options), null);
    }

    MLError("Object must respond to '-perform*arguments*options*'");
    return null;
}


static MLLoadWithPriority(101) MLMetal() {
    MLObjectClass.class = &MLObjectMetaClass;
    MLNullClass.class = &MLNullMetaClass;
    MLBlockClass.class = &MLBlockMetaClass;
    MLBooleanClass.class = &MLBooleanMetaClass;
    MLNumberClass.class = &MLNumberMetaClass;
    MLWordClass.class = &MLWordMetaClass;
    MLDateClass.class = &MLDateMetaClass;
    MLDataClass.class = &MLDataMetaClass;
    MLArrayClass.class = &MLArrayMetaClass;
    MLStringClass.class = &MLStringMetaClass;
    MLDictionaryClass.class = &MLDictionaryMetaClass;
    MLInlineDataClass.class = &MLInlineDataMetaClass;
    MLInlineArrayClass.class = &MLInlineArrayMetaClass;
    MLInlineStringClass.class = &MLInlineStringMetaClass;
    MLInlineDictionaryClass.class = &MLInlineDictionaryMetaClass;
    MLMutableDataClass.class = &MLMutableDataMetaClass;
    MLMutableArrayClass.class = &MLMutableArrayMetaClass;
    MLMutableStringClass.class = &MLMutableStringMetaClass;
    MLMutableDictionaryClass.class = &MLMutableDictionaryMetaClass;
    MLPoolClass.class = &MLPoolMetaClass;

    MLObjectMetaClass.class = &MLObjectMetaClass;
    MLNullMetaClass.class = &MLNullMetaClass;
    MLBlockMetaClass.class = &MLBlockMetaClass;
    MLBooleanMetaClass.class = &MLBooleanMetaClass;
    MLNumberMetaClass.class = &MLNumberMetaClass;
    MLWordMetaClass.class = &MLWordMetaClass;
    MLDateMetaClass.class = &MLDateMetaClass;
    MLDataMetaClass.class = &MLDataMetaClass;
    MLArrayMetaClass.class = &MLArrayMetaClass;
    MLStringMetaClass.class = &MLStringMetaClass;
    MLDictionaryMetaClass.class = &MLDictionaryMetaClass;
    MLInlineDataMetaClass.class = &MLInlineDataMetaClass;
    MLInlineArrayMetaClass.class = &MLInlineArrayMetaClass;
    MLInlineStringMetaClass.class = &MLInlineStringMetaClass;
    MLInlineDictionaryMetaClass.class = &MLInlineDictionaryMetaClass;
    MLMutableDataMetaClass.class = &MLMutableDataMetaClass;
    MLMutableArrayMetaClass.class = &MLMutableArrayMetaClass;
    MLMutableStringMetaClass.class = &MLMutableStringMetaClass;
    MLMutableDictionaryMetaClass.class = &MLMutableDictionaryMetaClass;
    MLPoolMetaClass.class = &MLPoolMetaClass;

    MLObjectClass.superclass = null;
    MLNullClass.superclass = MLObject;
    MLBlockClass.superclass = MLObject;
    MLBooleanClass.superclass = MLObject;
    MLNumberClass.superclass = MLObject;
    MLWordClass.superclass = MLObject;
    MLDateClass.superclass = MLObject;
    MLDataClass.superclass = MLObject;
    MLArrayClass.superclass = MLObject;
    MLStringClass.superclass = MLObject;
    MLDictionaryClass.superclass = MLObject;
    MLInlineDataClass.superclass = MLData;
    MLInlineArrayClass.superclass = MLArray;
    MLInlineStringClass.superclass = MLString;
    MLInlineDictionaryClass.superclass = MLDictionary;
    MLMutableDataClass.superclass = MLData;
    MLMutableArrayClass.superclass = MLArray;
    MLMutableStringClass.superclass = MLString;
    MLMutableDictionaryClass.superclass = MLDictionary;
    MLPoolClass.superclass = MLObject;

    MLObjectMetaClass.superclass = MLObject;
    MLNullMetaClass.superclass = MLObjectMeta;
    MLBlockMetaClass.superclass = MLObjectMeta;
    MLBooleanMetaClass.superclass = MLObjectMeta;
    MLNumberMetaClass.superclass = MLObjectMeta;
    MLWordMetaClass.superclass = MLObjectMeta;
    MLDateMetaClass.superclass = MLObjectMeta;
    MLDataMetaClass.superclass = MLObjectMeta;
    MLArrayMetaClass.superclass = MLObjectMeta;
    MLStringMetaClass.superclass = MLObjectMeta;
    MLDictionaryMetaClass.superclass = MLObjectMeta;
    MLInlineDataMetaClass.superclass = MLDataMeta;
    MLInlineArrayMetaClass.superclass = MLArrayMeta;
    MLInlineStringMetaClass.superclass = MLStringMeta;
    MLInlineDictionaryMetaClass.superclass = MLDictionaryMeta;
    MLMutableDataMetaClass.superclass = MLDataMeta;
    MLMutableArrayMetaClass.superclass = MLArrayMeta;
    MLMutableStringMetaClass.superclass = MLStringMeta;
    MLMutableDictionaryMetaClass.superclass = MLDictionaryMeta;
    MLPoolMetaClass.superclass = MLObjectMeta;

    MLObjectClass.name = MLHelperCreateStringWithCharacters("Object");
    MLNullClass.name = MLHelperCreateStringWithCharacters("Null");
    MLBlockClass.name = MLHelperCreateStringWithCharacters("Method");
    MLBooleanClass.name = MLHelperCreateStringWithCharacters("Boolean");
    MLNumberClass.name = MLHelperCreateStringWithCharacters("Number");
    MLWordClass.name = MLHelperCreateStringWithCharacters("Word");
    MLDateClass.name = MLHelperCreateStringWithCharacters("Date");
    MLDataClass.name = MLHelperCreateStringWithCharacters("Data");
    MLArrayClass.name = MLHelperCreateStringWithCharacters("Array");
    MLStringClass.name = MLHelperCreateStringWithCharacters("String");
    MLDictionaryClass.name = MLHelperCreateStringWithCharacters("Dictionary");
    MLInlineDataClass.name = MLHelperCreateStringWithCharacters("InlineData");
    MLInlineArrayClass.name = MLHelperCreateStringWithCharacters("InlineArray");
    MLInlineStringClass.name = MLHelperCreateStringWithCharacters("InlineString");
    MLInlineDictionaryClass.name = MLHelperCreateStringWithCharacters("InlineDictionary");
    MLMutableDataClass.name = MLHelperCreateStringWithCharacters("MutableData");
    MLMutableArrayClass.name = MLHelperCreateStringWithCharacters("MutableArray");
    MLMutableStringClass.name = MLHelperCreateStringWithCharacters("MutableString");
    MLMutableDictionaryClass.name = MLHelperCreateStringWithCharacters("MutableDictionary");
    MLPoolClass.name = MLHelperCreateStringWithCharacters("Pool");

    MLObjectMetaClass.name = MLHelperCreateStringWithCharacters("ObjectMeta");
    MLNullMetaClass.name = MLHelperCreateStringWithCharacters("NullMeta");
    MLBlockMetaClass.name = MLHelperCreateStringWithCharacters("MethodMeta");
    MLBooleanMetaClass.name = MLHelperCreateStringWithCharacters("BooleanMeta");
    MLNumberMetaClass.name = MLHelperCreateStringWithCharacters("NumberMeta");
    MLWordMetaClass.name = MLHelperCreateStringWithCharacters("WordMeta");
    MLDateMetaClass.name = MLHelperCreateStringWithCharacters("DateMeta");
    MLDataMetaClass.name = MLHelperCreateStringWithCharacters("DataMeta");
    MLArrayMetaClass.name = MLHelperCreateStringWithCharacters("ArrayMeta");
    MLStringMetaClass.name = MLHelperCreateStringWithCharacters("StringMeta");
    MLDictionaryMetaClass.name = MLHelperCreateStringWithCharacters("DictionaryMeta");
    MLInlineDataMetaClass.name = MLHelperCreateStringWithCharacters("InlineDataMeta");
    MLInlineArrayMetaClass.name = MLHelperCreateStringWithCharacters("InlineArrayMeta");
    MLInlineStringMetaClass.name = MLHelperCreateStringWithCharacters("InlineStringMeta");
    MLInlineDictionaryMetaClass.name = MLHelperCreateStringWithCharacters("InlineDictionaryMeta");
    MLMutableDataMetaClass.name = MLHelperCreateStringWithCharacters("MutableDataMeta");
    MLMutableArrayMetaClass.name = MLHelperCreateStringWithCharacters("MutableArrayMeta");
    MLMutableStringMetaClass.name = MLHelperCreateStringWithCharacters("MutableStringMeta");
    MLMutableDictionaryMetaClass.name = MLHelperCreateStringWithCharacters("MutableDictionaryMeta");
    MLPoolMetaClass.name = MLHelperCreateStringWithCharacters("PoolMeta");

    MLObjectClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLObjectMeta, MLNull, MLBlock, MLBoolean, MLNumber, MLWord, MLDate, MLData, MLArray, MLString, MLDictionary, MLPool, null));
    MLNullClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLBlockClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLBooleanClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLNumberClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLWordClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLDateClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLDataClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLInlineData, MLMutableData, null));
    MLArrayClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLInlineArray, MLMutableArray, null));
    MLStringClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLInlineString, MLMutableString, null));
    MLDictionaryClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLInlineDictionary, MLMutableDictionary, null));
    MLInlineDataClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLInlineArrayClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLInlineStringClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLInlineDictionaryClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLMutableDataClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLMutableArrayClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLMutableStringClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLMutableDictionaryClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLPoolClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));

    MLObjectMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLNullMeta, MLBlockMeta, MLBooleanMeta, MLNumberMeta, MLWordMeta, MLDateMeta, MLDataMeta, MLArrayMeta, MLStringMeta, MLDictionaryMeta, MLPoolMeta, null));
    MLNullMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLBlockMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLBooleanMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLNumberMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLWordMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLDateMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLDataMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLInlineDataMeta, MLMutableDataMeta, null));
    MLArrayMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLInlineArrayMeta, MLMutableArrayMeta, null));
    MLStringMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLInlineStringMeta, MLMutableStringMeta, null));
    MLDictionaryMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(MLInlineDictionaryMeta, MLMutableDictionaryMeta, null));
    MLInlineDataMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLInlineArrayMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLInlineStringMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLInlineDictionaryMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLMutableDataMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLMutableArrayMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLMutableStringMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLMutableDictionaryMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));
    MLPoolMetaClass.subclasses = MLHelperCreateArrayWithObjects(MLPointerToVariables(null));

    MLObjectClass.methods = MLHelperCreateDictionaryWithMethods(MLObjectDefaultMethods);
    MLNullClass.methods = MLHelperCreateDictionaryWithMethods(MLNullDefaultMethods);
    MLBlockClass.methods = MLHelperCreateDictionaryWithMethods(MLBlockDefaultMethods);
    MLBooleanClass.methods = MLHelperCreateDictionaryWithMethods(MLBooleanDefaultMethods);
    MLNumberClass.methods = MLHelperCreateDictionaryWithMethods(MLNumberDefaultMethods);
    MLWordClass.methods = MLHelperCreateDictionaryWithMethods(MLWordDefaultMethods);
    MLDateClass.methods = MLHelperCreateDictionaryWithMethods(MLDateDefaultMethods);
    MLDataClass.methods = MLHelperCreateDictionaryWithMethods(MLDataDefaultMethods);
    MLArrayClass.methods = MLHelperCreateDictionaryWithMethods(MLArrayDefaultMethods);
    MLStringClass.methods = MLHelperCreateDictionaryWithMethods(MLStringDefaultMethods);
    MLDictionaryClass.methods = MLHelperCreateDictionaryWithMethods(MLDictionaryDefaultMethods);
    MLInlineDataClass.methods = MLHelperCreateDictionaryWithMethods(MLInlineDataDefaultMethods);
    MLInlineArrayClass.methods = MLHelperCreateDictionaryWithMethods(MLInlineArrayDefaultMethods);
    MLInlineStringClass.methods = MLHelperCreateDictionaryWithMethods(MLInlineStringDefaultMethods);
    MLInlineDictionaryClass.methods = MLHelperCreateDictionaryWithMethods(MLInlineDictionaryDefaultMethods);
    MLMutableDataClass.methods = MLHelperCreateDictionaryWithMethods(MLMutableDataDefaultMethods);
    MLMutableArrayClass.methods = MLHelperCreateDictionaryWithMethods(MLMutableArrayDefaultMethods);
    MLMutableStringClass.methods = MLHelperCreateDictionaryWithMethods(MLMutableStringDefaultMethods);
    MLMutableDictionaryClass.methods = MLHelperCreateDictionaryWithMethods(MLMutableDictionaryDefaultMethods);
    MLPoolClass.methods = MLHelperCreateDictionaryWithMethods(MLPoolDefaultMethods);

    MLObjectMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLObjectMetaDefaultMethods);
    MLNullMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLNullMetaDefaultMethods);
    MLBlockMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLBlockMetaDefaultMethods);
    MLBooleanMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLBooleanMetaDefaultMethods);
    MLNumberMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLNumberMetaDefaultMethods);
    MLWordMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLWordMetaDefaultMethods);
    MLDateMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLDateMetaDefaultMethods);
    MLDataMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLDataMetaDefaultMethods);
    MLArrayMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLArrayMetaDefaultMethods);
    MLStringMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLStringMetaDefaultMethods);
    MLDictionaryMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLDictionaryMetaDefaultMethods);
    MLInlineDataMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLInlineDataMetaDefaultMethods);
    MLInlineArrayMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLInlineArrayMetaDefaultMethods);
    MLInlineStringMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLInlineStringMetaDefaultMethods);
    MLInlineDictionaryMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLInlineDictionaryMetaDefaultMethods);
    MLMutableDataMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLMutableDataMetaDefaultMethods);
    MLMutableArrayMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLMutableArrayMetaDefaultMethods);
    MLMutableStringMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLMutableStringMetaDefaultMethods);
    MLMutableDictionaryMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLMutableDictionaryMetaDefaultMethods);
    MLPoolMetaClass.methods = MLHelperCreateDictionaryWithMethods(MLPoolMetaDefaultMethods);
}
