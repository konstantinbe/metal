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
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>


// --------------------------------------------------- Constants & Macros ------


#define RETAIN_COUNT_MAX LONG_MAX
#define DATA_DEFAULT_CAPACITY 16
#define ARRAY_DEFAULT_CAPACITY 16
#define STRING_DEFAULT_CAPACITY 15 // +1 for zero
#define DICTIONARY_DEFAULT_CAPACITY 8
#define CACHE_DEFAULT_CAPACITY 32
#define CHILDREN_DEFAULT_CAPACITY 8
#define METHODS_DEFAULT_CAPACITY 8
#define COLLECT_BLOCK_DEFAULT_CAPACITY 2048
#define SYMBOL_TABLE_DEFAULT_CAPACITY 2048
#define STRING_TABLE_DEFAULT_CAPACITY 2048
#define MAX_KEY_AND_COMMAND_LENGTH 1024


#define MAX(value1, value2) (value1 > value2 ? value1 : value2)
#define MIN(value1, value2) (value1 < value2 ? value1 : value2)


#define table(pointer) (*((struct Table*)pointer))
#define behavior(pointer) (*((struct Behavior*)pointer))
#define object(pointer) (*((struct Object*)pointer))
#define boolean(pointer) (*((struct Boolean*)pointer))
#define number(pointer) (*((struct Number*)pointer))
#define block(pointer) (*((struct Block*)pointer))
#define data(pointer) (*((struct Data*)pointer))
#define array(pointer) (*((struct Array*)pointer))
#define string(pointer) (*((struct String*)pointer))
#define dictionary(pointer) (*((struct Dictionary*)pointer))
#define bootstrap __attribute__((constructor(128)))


// ----------------------------------------------------------- Structures ------


struct Table {
    natural mask;
    integer count;
    void** entries;
};


struct Behavior {
    var owner;
    var parent;
    struct Table cache;
    struct Table methods;
    struct Table children;
};


struct Object {
    struct Behavior* behavior;
    integer retainCount;
};


struct Boolean {
    struct Behavior* behavior;
    integer retainCount;
};


struct Number {
    struct Behavior* behavior;
    integer retainCount;
    double number;
};


struct Block {
    struct Behavior* behavior;
    integer retainCount;
    void* code;
};


struct Data {
    struct Behavior* behavior;
    integer retainCount;
    integer capacity;
    integer count;
    natural hash;
    void* bytes;
};


struct Array {
    struct Behavior* behavior;
    integer retainCount;
    integer capacity;
    integer count;
    natural hash;
    var* objects;
};


struct String {
    struct Behavior* behavior;
    integer retainCount;
    integer capacity;
    integer length;
    natural hash;
    char* characters;
};


struct Dictionary {
    struct Behavior* behavior;
    integer retainCount;
    integer capacity;
    integer count;
    natural mask;
    natural hash;
    var* entries;
};


struct CollectBlock {
    struct CollectBlock* previousCollectBlock;
    integer capacity;
    integer count;
    var* objects;
};


struct TryCatchBlock {
    struct TryCatchBlock* previousTryCatchBlock;
    jmp_buf destination;
    var exception;
    bool thrown;
};


// ------------------------------------------------------------ Variables ------


static struct Behavior ObjectBehavior;
static struct Behavior BooleanBehavior;
static struct Behavior NumberBehavior;
static struct Behavior BlockBehavior;
static struct Behavior DataBehavior;
static struct Behavior ArrayBehavior;
static struct Behavior StringBehavior;
static struct Behavior DictionaryBehavior;
static struct Behavior NullBehavior;


static struct Object ObjectState = {&ObjectBehavior, RETAIN_COUNT_MAX};
static struct Boolean BooleanState = {&BooleanBehavior, RETAIN_COUNT_MAX};
static struct Number NumberState = {&NumberBehavior, RETAIN_COUNT_MAX};
static struct Block BlockState = {&BlockBehavior, RETAIN_COUNT_MAX};
static struct Data DataState = {&DataBehavior, RETAIN_COUNT_MAX};
static struct Array ArrayState = {&ArrayBehavior, RETAIN_COUNT_MAX};
static struct String StringState = {&StringBehavior, RETAIN_COUNT_MAX};
static struct Dictionary DictionaryState = {&DictionaryBehavior, RETAIN_COUNT_MAX};
static struct Object NullState = {&NullBehavior, RETAIN_COUNT_MAX};
static struct Boolean yesState = {&BooleanBehavior, RETAIN_COUNT_MAX};
static struct Boolean noState = {&BooleanBehavior, RETAIN_COUNT_MAX};


var const Object = &ObjectState;
var const Boolean = &BooleanState;
var const Number = &NumberState;
var const Block = &BlockState;
var const Data = &DataState;
var const Array = &ArrayState;
var const String = &StringState;
var const Dictionary = &DictionaryState;
var const null = &NullState;
var const yes = &yesState;
var const no = &noState;


static struct CollectBlock* CollectBlockTop = ZERO;
static struct TryCatchBlock* TryCatchBlockTop = ZERO;
static struct Table StringTable = {0, 0, ZERO};


static struct String* ObjectName = ZERO;
static struct String* BooleanName = ZERO;
static struct String* NumberName = ZERO;
static struct String* BlockName = ZERO;
static struct String* DataName = ZERO;
static struct String* ArrayName = ZERO;
static struct String* StringName = ZERO;
static struct String* DictionaryName = ZERO;
static struct String* NullName = ZERO;

static struct String* noAsString = ZERO;
static struct String* yesAsString = ZERO;

static struct String* GenericException = ZERO;
static struct String* OutOfBoundsException = ZERO;
static struct String* InvalidArgumentException = ZERO;
static struct String* CommandNotAllowedException = ZERO;
static struct String* InternalInconsistencyException = ZERO;
static struct String* ErrorOccuredException = ZERO;


// ---------------------------------------------------- Helper Functions -------


static void DataEnsureCapacity(struct Data* data, integer requiredCapacity);
static void ArrayEnsureCapacity(struct Array* array, integer requiredCapacity);
static void StringEnsureCapacity(struct String* string, integer requiredCapacity);
static void DictionaryEnsureCapacity(struct Dictionary* dictionary, integer requiredCapacity);
static inline natural RoundUpToPowerOfTwo(natural number);
static inline natural RoundDownToPowerOfTwo(natural number);
static natural Digest(integer count, const void* bytes);
static natural ComputeHashOfString(void* string);
static bool CheckIfStringsAreEqual(void* string1, void* string2);


// ----------------------------------------------------- Table Functions -------


static void Init(struct Table* table, integer capacity) {
    assert(table->entries == ZERO);
    capacity = RoundUpToPowerOfTwo(capacity);
    if (capacity < 1) capacity = 1;
    table->mask = capacity - 1;
    table->count = 0;
    table->entries = calloc(capacity * 2, sizeof(void*));
}


static void Clear(struct Table* table) {
    integer const capacity = table->mask + 1;
    memset(table->entries, 0, sizeof(void*) * 2 * capacity);
    table->count = 0;
}


static void* Get(struct Table* table, void* key, natural (*hash)(void*), bool (*equals)(void*, void*)) {
    assert(key > ZERO);
    assert(key < MORE);

    natural const mask = table->mask;
    void** const entries = table->entries;

    integer index = hash ? hash(key) & mask : (natural)key & mask;
    for (int i = 0; i <= mask; i += 1) {
        void* const keyAtIndex = entries[index * 2];
        void* const valueAtIndex = entries[index * 2 + 1];
        if (keyAtIndex == ZERO) return ZERO;
        if (equals ? equals(keyAtIndex, key) : keyAtIndex == key) return valueAtIndex;
        index = (index + 1) & mask;
    }

    return ZERO;
}


static integer Put(struct Table* table, void* key, void* value, natural (*hash)(void*), bool (*equals)(void*, void*)) {
    assert(key > ZERO);
    assert(key < MORE);
    assert(value < MORE);

    natural const mask = table->mask;
    integer const count = table->count;

    if (value > 0) {
        integer const oldCapacity = mask + 1;
        integer const requiredCapacity = count + 1;
        integer const oneFourthOfOldCapacity = oldCapacity >> 2;
        integer const threeFourthOfOldCapacity = oldCapacity - oneFourthOfOldCapacity;
        integer const newCapacity = RoundUpToPowerOfTwo(requiredCapacity + (requiredCapacity >> 1));

        if (requiredCapacity >= threeFourthOfOldCapacity) {
            void** const oldEntries = table->entries;
            void** const newEntries = calloc(sizeof(void*), 2 * newCapacity);

            table->count = 0;
            table->mask = newCapacity - 1;
            table->entries = newEntries;

            for (int index = 0; index < oldCapacity; index += 1) {
                void* const key = oldEntries[index * 2];
                void* const value = oldEntries[index * 2 + 1];
                if (key > ZERO && key < MORE) Put(table, key, value, hash, equals);
            }

            free(oldEntries);
        }
    }

    integer index = hash ? hash(key) & mask : (natural)key & mask;
    integer indexToInsertAt = -1;
    integer indexToRemoveAt = -1;
    integer indexToReplaceAt = -1;

    for (int i = 0; i <= mask; i += 1) {
        void* const keyAtIndex = table->entries[index * 2];

        if (keyAtIndex == ZERO || keyAtIndex == MORE) {
            if (indexToInsertAt < 0) indexToInsertAt = index;
            continue;
        }

        if (equals ? equals(keyAtIndex, key) : keyAtIndex == key) {
            indexToInsertAt = -1;
            indexToRemoveAt = value == 0 ? index : -1;
            indexToReplaceAt = value != 0 ? index : -1;
            break;
        }

        index = (index + 1) & mask;
    }

    if (indexToInsertAt >= 0) {
        table->entries[indexToInsertAt * 2] = key;
        table->entries[indexToInsertAt * 2 + 1] = value;
        table->count += 1;
        return indexToInsertAt;
    }

    if (indexToRemoveAt >= 0) {
        table->entries[indexToRemoveAt * 2] = MORE;
        table->entries[indexToRemoveAt * 2 + 1] = ZERO;
        table->count -= 1;
        return indexToRemoveAt;
    }

    if (indexToReplaceAt >= 0) {
        table->entries[indexToReplaceAt * 2 + 1] = value;
        return indexToReplaceAt;
    }

    return -1;
}


// ------------------------------------------------------- Object Methods ------


static var ObjectCreate(struct Object* self, var command, var options, ...) {
    struct Object* object = calloc(1, sizeof(struct Object));
    object->behavior = self->behavior;
    object->retainCount = 1;
    return object;
}


static var ObjectDestroy(struct Object* self, var command, var options, ...) {
    free(self);
    return null;
}


static var ObjectNew(struct Object* self, var command, var options, ...) {
    self = send(self, "create");
    self = send(self, "init");
    return autorelease(self);
}


static var ObjectInit(struct Object* self, var command, var options, ...) {
    return self;
}


static var ObjectIsKindOf(struct Object* self, var command, var object, var options, ...) {
    var proto = self;
    do {
        if (proto == object) return yes;
        proto = send(proto, "proto");
    } while (proto != null);
    return no;
}


static var ObjectIsMutable(struct Object* self, var command, var options, ...) {
    return yes;
}


static var ObjectRespondsTo(struct Object* self, var command, var commandToCheck, var options, ...) {
    void* code = lookup(self, commandToCheck, 0);
    return Boolean(code != ZERO);
}


static var ObjectAsString(struct Object* self, var command, var options, ...) {
    if (self == Object) return ObjectName;
    // TODO: put in the address of the object.
    return String("<Object XXX>");
}


static var ObjectHash(struct Object* self, var command, var options, ...) {
    return autorelease(NumberMake((natural)self));
}


static var ObjectEquals(struct Object* self, var command, var object, var options, ...) {
    return BooleanMake(self == object);
}


static var ObjectCompare(struct Object* self, var command, var object, var options, ...) {
    throw(CommandNotAllowedException);
    return null;
}


static var ObjectCopy(struct Object* self, var command, var options, ...) {
    throw(CommandNotAllowedException);
    return null;
}


static var ObjectAddMethodBlock(struct Object* self, var command, var method, var block, var options, ...) {
    if (self->behavior->owner != self) {
        struct Object* parent = self->behavior->owner;
        self->behavior = calloc(1, sizeof(struct Behavior));
        self->behavior->owner = self;
        self->behavior->parent = parent;
        Init(&self->behavior->cache, parent->behavior->cache.count);
        Init(&self->behavior->methods, METHODS_DEFAULT_CAPACITY);
        Init(&self->behavior->children, CHILDREN_DEFAULT_CAPACITY);
        Put(&parent->behavior->children, self, yes, ZERO, ZERO);
    }

    Put(&self->behavior->methods, method, block(block).code, ZERO, ZERO);
    preserve(method);

    // TODO: clear cache of all children.

    return self;
}


static var ObjectRemoveMethod(struct Object* self, var command, var method, var options, ...) {
    // TODO: implement.
    return null;
}


static var ObjectProto(struct Object* self, var command, var options, ...) {
    struct Object* parent = self->behavior->parent;
    struct Object* owner = self->behavior->owner;
    return owner == self ? parent : owner;
}


static var ObjectSetProto(struct Object* self, var command, var proto, var options, ...) {
    // TODO: implement.
    return null;
}


static var ObjectInfo(struct Object* self, var command, var message, var options, ...) {
    var description = send(message, "as-string");
    fprintf(stdout, "[INFO] %s\n", string(description).characters);
    return self;
}


static var ObjectWarning(struct Object* self, var command, var message, var options, ...) {
    var description = send(message, "as-string");
    fprintf(stderr, "[WARNING] %s\n", string(description).characters);
    return self;
}


static var ObjectError(struct Object* self, var command, var message, var options, ...) {
    var description = send(message, "as-string");
    fprintf(stderr, "[ERROR] %s\n", string(description).characters);
    throw(ErrorOccuredException);
    return self;
}


static var ObjectDebug(struct Object* self, var command, var message, var options, ...) {
    var description = send(message, "as-string");
    fprintf(stdout, "[DEBUG] %s\n", string(description).characters);
    return self;
}


// ------------------------------------------------------ Boolean Methods ------


static var BooleanCreate(struct Boolean* self, var command, var options, ...) {
    throw(CommandNotAllowedException);
    return null;
}


static var BooleanDestroy(struct Boolean* self, var command, var options, ...) {
    throw(CommandNotAllowedException);
    return null;
}


static var BooleanInit(struct Boolean* self, var command, var options, ...) {
    throw(CommandNotAllowedException);
    return null;
}


static var BooleanAsString(struct Boolean* self, var command, var options, ...) {
    if (self == Boolean) return BooleanName;
    if (self == yes) return yesAsString;
    if (self == no) return noAsString;
    throw(InternalInconsistencyException);
    return null;
}


static var BooleanIsMutable(struct Boolean* self, var command, var options, ...) {
    return no;
}


static var BooleanCompare(struct Boolean* self, var command, var object, var options, ...) {
    if (self == Boolean) return null;
    if (object != yes && object != no) return null;
    if (self == no && object == yes) return Number(-1);
    if (self == yes && object == no) return Number(+1);
    return Number(0);
}


static var BooleanCopy(struct Boolean* self, var command, var options, ...) {
    return self;
}


// ------------------------------------------------------- Number Methods ------


static var NumberCreate(struct Number* self, var command, var options, ...) {
    struct Number* number = calloc(1, sizeof(struct Number));
    number->behavior = self->behavior;
    number->retainCount = 1;
    number->number = 0;
    return number;
}


static var NumberIsMutable(struct Number* self, var command, var options, ...) {
    return no;
}


static var NumberAsString(struct Number* self, var command, var options, ...) {
    if (self == Number) return NumberName;

    // TODO: tweak to not print trailing zeros.
    decimal integerPart = 0;
    decimal fractionalPart = modf(self->number, &integerPart);
    integer const bufferSize = 1024 * 1024;
    char buffer[bufferSize + 1];
    int length = 0;

    if (fractionalPart == 0) {
        length = snprintf(buffer, bufferSize + 1, "%li", (integer)self->number);
    } else {
        length = snprintf(buffer, bufferSize + 1, "%f", self->number);
    }

    return autorelease(StringMake(length, buffer));
}


static var NumberHash(struct Number* self, var command, var options, ...) {
    return self;
}


static var NumberEquals(struct Number* self, var command, var object, var options, ...) {
    return DecimalFrom(self) == DecimalFrom(object) ? yes : no;
}


static var NumberCompare(struct Number* self, var command, var object, var options, ...) {
    decimal const number1 = DecimalFrom(self);
    decimal const number2 = DecimalFrom(object);
    if (number1 < number2) return Number(-1);
    if (number1 > number2) return Number(+1);
    return Number(0);
}


static var NumberCopy(struct Number* self, var command, var options, ...) {
    return retain(self);
}


// -------------------------------------------------------- Block Methods ------


static var BlockCreate(struct Block* self, var command, var options, ...) {
    struct Block* block = calloc(1, sizeof(struct Block));
    block->behavior = self->behavior;
    block->retainCount = 1;
    block->code = ZERO;
    return null;
}


static var BlockIsMutable(struct Block* self, var command, var options, ...) {
    return no;
}


static var BlockAsString(struct Block* self, var command, var options, ...) {
    if (self == Block) return BlockName;

    static struct String* string = ZERO;
    if (string == ZERO) {
        // TODO: put in the address of the block.
        string = preserve(String("<Block XXX>"));
    }
    return string;
}


// --------------------------------------------------------- Data Methods ------


static var DataCreate(struct Data* self, var command, var options, ...) {
    struct Data* data = calloc(1, sizeof(struct Data));
    data->behavior = self->behavior;
    data->retainCount = 1;
    data->capacity = 0;
    data->count = 0;
    data->bytes = ZERO;
    return data;
}


static var DataDestroy(struct Data* self, var command, var options, ...) {
    free(self->bytes);
    return super(self, "destroy");
}


static var DataInit(struct Data* self, var command, var options, ...) {
    return send(self, "init-with-capacity*", Number(1));
}


static var DataInitWithCapacity(struct Data* self, var command, var capacity, var options, ...) {
    self = super(self, "init");
    self->capacity = IntegerFrom(capacity);
    self->capacity = MAX(self->capacity, DATA_DEFAULT_CAPACITY);
    self->capacity = RoundUpToPowerOfTwo(self->capacity);
    self->count = 0;
    self->bytes = calloc(self->capacity, 1);
    return self;
}


static var DataIsMutable(struct Data* self, var command, var options, ...) {
    return self->capacity >= 0 ? yes : no;
}


static var DataAsString(struct Data* self, var command, var options, ...) {
    if (self == Data) return DataName;

    // TODO: implement Base64 encoding.
    char* table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    return null;
}


static var DataHash(struct Data* self, var command, var options, ...) {
    natural const digest = Digest(self->count, self->bytes);
    return autorelease(NumberMake((decimal)digest));
}


static var DataEquals(struct Data* self, var command, var object, var options, ...) {
    struct Data* data1 = self;
    struct Data* data2 = object;

    if (self == object) return yes;
    if (send(object, "is-kind-of*", Data) == no) return no;
    if (data1->count != data2->count) return no;

    integer const result = memcmp(data1->bytes, data2->bytes, data1->count);
    return result == 0 ? yes : no;
}


static var DataCopy(struct Data* self, var command, var options, ...) {
    if (self->capacity < 0) return retain(self);
    // TODO: make an immutable copy.
    return null;
}


static var DataAtCount(struct Data* self, var index, var count) {
    // TODO: implement.
    return null;
}


static var DataReplaceAtCountWith(struct Data* self, var index, var count, var data) {
    // TODO: implement.
    return self;
}


static var DataCount(struct Data* self) {
    return autorelease(NumberMake(self->count));
}


// -------------------------------------------------------- Array Methods ------


static var ArrayCreate(struct Array* self, var command, var options, ...) {
    struct Array* array = calloc(1, sizeof(struct Array));
    array->behavior = self->behavior;
    array->retainCount = 1;
    array->capacity = 0;
    array->count = 0;
    array->objects = ZERO;
    return array;
}


static var ArrayDestroy(struct Array* self, var command, var options, ...) {
    for (int i = 0; i < self->count; i += 1) {
        release(self->objects[i]);
    }
    free(self->objects);
    return super(self, "destroy");
}


static var ArrayInit(struct Array* self, var command, var options, ...) {
    return send(self, "init-with-capacity*", Number(1));
}


static var ArrayInitWithCapacity(struct Array* self, var command, var capacity, var options, ...) {
    self = super(self, "init");
    self->capacity = IntegerFrom(capacity);
    self->capacity = MAX(self->capacity, ARRAY_DEFAULT_CAPACITY);
    self->capacity = RoundUpToPowerOfTwo(self->capacity);
    self->count = 0;
    self->objects = calloc(self->capacity, sizeof(var));
    return self;
}


static var ArrayIsMutable(struct Array* self, var command, var options, ...) {
    return self->capacity >= 0 ? yes : no;
}


static var ArrayAsString(struct Array* self, var command, var options, ...) {
    if (self == Array) return ArrayName;
    // TODO: implement.
    return null;
}


static var ArrayHash(struct Array* self, var command, var options, ...) {
    // TODO: implement.
    return null;
}


static var ArrayEquals(struct Array* self, var command, var object, var options, ...) {
    struct Array* array1 = self;
    struct Array* array2 = object;

    if (self == object) return yes;
    if (send(object, "is-kind-of*", Array) == no) return no;
    if (array1->count != array2->count) return no;

    for (int index = 0; index < array1->count; index += 1) {
        var object1 = array1->objects[index];
        var object2 = array2->objects[index];
        if (send(object1, "equals*", object2) == no) return no;
    }

    return yes;
}


static var ArrayCopy(struct Array* self, var command, var options, ...) {
    var mutable = option(String("mutable"), no);
    if (self->capacity < 0) return retain(self);
    // TODO: make an immutable copy.
    return null;
}


static var ArrayAt(struct Array* self, var index) {
    integer const integerIndex = IntegerFrom(index);
    integer const integerCount = self->count;
    if (integerIndex < 0 || integerIndex >= integerCount) throw(OutOfBoundsException);
    return self->objects[integerIndex];
}


static var ArrayCount(struct Array* self) {
    return autorelease(NumberMake(self->count));
}


static var ArrayReplaceAtCountWith(struct Array* self, var index, var count, var array) {
    // TODO: implement.
    return self;
}


// ------------------------------------------------------- String Methods ------


static var StringCreate(struct String* self, var command, var options, ...) {
    struct String* string = calloc(1, sizeof(struct String));
    string->behavior = self->behavior;
    string->retainCount = 1;
    string->capacity = -1;
    string->length = 0;
    string->characters = ZERO;
    return string;
}


static var StringDestroy(struct String* self, var command, var options, ...) {
    if (self->capacity < 0) {
        Put(&StringTable, self, ZERO, ComputeHashOfString, CheckIfStringsAreEqual);
    }

    free(self->characters);
    return super(self, "destroy");
}


static var StringInit(struct String* self, var command, var options, ...) {
    return send(self, "init-with-capacity*", Number(1));
}


static var StringInitWithCapacity(struct String* self, var command, var capacity, var options, ...) {
    self = super(self, "init");
    self->capacity = IntegerFrom(capacity);
    self->capacity = MAX(self->capacity, STRING_DEFAULT_CAPACITY);
    self->capacity = RoundUpToPowerOfTwo(self->capacity + 1) - 1;
    self->length = 0;
    self->characters = calloc(self->capacity + 1, sizeof(char));
    return self;
}


static var StringIsMutable(struct String* self, var command, var options, ...) {
    return self->capacity >= 0 ? yes : no;
}


static var StringAsString(struct String* self, var command, var options, ...) {
    if (self == String) return StringName;
    return autorelease(send(self, "copy"));
}


static var StringHash(struct String* self, var command, var options, ...) {
    natural const hash = Digest(self->length, self->characters);
    return autorelease(NumberMake((decimal)hash));
}


static var StringEquals(struct String* self, var command, var object, var options, ...) {
    if (self == object) return yes;
    if (send(object, "is-kind-of*", String) == no) return no;

    struct String* string1 = self;
    struct String* string2 = object;
    if (string1->length != string2->length) return no;

    integer const result = strncmp(string1->characters, string2->characters, string1->length);
    return result == 0 ? yes : no;
}


static var StringCompare(struct String* self, var command, var object, var options, ...) {
    if (self == object) return yes;
    if (send(object, "is-kind-of*", String) == no) return null;

    struct String* string1 = self;
    struct String* string2 = object;

    integer const result = strncmp(string1->characters, string2->characters, string1->length);
    return autorelease(NumberMake(result));
}


static var StringCopy(struct String* self, var command, var options, ...) {
    if (self->capacity < 0) return retain(self);
    // TODO: make an immutable copy.
    return null;
}


static var StringAtCount(struct String* self, var index, var count) {
    // TODO: implement.
    return null;
}


static var StringLength(struct String* self) {
    return autorelease(NumberMake(self->length));
}


static var StringReplaceAtCountWith(struct String* self, var index, var count, var string) {
    // TODO: implement.
    return self;
}


// --------------------------------------------------- Dictionary Methods ------


static var DictionaryCreate(struct Dictionary* self, var command, var options, ...) {
    struct Dictionary* dictionary = calloc(1, sizeof(struct Dictionary));
    dictionary->behavior = self->behavior;
    dictionary->retainCount = 1;
    dictionary->capacity = 0;
    dictionary->count = 0;
    dictionary->mask = 0;
    dictionary->entries = ZERO;
    return dictionary;
}


static var DictionaryDestroy(struct Dictionary* self, var command, var options, ...) {
    for (int i = 0; i < self->count; i += 2) {
        release(self->entries[i]);
        release(self->entries[i + 1]);
    }
    free(self->entries);
    return super(self, "destroy");
}


static var DictionaryInit(struct Dictionary* self, var command, var options, ...) {
    return send(self, "init-with-capacity*", Number(1));
}


static var DictionaryInitWithCapacity(struct Dictionary* self, var command, var capacity, var options, ...) {
    self = super(self, "init");
    self->capacity = MAX(IntegerFrom(capacity), DICTIONARY_DEFAULT_CAPACITY);
    self->capacity = RoundUpToPowerOfTwo(self->capacity);
    self->count = 0;
    self->mask = self->capacity - 1;
    self->entries = calloc(self->capacity * 2, sizeof(var));
    return self;
}


static var DictionaryIsMutable(struct Dictionary* self, var command, var options, ...) {
    return self->capacity >= 0 ? yes : no;
}


static var DictionaryAsString(struct Dictionary* self, var command, var options, ...) {
    if (self == Dictionary) return DictionaryName;
    // TODO: implement.
    return null;
}


static var DictionaryHash(struct Dictionary* self, var command, var options, ...) {
    // TODO: implement.
    return null;
}


static var DictionaryEquals(struct Dictionary* self, var command, var object, var options, ...) {
    // TODO: implement.
    return null;
}


static var DictionaryCopy(struct Dictionary* self, var command, var options, ...) {
    if (self->capacity < 0) return retain(self);
    // TODO: make an immutable copy.
    return null;
}


static var DictionaryGet(struct Dictionary* self, var command, var key, var options, ...) {
    natural const mask = self->mask;
    natural const hash = NaturalFrom(send(key, "hash"));
    natural index = hash & mask;

    for (int i = 0; i <= mask; i += 1) {
        integer const indexOfKey = index * 2;
        integer const indexOfValue = indexOfKey + 1;
        var keyAtIndex = self->entries[indexOfKey];
        var valueAtIndex = self->entries[indexOfValue];
        if (send(keyAtIndex, "equals*", key) == yes) {
            return valueAtIndex;
        }
        index = (index + 1) & mask;
    }

    return null;
}


static var DictionarySetTo(struct Dictionary* self, var command, var key, var value, var options, ...) {
    DictionaryEnsureCapacity(self, self->count + 1);

    natural const mask = self->mask;
    natural const hash = NaturalFrom(send(key, "hash"));
    natural index = hash & mask;

    for (int i = 0; i <= mask; i += 1) {
        integer const indexOfKey = index * 2;
        integer const indexOfValue = indexOfKey + 1;
        var keyAtIndex = self->entries[indexOfKey];
        var valueAtIndex = self->entries[indexOfValue];

        if (keyAtIndex == zero) {
            retain(key);
            retain(value);
            self->entries[indexOfKey] = key;
            self->entries[indexOfValue] = value;
            break;
        }

        if (send(keyAtIndex, "equals*", key) == yes) {
            retain(key);
            retain(value);
            release(self->entries[indexOfKey]);
            release(self->entries[indexOfValue]);
            self->entries[indexOfKey] = key;
            self->entries[indexOfValue] = value;
            break;
        }

        index = (index + 1) & mask;
    }

    return self;
}


static var DictionaryRemove(struct Dictionary* self, var key) {
    natural const mask = self->mask;
    natural const hash = NaturalFrom(send(key, "hash"));
    natural index = hash & mask;

    for (int i = 0; i <= mask; i += 1) {
        integer const indexOfKey = index * 2;
        integer const indexOfValue = indexOfKey + 1;
        var keyAtIndex = self->entries[indexOfKey];
        var valueAtIndex = self->entries[indexOfValue];
        if (send(keyAtIndex, "equals*", key) == yes) {
            release(keyAtIndex);
            release(valueAtIndex);
            self->entries[indexOfKey] = zero;
            self->entries[indexOfValue] = zero;
        }
        index = (index + 1) & mask;
    }

    return self;
}


static var DictionaryCount(struct Dictionary* self) {
    return autorelease(NumberMake(self->count));
}


// --------------------------------------------------------- Null Methods ------


static var NullCreate(struct Object* self, var command, var options, ...) {
    throw(CommandNotAllowedException);
    return null;
}


static var NullDestroy(struct Object* self, var command, var options, ...) {
    throw(CommandNotAllowedException);
    return null;
}


static var NullInit(struct Object* self, var command, var options, ...) {
    throw(CommandNotAllowedException);
    return null;
}


static var NullAsString(struct Object* self, var command, var options, ...) {
    return NullName;
}


static var NullIsMutable(struct Object* self, var command, var options, ...) {
    return no;
}


static var NullEquals(struct Object* self, var command, var object, var options, ...) {
    return object == null ? yes : no;
}


static var NullCopy(struct Object* self, var command, var options, ...) {
    return retain(self);
}


// ---------------------------------------------------- Factory Functions ------


var BooleanMake(long boolean) {
    return boolean ? yes : no;
}


var NumberMake(double value) {
    struct Number* number = calloc(1, sizeof(struct Number));
    number->behavior = &NumberBehavior;
    number->retainCount = 1;
    number->number = value;
    return number;
}


var BlockMake(void* code) {
    assert(code != ZERO);
    struct Block* block = calloc(1, sizeof(struct Block));
    block->behavior = &BlockBehavior;
    block->retainCount = 1;
    block->code = code;
    return block;
}


var DataMake(long count, const void* bytes) {
    assert(count >= 0);
    struct Data* data = calloc(1, sizeof(struct Data));
    data->behavior = &DataBehavior;
    data->retainCount = 1;
    data->capacity = -1;
    data->count = count;
    data->bytes = calloc(count, 1);
    memcpy(data->bytes, bytes, count);
    return data;
}


var ArrayMake(long count, ...) {
    assert(count >= 0);
    struct Array* array = calloc(1, sizeof(struct Array));
    array->behavior = &ArrayBehavior;
    array->retainCount = 1;
    array->capacity = -1;
    array->count = count;
    array->objects = calloc(count, sizeof(var));
    va_list arguments;
    va_start(arguments, count);
    for (int i = 0; i < count; i += 1) {
        var object = va_arg(arguments, var);
        array->objects[i] = retain(object);
    }
    va_end(arguments);
    return array;
}


var StringMake(long length, const char* characters) {
    assert(length >= 0);

    natural const hash = Digest(length, characters);
    bool const couldBeCommandOrKey = length <= MAX_KEY_AND_COMMAND_LENGTH;

    if (couldBeCommandOrKey) {
        struct String proxy = {&StringBehavior, RETAIN_COUNT_MAX, -1, length, hash, (char*)characters};
        struct String* string = Get(&StringTable, &proxy, ComputeHashOfString, CheckIfStringsAreEqual);
        if (string != ZERO) return retain(string);
    }

    struct String* string = calloc(1, sizeof(struct String));
    string->behavior = &StringBehavior;
    string->retainCount = 1;
    string->capacity = -1;
    string->length = length;
    string->hash = hash;
    string->characters = calloc(length + 1, sizeof(char));
    strncpy(string->characters, characters, length);
    if (couldBeCommandOrKey) Put(&StringTable, string, string, ComputeHashOfString, CheckIfStringsAreEqual);

    return string;
}


var DictionaryMake(long count, ...) {
    assert(count >= 0);
    assert(count % 2 == 0);

    struct Dictionary* dictionary = calloc(1, sizeof(struct Dictionary));
    dictionary->behavior = &DictionaryBehavior;
    dictionary->retainCount = 1;
    dictionary->capacity = -1;
    dictionary->count = 0;

    va_list arguments;
    // va_start(arguments, first);
    // var firstKey = first;
    // var firstValue = va_arg(arguments, var);
    // DictionarySetTo(dictionary, zero, firstKey, firstValue, zero);
    // for (int i = 2; i < count; i += 1) {
    //     var key = va_arg(arguments, var);
    //     var value = va_arg(arguments, var);
    //     DictionarySetTo(dictionary, zero, key, value, zero);
    // }
    // va_end(arguments);
    return dictionary;
}


// ------------------------------------------------- Conversion Functions ------


integer IntegerFrom(var number) {
    return (integer)number(number).number;
}


natural NaturalFrom(var number) {
    return (natural)number(number).number;
}


decimal DecimalFrom(var number) {
    return (decimal)number(number).number;
}


// ---------------------------------------------- Collect-Block Functions ------


void* CollectBlockPush() {
    struct CollectBlock* collectBlock = calloc(1, sizeof(struct CollectBlock));
    collectBlock->previousCollectBlock = CollectBlockTop;
    collectBlock->capacity = COLLECT_BLOCK_DEFAULT_CAPACITY;
    collectBlock->count = 0;
    collectBlock->objects = calloc(sizeof(var), COLLECT_BLOCK_DEFAULT_CAPACITY);
    CollectBlockTop = collectBlock;
    return CollectBlockTop;
}


void* CollectBlockPop(void* collectBlock) {
    // TODO: remove  all child collect blocks from the stack
    // and release its objects if landed here through exception.
    assert(collectBlock == CollectBlockTop);
    struct CollectBlock* collectBlockToPop = collectBlock;
    CollectBlockTop = collectBlockToPop->previousCollectBlock;
    for (int index = 0; index < collectBlockToPop->count; index += 1) {
        release(collectBlockToPop->objects[index]);
    }
    free(collectBlockToPop->objects);
    free(collectBlockToPop);
    return ZERO;
}


// -------------------------------------------- Try-Catch-Block Functions ------


void* TryCatchBlockPush() {
    struct TryCatchBlock* tryCatchBlock = calloc(1, sizeof(struct TryCatchBlock));
    tryCatchBlock->previousTryCatchBlock = TryCatchBlockTop;
    tryCatchBlock->exception = null;
    tryCatchBlock->thrown = false;
    TryCatchBlockTop = tryCatchBlock;
    return tryCatchBlock;
}


void* TryCatchBlockPop(void* tryCatchBlock) {
    assert(tryCatchBlock == TryCatchBlockTop);
    struct TryCatchBlock* tryCatchBlockToPop = tryCatchBlock;
    TryCatchBlockTop = tryCatchBlockToPop->previousTryCatchBlock;
    release(tryCatchBlockToPop->exception);
    free(tryCatchBlockToPop);
    return ZERO;
}


void* TryCatchBlockTry(void* tryCatchBlock) {
    assert(tryCatchBlock == TryCatchBlockTop);
    return TryCatchBlockTop->destination;
}


var TryCatchBlockCatch(void* tryCatchBlock) {
    assert(tryCatchBlock == TryCatchBlockTop);
    return TryCatchBlockTop->exception;
}


// ---------------------------------------------------- Keyword Functions ------


var retain(var object) {
    if (object(object).retainCount < RETAIN_COUNT_MAX) object(object).retainCount += 1;
    return object;
}


var release(var object) {
    integer const retainCount = object(object).retainCount;

    if (retainCount >= RETAIN_COUNT_MAX) {
        return object;
    }

    if (retainCount >= 2) {
        object(object).retainCount -= 1;
        return object;
    }

    if (retainCount == 1) {
        object(object).retainCount = 0;
        return send(object, "destroy");
    }

    fprintf(stderr, "[WARNING] Released an object with retain count 0, retain/release calls seem to be unbalanced, aborting ...\n");
    return null;
}


var preserve(var object) {
    object(object).retainCount = RETAIN_COUNT_MAX;
    return object;
}


var autorelease(var object) {
    if (object(object).retainCount >= RETAIN_COUNT_MAX) return object;
    if (CollectBlockTop == ZERO) {
        fprintf(stderr, "[WARNING] No collect block found, leaking ...\n");
        return object;
    }
    integer const capacity = CollectBlockTop->capacity;
    integer const count = CollectBlockTop->count;
    if (count >= capacity) {
        assert(capacity > 0);
        var* objects = calloc(capacity * 2, sizeof(var));
        memcpy(objects, CollectBlockTop->objects, capacity * sizeof(var));
        free(CollectBlockTop->objects);
        CollectBlockTop->objects = objects;
        CollectBlockTop->capacity = capacity * 2;
    }
    CollectBlockTop->objects[count] = object;
    CollectBlockTop->count += 1;
    return object;
}


var import(const char* name) {
    // TODO: implement.
    return null;
}


var export(const char* name, void* code) {
    // TODO: implement.
    return null;
}


void* lookup(var self, var command, natural level) {
    struct Behavior* const behavior = object(self).behavior;
    struct Table* const methods = &behavior->methods;
    struct Table* const cache = &behavior->cache;
    void* const key = (void*)((natural)command | level);

    // Look up in cache.
    void* code = Get(cache, key, ZERO, ZERO);
    if (code != ZERO) return code;

    // Look up in own methods.
    assert(string(command).length <= MAX_KEY_AND_COMMAND_LENGTH);
    code = Get(methods, command, ZERO, ZERO);

    // Continue searching in parent's methods if needed.
    if (behavior->parent != null) {
        if (code && level > 0) code = lookup(behavior->parent, command, level - 1);
        if (!code) code = lookup(behavior->parent, command, level);
    }

    // TOOD: add fallback.

    // cache found method.
    assert(code != ZERO);
    Put(cache, key, code, ZERO, ZERO);
    preserve(command);

    // Return found & now cached method.
    return code;
}


void throw(var exception) {
    retain(exception);
    struct TryCatchBlock* tryCatchBlock = TryCatchBlockTop;
    if (tryCatchBlock != ZERO && tryCatchBlock->thrown) {
        TryCatchBlockPop(tryCatchBlock);
        tryCatchBlock = TryCatchBlockTop;
    }
    if (tryCatchBlock == ZERO) {
        fprintf(stderr, "[ERROR] Exception not catched\n");
        exit(1);
    }
    tryCatchBlock->exception = exception;
    tryCatchBlock->thrown = true;
    longjmp(tryCatchBlock->destination, 1);
}


// -------------------------------------------------------- Bootstrapping ------


bootstrap static void Metal() {
    collect {
        Init(&StringTable, STRING_TABLE_DEFAULT_CAPACITY);

        ObjectBehavior.owner = &ObjectState;
        BooleanBehavior.owner = &BooleanState;
        NumberBehavior.owner = &NumberState;
        BlockBehavior.owner = &BlockState;
        DataBehavior.owner = &DataState;
        ArrayBehavior.owner = &ArrayState;
        StringBehavior.owner = &StringState;
        DictionaryBehavior.owner = &DictionaryState;
        NullBehavior.owner = &NullState;

        ObjectBehavior.parent = null;
        BooleanBehavior.parent = &ObjectState;
        NumberBehavior.parent = &ObjectState;
        BlockBehavior.parent = &ObjectState;
        DataBehavior.parent = &ObjectState;
        ArrayBehavior.parent = &ObjectState;
        StringBehavior.parent = &ObjectState;
        DictionaryBehavior.parent = &ObjectState;
        NullBehavior.parent = &ObjectState;


        Init(&ObjectBehavior.cache, CACHE_DEFAULT_CAPACITY);
        Init(&BooleanBehavior.cache, CACHE_DEFAULT_CAPACITY);
        Init(&NumberBehavior.cache, CACHE_DEFAULT_CAPACITY);
        Init(&BlockBehavior.cache, CACHE_DEFAULT_CAPACITY);
        Init(&DataBehavior.cache, CACHE_DEFAULT_CAPACITY);
        Init(&ArrayBehavior.cache, CACHE_DEFAULT_CAPACITY);
        Init(&StringBehavior.cache, CACHE_DEFAULT_CAPACITY);
        Init(&DictionaryBehavior.cache, CACHE_DEFAULT_CAPACITY);
        Init(&NullBehavior.cache, CACHE_DEFAULT_CAPACITY);

        Init(&ObjectBehavior.methods, 32);
        Init(&BooleanBehavior.methods, 32);
        Init(&NumberBehavior.methods, 32);
        Init(&BlockBehavior.methods, 32);
        Init(&DataBehavior.methods, 32);
        Init(&ArrayBehavior.methods, 32);
        Init(&StringBehavior.methods, 32);
        Init(&DictionaryBehavior.methods, 32);
        Init(&NullBehavior.methods, 32);

        ObjectAddMethodBlock(Object, zero, String("create"), Block(ObjectCreate), zero);
        ObjectAddMethodBlock(Object, zero, String("destroy"), Block(ObjectDestroy), zero);
        ObjectAddMethodBlock(Object, zero, String("init"), Block(ObjectInit), zero);
        ObjectAddMethodBlock(Object, zero, String("new"), Block(ObjectNew), zero);
        ObjectAddMethodBlock(Object, zero, String("is-kind-of*"), Block(ObjectIsKindOf), zero);
        ObjectAddMethodBlock(Object, zero, String("is-mutable"), Block(ObjectIsMutable), zero);
        ObjectAddMethodBlock(Object, zero, String("responds-to*"), Block(ObjectRespondsTo), zero);
        ObjectAddMethodBlock(Object, zero, String("as-string"), Block(ObjectAsString), zero);
        ObjectAddMethodBlock(Object, zero, String("hash"), Block(ObjectHash), zero);
        ObjectAddMethodBlock(Object, zero, String("equals*"), Block(ObjectEquals), zero);
        ObjectAddMethodBlock(Object, zero, String("compare*"), Block(ObjectCompare), zero);
        ObjectAddMethodBlock(Object, zero, String("copy"), Block(ObjectCopy), zero);
        ObjectAddMethodBlock(Object, zero, String("add-method*block*"), Block(ObjectAddMethodBlock), zero);
        ObjectAddMethodBlock(Object, zero, String("remove-method*"), Block(ObjectRemoveMethod), zero);
        ObjectAddMethodBlock(Object, zero, String("proto"), Block(ObjectProto), zero);
        ObjectAddMethodBlock(Object, zero, String("set-proto*"), Block(ObjectSetProto), zero);
        ObjectAddMethodBlock(Object, zero, String("info*"), Block(ObjectInfo), zero);
        ObjectAddMethodBlock(Object, zero, String("warning*"), Block(ObjectWarning), zero);
        ObjectAddMethodBlock(Object, zero, String("error*"), Block(ObjectError), zero);
        ObjectAddMethodBlock(Object, zero, String("debug*"), Block(ObjectDebug), zero);

        ObjectAddMethodBlock(Boolean, zero, String("create"), Block(BooleanCreate), zero);
        ObjectAddMethodBlock(Boolean, zero, String("destroy"), Block(BooleanDestroy), zero);
        ObjectAddMethodBlock(Boolean, zero, String("init"), Block(BooleanInit), zero);
        ObjectAddMethodBlock(Boolean, zero, String("as-string"), Block(BooleanAsString), zero);
        ObjectAddMethodBlock(Boolean, zero, String("is-mutable"), Block(BooleanIsMutable), zero);
        ObjectAddMethodBlock(Boolean, zero, String("compare*"), Block(BooleanCompare), zero);
        ObjectAddMethodBlock(Boolean, zero, String("copy"), Block(BooleanCopy), zero);

        ObjectAddMethodBlock(Number, zero, String("create"), Block(NumberCreate), zero);
        ObjectAddMethodBlock(Number, zero, String("is-mutable"), Block(NumberIsMutable), zero);
        ObjectAddMethodBlock(Number, zero, String("as-string"), Block(NumberAsString), zero);
        ObjectAddMethodBlock(Number, zero, String("hash"), Block(NumberHash), zero);
        ObjectAddMethodBlock(Number, zero, String("equals*"), Block(NumberEquals), zero);
        ObjectAddMethodBlock(Number, zero, String("compare*"), Block(NumberCompare), zero);
        ObjectAddMethodBlock(Number, zero, String("copy"), Block(NumberCopy), zero);

        ObjectAddMethodBlock(Block, zero, String("create"), Block(BlockCreate), zero);
        ObjectAddMethodBlock(Block, zero, String("is-mutable"), Block(BlockIsMutable), zero);
        ObjectAddMethodBlock(Block, zero, String("as-string"), Block(BlockAsString), zero);

        ObjectAddMethodBlock(Data, zero, String("create"), Block(DataCreate), zero);
        ObjectAddMethodBlock(Data, zero, String("destroy"), Block(DataDestroy), zero);
        ObjectAddMethodBlock(Data, zero, String("init"), Block(DataInit), zero);
        ObjectAddMethodBlock(Data, zero, String("init-with-capacity*"), Block(DataInitWithCapacity), zero);
        ObjectAddMethodBlock(Data, zero, String("is-mutable"), Block(DataIsMutable), zero);
        ObjectAddMethodBlock(Data, zero, String("as-string"), Block(DataAsString), zero);
        ObjectAddMethodBlock(Data, zero, String("hash"), Block(DataHash), zero);
        ObjectAddMethodBlock(Data, zero, String("equals*"), Block(DataEquals), zero);
        ObjectAddMethodBlock(Data, zero, String("copy"), Block(DataCopy), zero);
        ObjectAddMethodBlock(Data, zero, String("at*count*"), Block(DataAtCount), zero);
        ObjectAddMethodBlock(Data, zero, String("replace-at*count*with*"), Block(DataReplaceAtCountWith), zero);
        ObjectAddMethodBlock(Data, zero, String("count"), Block(DataCount), zero);

        ObjectAddMethodBlock(Array, zero, String("create"), Block(ArrayCreate), zero);
        ObjectAddMethodBlock(Array, zero, String("destroy"), Block(ArrayDestroy), zero);
        ObjectAddMethodBlock(Array, zero, String("init"), Block(ArrayInit), zero);
        ObjectAddMethodBlock(Array, zero, String("init-with-capacity*"), Block(ArrayInitWithCapacity), zero);
        ObjectAddMethodBlock(Array, zero, String("is-mutable"), Block(ArrayIsMutable), zero);
        ObjectAddMethodBlock(Array, zero, String("as-string"), Block(ArrayAsString), zero);
        ObjectAddMethodBlock(Array, zero, String("hash"), Block(ArrayHash), zero);
        ObjectAddMethodBlock(Array, zero, String("equals*"), Block(ArrayEquals), zero);
        ObjectAddMethodBlock(Array, zero, String("copy"), Block(ArrayCopy), zero);
        ObjectAddMethodBlock(Array, zero, String("at*"), Block(ArrayAt), zero);
        ObjectAddMethodBlock(Array, zero, String("count"), Block(ArrayCount), zero);
        ObjectAddMethodBlock(Array, zero, String("replace-at*count*with*"), Block(ArrayReplaceAtCountWith), zero);

        ObjectAddMethodBlock(String, zero, String("create"), Block(StringCreate), zero);
        ObjectAddMethodBlock(String, zero, String("destroy"), Block(StringDestroy), zero);
        ObjectAddMethodBlock(String, zero, String("init"), Block(StringInit), zero);
        ObjectAddMethodBlock(String, zero, String("init-with-capacity*"), Block(StringInitWithCapacity), zero);
        ObjectAddMethodBlock(String, zero, String("is-mutable"), Block(StringIsMutable), zero);
        ObjectAddMethodBlock(String, zero, String("as-string"), Block(StringAsString), zero);
        ObjectAddMethodBlock(String, zero, String("hash"), Block(StringHash), zero);
        ObjectAddMethodBlock(String, zero, String("equals*"), Block(StringEquals), zero);
        ObjectAddMethodBlock(String, zero, String("compare*"), Block(StringCompare), zero);
        ObjectAddMethodBlock(String, zero, String("copy"), Block(StringCopy), zero);
        ObjectAddMethodBlock(String, zero, String("at*count*"), Block(StringAtCount), zero);
        ObjectAddMethodBlock(String, zero, String("length"), Block(StringLength), zero);
        ObjectAddMethodBlock(String, zero, String("replace-at*count*with*"), Block(StringReplaceAtCountWith), zero);

        ObjectAddMethodBlock(Dictionary, zero, String("create"), Block(DictionaryCreate), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("destroy"), Block(DictionaryDestroy), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("init"), Block(DictionaryInit), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("init-with-capacity*"), Block(DictionaryInitWithCapacity), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("is-mutable"), Block(DictionaryIsMutable), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("as-string"), Block(DictionaryAsString), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("hash"), Block(DictionaryHash), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("equals*"), Block(DictionaryEquals), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("copy"), Block(DictionaryCopy), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("get*"), Block(DictionaryGet), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("set*to*"), Block(DictionarySetTo), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("remove*"), Block(DictionaryRemove), zero);
        ObjectAddMethodBlock(Dictionary, zero, String("count"), Block(DictionaryCount), zero);

        ObjectAddMethodBlock(null, zero, String("create"), Block(NullCreate), zero);
        ObjectAddMethodBlock(null, zero, String("destroy"), Block(NullDestroy), zero);
        ObjectAddMethodBlock(null, zero, String("init"), Block(NullInit), zero);
        ObjectAddMethodBlock(null, zero, String("as-string"), Block(NullAsString), zero);
        ObjectAddMethodBlock(null, zero, String("is-mutable"), Block(NullIsMutable), zero);
        ObjectAddMethodBlock(null, zero, String("equals*"), Block(NullEquals), zero);
        ObjectAddMethodBlock(null, zero, String("copy"), Block(NullCopy), zero);

        Init(&ObjectBehavior.children, 64);
        Init(&BooleanBehavior.children, 1);
        Init(&NumberBehavior.children, 1);
        Init(&BlockBehavior.children, 1);
        Init(&DataBehavior.children, 1);
        Init(&ArrayBehavior.children, 1);
        Init(&StringBehavior.children, 1);
        Init(&DictionaryBehavior.children, 1);
        Init(&NullBehavior.children, 1);

        Put(&ObjectBehavior.children, Boolean, yes, ZERO, ZERO);
        Put(&ObjectBehavior.children, Number, yes, ZERO, ZERO);
        Put(&ObjectBehavior.children, Block, yes, ZERO, ZERO);
        Put(&ObjectBehavior.children, Data, yes, ZERO, ZERO);
        Put(&ObjectBehavior.children, Array, yes, ZERO, ZERO);
        Put(&ObjectBehavior.children, String, yes, ZERO, ZERO);
        Put(&ObjectBehavior.children, Dictionary, yes, ZERO, ZERO);
        Put(&ObjectBehavior.children, null, yes, ZERO, ZERO);

        // TODO: implement.

        ObjectName = preserve(String("Object"));
        BooleanName = preserve(String("Boolean"));
        NumberName = preserve(String("Number"));
        BlockName = preserve(String("Block"));
        DataName = preserve(String("Data"));
        ArrayName = preserve(String("Array"));
        StringName = preserve(String("String"));
        DictionaryName = preserve(String("Dictionary"));
        NullName = preserve(String("Null"));

        noAsString = preserve(String("no"));
        yesAsString = preserve(String("yes"));

        GenericException = preserve(String("GenericException"));
        OutOfBoundsException = preserve(String("OutOfBoundsException"));
        InvalidArgumentException = preserve(String("InvalidArgumentException"));
        CommandNotAllowedException = preserve(String("CommandNotAllowedException"));
        InternalInconsistencyException = preserve(String("InternalInconsistencyException"));
        ErrorOccuredException = preserve(String("ErrorOccuredException"));
    }
}


// ------------------------------------------------------------- Private -------


static void DataEnsureCapacity(struct Data* data, integer requiredCapacity) {
    if (requiredCapacity <= DATA_DEFAULT_CAPACITY) requiredCapacity = DATA_DEFAULT_CAPACITY;
    if (requiredCapacity <= data->capacity) return;
    data->capacity = RoundUpToPowerOfTwo(requiredCapacity);
    data->bytes = realloc(data->bytes, data->capacity);
}


static void ArrayEnsureCapacity(struct Array* array, integer requiredCapacity) {
    if (requiredCapacity <= ARRAY_DEFAULT_CAPACITY) requiredCapacity = ARRAY_DEFAULT_CAPACITY;
    if (requiredCapacity <= array->capacity) return;
    array->capacity = RoundUpToPowerOfTwo(requiredCapacity);
    array->objects = realloc(array->objects, sizeof(var) * array->capacity);
}


static void StringEnsureCapacity(struct String* string, integer requiredCapacity) {
    if (requiredCapacity <= STRING_DEFAULT_CAPACITY) requiredCapacity = STRING_DEFAULT_CAPACITY;
    if (requiredCapacity <= string->capacity) return;
    string->capacity = RoundUpToPowerOfTwo(requiredCapacity + 1) - 1;
    string->characters = realloc(string->characters, sizeof(char) * (string->capacity + 1));
}


static void DictionaryEnsureCapacity(struct Dictionary* dictionary, integer requiredCapacity) {
    if (requiredCapacity <= DICTIONARY_DEFAULT_CAPACITY) requiredCapacity = DICTIONARY_DEFAULT_CAPACITY;
    integer const oldCapacity = dictionary->capacity;
    integer const oneFourthOfOldCapacity = oldCapacity >> 2;
    integer const threeFourthOfOldCapacity = oldCapacity - oneFourthOfOldCapacity;
    integer const newCapacity = RoundUpToPowerOfTwo(requiredCapacity + (requiredCapacity >> 1));
    if (requiredCapacity < threeFourthOfOldCapacity) return;

    var* oldEntries = dictionary->entries;
    var* newEntries = calloc(sizeof(var), 2 * newCapacity);

    dictionary->capacity = newCapacity;
    dictionary->count = 0;
    dictionary->mask = dictionary->capacity - 1;
    dictionary->entries = newEntries;

    for (integer index = 0; index < oldCapacity; index += 1) {
        var key = oldEntries[index * 2];
        var value = oldEntries[index * 2 + 1];
        if (key) DictionarySetTo(dictionary, zero, key, value, zero);
    }

    free(oldEntries);
}


static inline natural RoundUpToPowerOfTwo(natural number) {
    uint64_t value = number;
    value -= 1;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    value += 1;
    return (natural)value;
}


static inline natural RoundDownToPowerOfTwo(natural number) {
    uint64_t value = number;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    value -= value >> 1;
    return (natural)value;
}


static natural Digest(integer count, const void* bytes) {
    static uint64_t const defaultSeed = 0;
    uint64_t const seed = defaultSeed;
    uint64_t const m = 0xc6a4a7935bd1e995;
    int64_t const r = 47;

    uint64_t h = seed ^ (count * m);

    const uint64_t* data = (const uint64_t*)bytes;
    const uint64_t* end = data + (count / 8);

    while (data != end) {
        uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const unsigned char* data2 = (const unsigned char*)data;
    switch (count & 7 /* count % 8 */) {
        case 7: h ^= (uint64_t)(data2[6]) << 48;
        case 6: h ^= (uint64_t)(data2[5]) << 40;
        case 5: h ^= (uint64_t)(data2[4]) << 32;
        case 4: h ^= (uint64_t)(data2[3]) << 24;
        case 3: h ^= (uint64_t)(data2[2]) << 16;
        case 2: h ^= (uint64_t)(data2[1]) << 8;
        case 1: h ^= (uint64_t)(data2[0]);
                h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    natural hash = (natural)h;
    if (hash == NATURAL_MAX) hash = NATURAL_MAX - 1;
    if (hash == NATURAL_MIN) hash = NATURAL_MIN + 1;
    return hash;
}


static natural ComputeHashOfString(void* string) {
    if (string(string).hash == 0) {
        string(string).hash = Digest(string(string).length, string(string).characters);
    }
    return string(string).hash;
}


static bool CheckIfStringsAreEqual(void* string1, void* string2) {
    if (string1 == string2) return true;
    if (string(string1).length != string(string2).length) return false;
    return strncmp(string(string1).characters, string(string2).characters, string(string1).length) == 0;
}
