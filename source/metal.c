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
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>


// --------------------------------------------------- Constants & Macros ------


#define RETAIN_COUNT_MAX NATURAL_MAX
#define DATA_DEFAULT_CAPACITY 16
#define ARRAY_DEFAULT_CAPACITY 16
#define STRING_DEFAULT_CAPACITY 15 // +1 for '\0'
#define DICTIONARY_DEFAULT_CAPACITY 8
#define CACHE_DEFAULT_CAPACITY 32
#define CHILDREN_DEFAULT_CAPACITY 8
#define METHODS_DEFAULT_CAPACITY 8
#define COLLECT_BLOCK_DEFAULT_CAPACITY 2048
#define SYMBOL_TABLE_DEFAULT_CAPACITY 2048
#define STRING_TABLE_DEFAULT_CAPACITY 2048
#define MAX_KEY_AND_COMMAND_LENGTH 2048
#define MUTABLE_FLAG ((natural)1 << 0)


#define MAX(value1, value2) ((value1) > (value2) ? (value1) : (value2))
#define MIN(value1, value2) ((value1) < (value2) ? (value1) : (value2))


#define table(pointer) (*((struct Table*)pointer))
#define meta(pointer) (*((struct Meta*)pointer))
#define object(pointer) (*((struct Object*)pointer))
#define boolean(pointer) (*((struct Boolean*)pointer))
#define number(pointer) (*((struct Number*)pointer))
#define block(pointer) (*((struct Block*)pointer))
#define data(pointer) (*((struct Data*)pointer))
#define array(pointer) (*((struct Array*)pointer))
#define string(pointer) (*((struct String*)pointer))
#define dictionary(pointer) (*((struct Dictionary*)pointer))


#define bootstrap __attribute__((constructor(128)))
#define assert(condition, message, args...) if (!(condition)) { fprintf(stderr, "[ERROR] Assertion failure in function %s in file %s line %d: " message, __FUNCTION__, __FILE__, __LINE__, ## args); int* pointer = NULL; *pointer = 0; }
#define fail(message, args...) { fprintf(stderr, "[ERROR] Failed in function %s in file %s line %d: " message, __FUNCTION__, __FILE__, __LINE__, ## args); int* pointer = NULL; *pointer = 0; }


// ----------------------------------------------------------- Structures ------


struct Entry {
    natural probe;
    natural key;
    natural value;
    natural extra;
};


struct Table {
    natural mask;
    natural count;
    natural probeMax;
    struct Entry* entries;
};


struct Meta {
    var owner;
    var parent;
    natural size;
    struct Table cache;
    struct Table methods;
    struct Table children;
};


struct Object {
    struct Meta* meta;
    natural retainCount;
    natural flags;
};


struct Boolean {
    struct Meta* meta;
    natural retainCount;
    natural flags;
};


struct Number {
    struct Meta* meta;
    natural retainCount;
    natural flags;
    double number;
};


struct Block {
    struct Meta* meta;
    natural retainCount;
    natural flags;
    void* code;
};


struct Data {
    struct Meta* meta;
    natural retainCount;
    natural flags;
    integer capacity;
    integer count;
    natural hash;
    void* bytes;
};


struct Array {
    struct Meta* meta;
    natural retainCount;
    natural flags;
    integer capacity;
    integer count;
    natural hash;
    var* objects;
};


struct String {
    struct Meta* meta;
    natural retainCount;
    natural flags;
    integer capacity;
    integer length;
    natural hash;
    char* characters;
};


struct Dictionary {
    struct Meta* meta;
    natural retainCount;
    natural flags;
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


// ---------------------------------------------------------------- Types ------


typedef natural (*HashFunction)(natural);
typedef bool (*EqualsFunction)(natural, natural);


// ------------------------------------------------------------ Variables ------


static struct Meta ObjectMeta;
static struct Meta BooleanMeta;
static struct Meta NumberMeta;
static struct Meta BlockMeta;
static struct Meta DataMeta;
static struct Meta ArrayMeta;
static struct Meta StringMeta;
static struct Meta DictionaryMeta;
static struct Meta NullMeta;


static struct Object ObjectState = {.meta = &ObjectMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct Boolean BooleanState = {.meta = &BooleanMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct Number NumberState = {.meta = &NumberMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct Block BlockState = {.meta = &BlockMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct Data DataState = {.meta = &DataMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct Array ArrayState = {.meta = &ArrayMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct String StringState = {.meta = &StringMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct Dictionary DictionaryState = {.meta = &DictionaryMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct Object NullState = {.meta = &NullMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct Boolean YesState = {.meta = &BooleanMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};
static struct Boolean NoState = {.meta = &BooleanMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0};


var const Object = &ObjectState;
var const Boolean = &BooleanState;
var const Number = &NumberState;
var const Block = &BlockState;
var const Data = &DataState;
var const Array = &ArrayState;
var const String = &StringState;
var const Dictionary = &DictionaryState;
var const null = &NullState;
var const yes = &YesState;
var const no = &NoState;


static struct CollectBlock* CollectBlockTop = ZERO;
static struct TryCatchBlock* TryCatchBlockTop = ZERO;


static struct Table StringTable = {.mask = 0, .count = 0, .probeMax = 0, .entries = ZERO};


static struct String* ObjectName = ZERO;
static struct String* BooleanName = ZERO;
static struct String* NumberName = ZERO;
static struct String* BlockName = ZERO;
static struct String* DataName = ZERO;
static struct String* ArrayName = ZERO;
static struct String* StringName = ZERO;
static struct String* DictionaryName = ZERO;
static struct String* NullName = ZERO;
static struct String* NoAsString = ZERO;
static struct String* YesAsString = ZERO;


static struct String* InvalidArgumentException = ZERO;
static struct String* InternalInconsistencyException = ZERO;


// ---------------------------------------------------- Helper Functions -------


static void DataEnsureCapacity(struct Data* data, integer requiredCapacity);
static void ArrayEnsureCapacity(struct Array* array, integer requiredCapacity);
static void StringEnsureCapacity(struct String* string, integer requiredCapacity);
static void DictionaryEnsureCapacity(struct Dictionary* dictionary, integer requiredCapacity);
static inline natural RoundUpToPowerOfTwo(natural number);
static inline natural RoundDownToPowerOfTwo(natural number);
static inline natural StringHashFunction(natural key);
static inline bool StringEqualsFunction(natural key1, natural key2);
static natural Digest(integer count, const void* bytes);


// ------------------------------------------------- Hash Table Functions ------


static inline struct Table* TableCreate(struct Table* table, natural capacity) {
    capacity = RoundUpToPowerOfTwo(capacity);
    table->mask = capacity - 1;
    table->count = 0;
    table->probeMax = 0;
    table->entries = calloc(capacity, sizeof(struct Entry));
    return table;
}


static inline struct Table* TableDestroy(struct Table* table) {
    free(table->entries);
    memset(table, 0, sizeof(struct Table));
    return table;
}


static inline natural TableGet(struct Table* table, struct Entry* entry, HashFunction hashFunction, EqualsFunction equalsFunction) {
    natural const key = entry->key;
    natural const mask = table->mask;
    natural const count = table->count;
    natural const probeMax = table->probeMax;
    struct Entry* const entries = table->entries;
    natural const hash = hashFunction ? hashFunction(key) : key;

    for (natural probe = 1, index = hash & mask; probe <= probeMax; ({ probe += 1; index = (index + 1) & mask; })) {
        struct Entry const current = entries[index];
        bool const isEmpty = current.probe == 0;
        bool const isFound = !isEmpty && (equalsFunction ? equalsFunction(key, current.key) : key == current.key);

        if (isFound) {
            *entry = current;
            return index;
        }
    }

    return NATURAL_MAX;
}


static inline natural TablePut(struct Table* table, struct Entry* entry, HashFunction hashFunction, EqualsFunction equalsFunction) {
    natural const key = entry->key;
    natural const value = entry->value;

    bool const shouldInsert = value != 0;
    bool const shouldRemove = value == 0;

    natural const quarterOfCapacity = (table->mask + 1) >> 2;
    bool const isNearlyFull = table->count > quarterOfCapacity + (quarterOfCapacity << 1);
    bool const isNearlyEmpty = table->count < quarterOfCapacity;
    bool const shouldExpand = shouldInsert & isNearlyFull;
    bool const shouldContract = shouldRemove & isNearlyEmpty;

    if (shouldExpand) {
        natural const capacityOld = table->mask + 1;
        natural const capacityNew = capacityOld << 1;

        struct Entry* const entriesOld = table->entries;
        struct Entry* const entriesNew = calloc(capacityNew, sizeof(struct Entry));

        table->mask = capacityNew - 1;
        table->count = 0;
        table->probeMax = 0;
        table->entries = entriesNew;

        for (natural index = 0; index < capacityOld; index += 1) {
            struct Entry current = entriesOld[index];
            if (current.probe == 0) continue;
            TablePut(table, &current, hashFunction, equalsFunction);
        }

        free(entriesOld);
    }

    if (shouldContract) {
        natural const capacityOld = table->mask + 1;
        natural const capacityNew = capacityOld >> 1;

        struct Entry* const entriesOld = table->entries;
        struct Entry* const entriesNew = calloc(capacityNew, sizeof(struct Entry));

        table->mask = capacityNew - 1;
        table->count = 0;
        table->probeMax = 0;
        table->entries = entriesNew;

        for (natural index = 0; index < capacityOld; index += 1) {
            struct Entry current = entriesOld[index];
            if (current.probe == 0) continue;
            TablePut(table, &current, hashFunction, equalsFunction);
        }

        free(entriesOld);
    }

    natural const mask = table->mask;
    natural const count = table->count;
    natural const probeMax = table->probeMax;
    struct Entry* const entries = table->entries;
    natural const hash = hashFunction ? hashFunction(key) : key;

    for (natural probe = 1, index = hash & mask; true; ({ probe += 1; index = (index + 1) & mask; })) {
        struct Entry const current = entries[index];
        bool const isEmpty = current.probe == 0;
        bool const isFound = !isEmpty && (equalsFunction ? equalsFunction(key, current.key) : entry->key == current.key);
        bool const needsSwap = current.probe < probe;

        if (shouldInsert && isEmpty) {
            table->count += 1;
            table->probeMax = MAX(table->probeMax, probe);
            entries[index] = *entry;
            entries[index].probe = probe;
            entry->key = 0;
            entry->value = 0;
            entry->extra = 0;
            return index;
        }

        if (shouldInsert && isFound) {
            table->probeMax = MAX(table->probeMax, probe);
            entries[index] = *entry;
            entries[index].probe = probe;
            *entry = current;
            return index;
        }

        if (shouldInsert && needsSwap) {
            entries[index] = *entry;
            entries[index].probe = probe;
            *entry = current;
            probe = current.probe;
            continue;
        }

        if (shouldRemove && isFound) {
            table->count -= 1;
            entries[index].probe = 0;
            entries[index].key = 0;
            entries[index].value = 0;
            entries[index].extra = 0;
            *entry = current;
            return index;
        }

        if (shouldRemove && probe >= probeMax) {
            entry->key = 0;
            entry->value = 0;
            entry->extra = 0;
            break;
        }
    }

    return NATURAL_MAX;
}


// ------------------------------------------------------- Object Methods ------


static var ObjectAllocate(struct Object* self, var super, var command, var options, ...) {
    return calloc(1, self->meta->size);
}


static var ObjectCreate(struct Object* self, var super, var command, var options, ...) {
    var mutable = option("mutable", no);
    var copy = option("copy", null);

    if (copy != null) {
        send(self, "error*", String("InvalidOptionException | Can't create a copy of X"));
        return null;
    }

    struct Meta* meta = self->meta;
    struct Object* object = self;

    if (object->retainCount > 0) {
        object = send(self, "allocate", options(String("mutable"), mutable));
        object->meta = meta;
    }

    object->retainCount = 1;
    object->flags = mutable == yes ? MUTABLE_FLAG : 0;

    return autorelease(object);
}


static var ObjectDestroy(struct Object* self, var super, var command, var options, ...) {
    free(self);
    return null;
}


static var ObjectIsKindOf(struct Object* self, var super, var command, var object, var options, ...) {
    var proto = self;
    do {
        if (proto == object) return yes;
        proto = send(proto, "proto");
    } while (proto != null);
    return no;
}


static var ObjectIsMutable(struct Object* self, var super, var command, var options, ...) {
    return self->flags & MUTABLE_FLAG ? yes : no;
}


static var ObjectRespondsTo(struct Object* self, var super, var command, var commandToCheck, var options, ...) {
    // TODO: optimize.
    var lookedUpSuper = zero;
    void* code = lookup(self, commandToCheck, &lookedUpSuper);
    return Boolean(code != ZERO);
}


static var ObjectAsString(struct Object* self, var super, var command, var options, ...) {
    if (self == Object) return ObjectName;
    // TODO: put in the address of the object.
    return String("<Object XXX>");
}


static var ObjectSelf(struct Object* self, var super, var command, var options, ...) {
    return self;
}


static var ObjectHash(struct Object* self, var super, var command, var options, ...) {
    return Number((natural)self);
}


static var ObjectEquals(struct Object* self, var super, var command, var object, var options, ...) {
    return BooleanMake(self == object);
}


static var ObjectAddMethodBlock(struct Object* self, var super, var command, var method, var block, var options, ...) {
    if (self->meta->owner != self) {
        struct Object* parent = self->meta->owner;

        self->meta = calloc(1, sizeof(struct Meta));
        self->meta->owner = self;
        self->meta->parent = parent;

        TableCreate(&self->meta->cache, parent->meta->cache.count);
        TableCreate(&self->meta->methods, METHODS_DEFAULT_CAPACITY);
        TableCreate(&self->meta->children, CHILDREN_DEFAULT_CAPACITY);

        struct Entry entry = {.key = (natural)self, .value = (natural)yes, .extra = 0};
        TablePut(&parent->meta->children, &entry, ZERO, ZERO);
    }

    struct Entry entry = {.key = (natural)method, .value = (natural)block(block).code, .extra = 0};
    TablePut(&self->meta->methods, &entry, ZERO, ZERO);
    preserve(method);

    // TODO: clear cache of all children.

    return self;
}


static var ObjectRemoveMethod(struct Object* self, var super, var command, var method, var options, ...) {
    // TODO: implement.
    return null;
}


static var ObjectProto(struct Object* self, var super, var command, var options, ...) {
    struct Object* parent = self->meta->parent;
    struct Object* owner = self->meta->owner;
    return owner == self ? parent : owner;
}


static var ObjectSetProto(struct Object* self, var super, var command, var proto, var options, ...) {
    // TODO: implement.
    return null;
}


static var ObjectInfo(struct Object* self, var super, var command, var message, var options, ...) {
    var description = send(message, "as-string");
    fprintf(stderr, "[INFO] %s\n", string(description).characters);
    return self;
}


static var ObjectWarning(struct Object* self, var super, var command, var message, var options, ...) {
    var description = send(message, "as-string");
    fprintf(stderr, "[WARNING] %s\n", string(description).characters);
    return self;
}


static var ObjectError(struct Object* self, var super, var command, var message, var options, ...) {
    throw(message);
    return self;
}


static var ObjectDebug(struct Object* self, var super, var command, var message, var options, ...) {
    var description = send(message, "as-string");
    fprintf(stderr, "[DEBUG] %s\n", string(description).characters);
    return self;
}


// ------------------------------------------------------ Boolean Methods ------


static var BooleanCreate(struct Boolean* self, var super, var command, var options, ...) {
    send(self, "error*", String("InvalidCommandException | Can't create a Boolean"));
    return null;
}


static var BooleanDestroy(struct Boolean* self, var super, var command, var options, ...) {
    send(self, "error*", String("InvalidCommandException | Can't destroy a Boolean"));
    return null;
}


static var BooleanAsString(struct Boolean* self, var super, var command, var options, ...) {
    if (self == Boolean) return BooleanName;
    if (self == yes) return YesAsString;
    if (self == no) return NoAsString;

    send(self, "error*", String("InternalInconsistencyException | Can't return boolean as-string, self is neither yes, nor no, nor Boolean"));
    return null;
}


static var BooleanCompare(struct Boolean* self, var super, var command, var object, var options, ...) {
    if (self == Boolean) return null;
    if (object != yes && object != no) return null;

    if (self == no && object == yes) return Number(-1);
    if (self == yes && object == no) return Number(+1);

    return Number(0);
}


static var BooleanCopy(struct Boolean* self, var super, var command, var options, ...) {
    return self;
}


// ------------------------------------------------------- Number Methods ------


static var NumberCreate(struct Number* self, var super, var command, var options, ...) {
    send(self, "error*", String("InvalidCommandException | Can't create a Number"));
    return null;
}


static var NumberAsString(struct Number* self, var super, var command, var options, ...) {
    if (self == Number) return NumberName;

    // TODO: tweak to not print trailing zeros.
    decimal integerPart = 0;
    decimal fractionalPart = modf(self->number, &integerPart);
    integer const bufferSize = 1024 * 1024;
    char buffer[bufferSize + 1];
    int length = 0;

    if (fractionalPart == 0) {
        length = snprintf(buffer, bufferSize + 1, "%li", (integer)self->number);
    }
    else {
        length = snprintf(buffer, bufferSize + 1, "%f", self->number);
    }

    return autorelease(StringMake(length, buffer));
}


static var NumberHash(struct Number* self, var super, var command, var options, ...) {
    return self;
}


static var NumberEquals(struct Number* self, var super, var command, var object, var options, ...) {
    return DecimalFrom(self) == DecimalFrom(object) ? yes : no;
}


static var NumberCompare(struct Number* self, var super, var command, var object, var options, ...) {
    decimal const number1 = DecimalFrom(self);
    decimal const number2 = DecimalFrom(object);

    if (number1 < number2) return Number(-1);
    if (number1 > number2) return Number(+1);

    return Number(0);
}


static var NumberCopy(struct Number* self, var super, var command, var options, ...) {
    return retain(self);
}


// -------------------------------------------------------- Block Methods ------


static var BlockCreate(struct Block* self, var super, var command, var options, ...) {
    send(self, "error*", String("InvalidCommandException | Can't create a Block"));
    return null;
}


static var BlockAsString(struct Block* self, var super, var command, var options, ...) {
    if (self == Block) return BlockName;

    static struct String* string = ZERO;
    if (string == ZERO) {
        // TODO: put in the address of the block.
        string = preserve(String("<Block XXX>"));
    }
    return string;
}


// --------------------------------------------------------- Data Methods ------


static var DataCreate(struct Data* self, var super, var command, var options, ...) {
    var mutable = option("mutable", no);
    var capacity = option("capacity", Number(1));
    var copy = option("copy", null);

    // TODO: copy if needed.

    self = super(self, "create", String("mutable"), mutable);
    self->capacity = IntegerFrom(capacity);
    self->capacity = MAX(self->capacity, DATA_DEFAULT_CAPACITY);
    self->capacity = RoundUpToPowerOfTwo(self->capacity);
    self->count = 0;
    self->bytes = calloc(self->capacity, 1);

    return self;
}


static var DataDestroy(struct Data* self, var super, var command, var options, ...) {
    free(self->bytes);
    return super(self, "destroy");
}


static var DataAsString(struct Data* self, var super, var command, var options, ...) {
    if (self == Data) return DataName;

    // TODO: implement Base64 encoding.
    char* table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    return String("<Data XXX>");
}


static var DataHash(struct Data* self, var super, var command, var options, ...) {
    natural const digest = Digest(self->count, self->bytes);
    return Number((decimal)digest);
}


static var DataEquals(struct Data* self, var super, var command, var object, var options, ...) {
    if (self == object) return yes;
    if (send(object, "is-kind-of*", Data) == no) return no;

    struct Data* data1 = self;
    struct Data* data2 = object;

    if (data1->count != data2->count) return no;

    integer const result = memcmp(data1->bytes, data2->bytes, data1->count);
    return result == 0 ? yes : no;
}


static var DataCopy(struct Data* self, var super, var command, var options, ...) {
    if (self->capacity < 0) return retain(self);
    // TODO: make an immutable copy.
    return null;
}


static var DataAtCount(struct Data* self, var super, var command, var index, var count, var options, ...) {
    // TODO: implement.
    return null;
}


static var DataReplaceAtCountWith(struct Data* self, var super, var command, var index, var count, var data, var options, ...) {
    // TODO: implement.
    return self;
}


static var DataCount(struct Data* self, var super, var command, var options, ...) {
    return Number(self->count);
}


// -------------------------------------------------------- Array Methods ------


static var ArrayCreate(struct Array* self, var super, var command, var options, ...) {
    var mutable = option("mutable", no);
    var capacity = option("capacity", Number(1));
    var copy = option("copy", null);

    // TODO: copy if needed.

    self = super(self, "create", String("mutable"), mutable);
    self->capacity = IntegerFrom(capacity);
    self->capacity = MAX(self->capacity, ARRAY_DEFAULT_CAPACITY);
    self->capacity = RoundUpToPowerOfTwo(self->capacity);
    self->count = 0;
    self->objects = calloc(self->capacity, sizeof(var));

    return self;
}


static var ArrayDestroy(struct Array* self, var super, var command, var options, ...) {
    for (int i = 0; i < self->count; i += 1) {
        release(self->objects[i]);
    }
    free(self->objects);
    return super(self, "destroy");
}


static var ArrayAsString(struct Array* self, var super, var command, var options, ...) {
    if (self == Array) return ArrayName;
    return String("<Array XXX>");
}


static var ArrayHash(struct Array* self, var super, var command, var options, ...) {
    // TODO: implement.
    return null;
}


static var ArrayEquals(struct Array* self, var super, var command, var object, var options, ...) {
    if (self == object) return yes;
    if (send(object, "is-kind-of*", Array) == no) return no;

    struct Array* array1 = self;
    struct Array* array2 = object;

    if (array1->count != array2->count) return no;

    for (int index = 0; index < array1->count; index += 1) {
        var object1 = array1->objects[index];
        var object2 = array2->objects[index];
        if (send(object1, "equals*", object2) == no) return no;
    }

    return yes;
}


static var ArrayCopy(struct Array* self, var super, var command, var options, ...) {
    var mutable = option(String("mutable"), no);
    // TODO: don't copy if mutable = no and object is immutable
    return send(self, "create", options(String("mutable"), mutable, String("copy"), yes));
}


static var ArrayAt(struct Array* self, var super, var command, var index, var options, ...) {
    integer const integerIndex = IntegerFrom(index);
    integer const integerCount = self->count;

    if (integerIndex < 0 || integerIndex >= integerCount) {
        send(self, "error*", String("RangeException | Can't access object at index X"));
    }

    return self->objects[integerIndex];
}


static var ArrayCount(struct Array* self, var super, var command, var options, ...) {
    return Number(self->count);
}


static var ArrayReplaceAtCountWith(struct Array* self, var super, var command, var index, var count, var objects, var options, ...) {
    integer const integerIndex = IntegerFrom(index);
    integer const integerCount = IntegerFrom(count);

    // Make sure array is mutable:
    if (send(self, "is-mutable") == no) {
        send(self, "error*", String("ImmutableException | Can't replace X objects at index Y with Z, object isn't mutable"));
    }

    // Validate arguments:
    if (integerIndex < 0 || integerIndex > self->count) {
        send(self, "error*", String("RangeException | Can't replace X objects at index Y with Z, index Y is out of range [0, B]"));
    }

    if (integerCount < 0) {
        send(self, "error*", String("RangeException | Can't replace X objects at index Y with Z, count Y out of range [0, ∞]"));
    }

    // Gather information:
    integer const revisedCount = MIN(self->count - integerIndex, integerCount);
    integer const countOfObjects = IntegerFrom(send(objects, "count"));
    integer const requiredCapacity = self->count + countOfObjects - revisedCount;

    // Ensure enough capacity:
    if (self->capacity < requiredCapacity) {
        ArrayEnsureCapacity(self, requiredCapacity);
    }

    // Remove & release objects to be replaced:
    for (integer i = integerIndex; i < integerIndex + revisedCount; i += 1) {
        release(self->objects[i]);
        self->objects[i] = ZERO;
    }

    // Make room for new objects:
    for (integer j = self->count - 1; j >= integerIndex + revisedCount; j -= 1) {
        self->objects[j + countOfObjects - revisedCount] = self->objects[j];
        self->objects[j] = ZERO;
    }

    // Insert & retain new objects:
    for (integer k = 0; k < countOfObjects; k += 1) {
        var const object = send(objects, "at*", Number(k));
        self->objects[k + integerIndex] = retain(object);
    }

    // Update own properties:
    self->count = self->count + countOfObjects - revisedCount;

    // Done.
    return self;
}


// ------------------------------------------------------- String Methods ------


static var StringCreate(struct String* self, var super, var command, var options, ...) {
    var mutable = option("mutable", no);
    var capacity = option("capacity", Number(1));
    var copy = option("copy", null);

    // TODO: copy if needed.

    self = super(self, "create", String("mutable"), mutable);
    self->capacity = IntegerFrom(capacity);
    self->capacity = MAX(self->capacity, STRING_DEFAULT_CAPACITY);
    self->capacity = RoundUpToPowerOfTwo(self->capacity + 1) - 1;
    self->length = 0;
    self->characters = calloc(self->capacity + 1, sizeof(char));

    return self;
}


static var StringDestroy(struct String* self, var super, var command, var options, ...) {
    if (self->capacity < 0) {
        struct Entry entry = {.key = (natural)self, .value = 0, .extra = 0};
        TablePut(&StringTable, &entry, StringHashFunction, StringEqualsFunction);
    }

    free(self->characters);
    return super(self, "destroy");
}


static var StringAsString(struct String* self, var super, var command, var options, ...) {
    if (self == String) return StringName;
    return autorelease(send(self, "copy"));
}


static var StringHash(struct String* self, var super, var command, var options, ...) {
    natural const hashFunction = Digest(self->length, self->characters);
    return Number((decimal)hashFunction);
}


static var StringEquals(struct String* self, var super, var command, var object, var options, ...) {
    if (self == object) return yes;
    if (send(object, "is-kind-of*", String) == no) return no;

    struct String* string1 = self;
    struct String* string2 = object;

    if (string1->length != string2->length) return no;

    integer const result = strncmp(string1->characters, string2->characters, string1->length);
    return result == 0 ? yes : no;
}


static var StringCompare(struct String* self, var super, var command, var object, var options, ...) {
    if (self == object) return yes;
    if (send(object, "is-kind-of*", String) == no) return null;

    struct String* string1 = self;
    struct String* string2 = object;

    integer const result = strncmp(string1->characters, string2->characters, string1->length);
    return Number(result);
}


static var StringCopy(struct String* self, var super, var command, var options, ...) {
    if (self->capacity < 0) return retain(self);
    // TODO: make an immutable copy.
    return null;
}


static var StringAtCount(struct String* self, var super, var command, var index, var count, var options, ...) {
    // TODO: implement.
    return null;
}


static var StringLength(struct String* self, var super, var command, var options, ...) {
    return Number(self->length);
}


static var StringReplaceAtCountWith(struct String* self, var super, var command, var index, var count, var string, var options, ...) {
    // TODO: implement.
    return self;
}


// --------------------------------------------------- Dictionary Methods ------


static var DictionaryCreate(struct Dictionary* self, var super, var command, var options, ...) {
    var mutable = option("mutable", no);
    var capacity = option("capacity", Number(1));
    var copy = option("copy", null);

    // TODO: copy if needed.

    self = super(self, "create", String("mutable"), mutable);
    self->capacity = MAX(IntegerFrom(capacity), DICTIONARY_DEFAULT_CAPACITY);
    self->capacity = RoundUpToPowerOfTwo(self->capacity);
    self->count = 0;
    self->mask = self->capacity - 1;
    self->entries = calloc(self->capacity * 2, sizeof(var));

    return self;
}


static var DictionaryDestroy(struct Dictionary* self, var super, var command, var options, ...) {
    for (int i = 0; i < self->count; i += 2) {
        var key = self->entries[i];
        var value = self->entries[i + 1];

        if (key != zero && key != more) {
            release(key);
            release(value);
        }

        self->entries[i] = zero;
        self->entries[i + 1] = zero;
    }
    free(self->entries);
    return super(self, "destroy");
}


static var DictionaryAsString(struct Dictionary* self, var super, var command, var options, ...) {
    if (self == Dictionary) return DictionaryName;
    // TODO: implement.
    return String("<Dictionary XXX>");
}


static var DictionaryHash(struct Dictionary* self, var super, var command, var options, ...) {
    // TODO: implement.
    return null;
}


static var DictionaryEquals(struct Dictionary* self, var super, var command, var object, var options, ...) {
    if (self == object) return yes;
    if (send(object, "is-kind-of*", Dictionary) == no) return no;

    struct Dictionary* dictionary1 = self;
    struct Dictionary* dictionary2 = object;

    if (dictionary1->count != dictionary2->count) return no;

    for (int index = 0; index < dictionary1->count; index += 1) {
        var key = dictionary1->entries[index * 2];
        if (key != zero) {
            var value1 = dictionary1->entries[index * 2 + 1];
            var value2 = send(dictionary2, "get*", key);
            if (send(value1, "equals*", value2) == no) return no;
        }
    }

    return yes;
}


static var DictionaryCopy(struct Dictionary* self, var super, var command, var options, ...) {
    if (self->capacity < 0) return retain(self);
    // TODO: make an immutable copy.
    return null;
}


static var DictionaryGet(struct Dictionary* self, var super, var command, var key, var options, ...) {
    natural const mask = self->mask;
    natural const hash = NaturalFrom(send(key, "hash"));
    natural index = hash & mask;

    for (int i = 0; i <= mask; i += 1) {
        integer const indexOfKey = index * 2;
        integer const indexOfValue = indexOfKey + 1;
        var const keyAtIndex = self->entries[indexOfKey];
        var const valueAtIndex = self->entries[indexOfValue];

        if (keyAtIndex == more) {
            continue;
        }

        if (keyAtIndex == zero) {
            return null;
        }

        if (send(keyAtIndex, "equals*", key) == yes) {
            return valueAtIndex;
        }

        index = (index + 1) & mask;
    }

    return null;
}


static var DictionarySetTo(struct Dictionary* self, var super, var command, var key, var value, var options, ...) {
    DictionaryEnsureCapacity(self, self->count + 1);

    natural const mask = self->mask;
    natural const hash = NaturalFrom(send(key, "hash"));
    natural index = hash & mask;

    for (int i = 0; i <= mask; i += 1) {
        integer const indexOfKey = index * 2;
        integer const indexOfValue = indexOfKey + 1;
        var const keyAtIndex = self->entries[indexOfKey];

        if (keyAtIndex == zero || keyAtIndex == more) {
            retain(key);
            retain(value);
            self->entries[indexOfKey] = key;
            self->entries[indexOfValue] = value;
            self->count += 1;
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


static var DictionaryRemove(struct Dictionary* self, var super, var command, var key, var options, ...) {
    natural const mask = self->mask;
    natural const hash = NaturalFrom(send(key, "hash"));
    natural index = hash & mask;

    for (int i = 0; i <= mask; i += 1) {
        integer const indexOfKey = index * 2;
        integer const indexOfValue = indexOfKey + 1;
        var const keyAtIndex = self->entries[indexOfKey];
        var const valueAtIndex = self->entries[indexOfValue];

        if (keyAtIndex == more) {
            continue;
        }

        if (keyAtIndex == zero) {
            return self;
        }

        if (send(keyAtIndex, "equals*", key) == yes) {
            release(keyAtIndex);
            release(valueAtIndex);

            self->entries[indexOfKey] = more;
            self->entries[indexOfValue] = zero;

            return self;
        }

        index = (index + 1) & mask;
    }

    return self;
}


static var DictionaryCount(struct Dictionary* self, var super, var command, var options, ...) {
    return Number(self->count);
}


// --------------------------------------------------------- Null Methods ------


static var NullCreate(struct Object* self, var super, var command, var options, ...) {
    send(self, "error*", String("InvalidCommandException | Can't create a Null"));
    return null;
}


static var NullDestroy(struct Object* self, var super, var command, var options, ...) {
    send(self, "error*", String("InvalidCommandException | Can't destroy a Null"));
    return null;
}


static var NullAsString(struct Object* self, var super, var command, var options, ...) {
    return NullName;
}


static var NullEquals(struct Object* self, var super, var command, var object, var options, ...) {
    return object == null ? yes : no;
}


static var NullCopy(struct Object* self, var super, var command, var options, ...) {
    return retain(self);
}


// ---------------------------------------------------- Factory Functions ------


var BooleanMake(long boolean) {
    return boolean ? yes : no;
}


var NumberMake(double value) {
    struct Number* number = calloc(1, sizeof(struct Number));
    number->meta = &NumberMeta;
    number->retainCount = 1;
    number->number = value;
    return number;
}


var BlockMake(void* code) {
    assert(code != ZERO, "When making a block, code must be != ZERO");
    struct Block* block = calloc(1, sizeof(struct Block));
    block->meta = &BlockMeta;
    block->retainCount = 1;
    block->code = code;
    return block;
}


var DataMake(long count, const void* bytes) {
    assert(count >= 0, "When making a data object, count must be >= 0");
    struct Data* data = calloc(1, sizeof(struct Data));
    data->meta = &DataMeta;
    data->retainCount = 1;
    data->capacity = -1;
    data->count = count;
    data->bytes = calloc(count, 1);
    memcpy(data->bytes, bytes, count);
    return data;
}


var ArrayMake(long count, ...) {
    assert(count >= 0, "When making an array, count must be >= 0");

    // Create array:
    struct Array* array = calloc(1, sizeof(struct Array));
    array->meta = &ArrayMeta;
    array->retainCount = 1;
    array->capacity = -1;
    array->count = count;
    array->objects = calloc(count, sizeof(var));

    // Collect objects:
    va_list arguments;
    va_start(arguments, count);
    for (int i = 0; i < count; i += 1) {
        var object = va_arg(arguments, var);
        array->objects[i] = object;

        // TODO: retain here instaed of the extra pass below.

    }

    // Make mutable if needed:
    if (count > 0 && array->objects[count - 1] == more) {
        array->flags |= MUTABLE_FLAG;
        array->objects[count - 1] = ZERO;
        array->count = count - 1;
        array->capacity = count;
        ArrayEnsureCapacity(array, count);
    }

    // Retain objects:
    for (int i = 0; i < array->count; i += 1) {
        retain(array->objects[i]);
    }

    // Done:
    va_end(arguments);
    return array;
}


var StringMake(long length, const char* characters) {
    assert(length >= 0, "When making a string, length must be >= 0");

    natural const hash = Digest(length, characters);
    bool const couldBeCommandOrKey = length <= MAX_KEY_AND_COMMAND_LENGTH;

    if (couldBeCommandOrKey) {
        struct String proxy = {.meta = &StringMeta, .retainCount = RETAIN_COUNT_MAX, .flags = 0, .capacity = -1, .length = length, .hash = hash, .characters = (char*)characters};
        struct Entry entry = {.key = (natural)&proxy, .value = 0, .extra = 0};

        TableGet(&StringTable, &entry, StringHashFunction, StringEqualsFunction);
        struct String* string = (struct String*)entry.value;

        if (string != ZERO) return retain(string);
    }

    struct String* string = calloc(1, sizeof(struct String));
    string->meta = &StringMeta;
    string->retainCount = 1;
    string->capacity = -1;
    string->length = length;
    string->hash = hash;
    string->characters = calloc(length + 1, sizeof(char));
    strncpy(string->characters, characters, length);

    if (couldBeCommandOrKey) {
        struct Entry entry = {.key = (natural)string, .value = (natural)string, .extra = 0};
        TablePut(&StringTable, &entry, StringHashFunction, StringEqualsFunction);
    }

    return string;
}


var DictionaryMake(long count, ...) {
    assert(count >= 0, "When making a dictionary, count must be >= 0");

    struct Dictionary* dictionary = calloc(1, sizeof(struct Dictionary));
    dictionary->meta = &DictionaryMeta;
    dictionary->flags = 0;
    dictionary->retainCount = 1;
    dictionary->capacity = 0;
    dictionary->count = 0;
    dictionary->mask = 0;

    DictionaryEnsureCapacity(dictionary, count);

    va_list arguments;
    va_start(arguments, count);

    for (int i = 0; i < count - 1; i += 2) {
        var key = va_arg(arguments, var);
        var value = va_arg(arguments, var);
        DictionarySetTo(dictionary, NULL, null, key, value, zero);
    }

    // Make mutable if needed:
    if (va_arg(arguments, var) == more) {
        dictionary->flags = MUTABLE_FLAG;
    }

    // Done:
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
    // TODO: remove all child collect blocks from the stack
    // and release its objects if landed here through exception.
    assert(collectBlock == CollectBlockTop, "When popping a collect block, the top-most one must be the same as the one passed in");
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
    assert(tryCatchBlock == TryCatchBlockTop, "When popping a try-catch block, the top-most one must be the same as the one passed in");
    struct TryCatchBlock* tryCatchBlockToPop = tryCatchBlock;
    TryCatchBlockTop = tryCatchBlockToPop->previousTryCatchBlock;
    release(tryCatchBlockToPop->exception);
    free(tryCatchBlockToPop);
    return ZERO;
}


void* TryCatchBlockTry(void* tryCatchBlock) {
    assert(tryCatchBlock == TryCatchBlockTop, "When beginning a try block, the top-most try-catch block must be the same as the one passed in");
    return TryCatchBlockTop->destination;
}


var TryCatchBlockCatch(void* tryCatchBlock) {
    assert(tryCatchBlock == TryCatchBlockTop, "When beginning a catch block, the top-most try-catch block must be the same as the one passed in");
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


var autorelease(var object) {
    if (object(object).retainCount >= RETAIN_COUNT_MAX) return object;
    if (CollectBlockTop == ZERO) {
        fprintf(stderr, "[WARNING] No collect block found, leaking ...\n");
        return object;
    }

    integer const capacity = CollectBlockTop->capacity;
    integer const count = CollectBlockTop->count;

    if (count >= capacity) {
        assert(capacity > 0, "When increasing the capacity of a collect pool, capacity must be > 0");
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


var preserve(var object) {
    object(object).retainCount = RETAIN_COUNT_MAX;
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


void* lookup(var object, var command, var* super) {
    struct Meta* const meta = object(object).meta;
    struct Table* const cache = &meta->cache;
    struct Table* const methods = &meta->methods;
    struct Object* const parent = meta->parent;
    void* code = ZERO;

    // TODO: look up in cache, old implementation:
    // void* code = HashMapGet(cache, key, ZERO, ZERO);
    // if (code != ZERO) return code;

    // Look up in own methods:
    assert(string(command).length <= MAX_KEY_AND_COMMAND_LENGTH, "When looking up a method for a given command, the length of the command must be <= MAX_KEY_AND_COMMAND_LENGTH");
    struct Entry entry = {.key = (natural)command, .value = 0, .extra = 0};
    TableGet(methods, &entry, ZERO, ZERO);

    code = (void *)entry.value;
    if (code) *super = parent;

    // Continue searching in parent's methods if needed:
    if (!code && parent != null) {
        code = lookup(parent, command, super);
    }

    // TOOD: add fallback.

    // Make sure method was found:
    assert(code != ZERO, "At this point, code must be either the found method or a fallback method but should never be ZERO");

    // TODO: Cache found method, old implementation:
    // HashMapPut(cache, key, code, ZERO, ZERO);
    // preserve(command);

    // Return found & now cached method:
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
        var description = send(exception, "as-string");
        fprintf(stderr, "[ERROR] %s\n", string(description).characters);
    }

    tryCatchBlock->exception = exception;
    tryCatchBlock->thrown = true;

    longjmp(tryCatchBlock->destination, 1);
}


void inspect(var object) {
    collect {
        var description = send(object, "as-string");
        fprintf(stderr, "%s\n", string(description).characters);
    }
}


// -------------------------------------------------------- Bootstrapping ------


static void bootstrap Metal() {
    collect {
        TableCreate(&StringTable, STRING_TABLE_DEFAULT_CAPACITY);

        ObjectMeta.owner = &ObjectState;
        BooleanMeta.owner = &BooleanState;
        NumberMeta.owner = &NumberState;
        BlockMeta.owner = &BlockState;
        DataMeta.owner = &DataState;
        ArrayMeta.owner = &ArrayState;
        StringMeta.owner = &StringState;
        DictionaryMeta.owner = &DictionaryState;
        NullMeta.owner = &NullState;

        ObjectMeta.parent = null;
        BooleanMeta.parent = &ObjectState;
        NumberMeta.parent = &ObjectState;
        BlockMeta.parent = &ObjectState;
        DataMeta.parent = &ObjectState;
        ArrayMeta.parent = &ObjectState;
        StringMeta.parent = &ObjectState;
        DictionaryMeta.parent = &ObjectState;
        NullMeta.parent = &ObjectState;

        ObjectMeta.size = sizeof(struct Object);
        BooleanMeta.size = sizeof(struct Boolean);
        NumberMeta.size = sizeof(struct Number);
        BlockMeta.size = sizeof(struct Block);
        DataMeta.size = sizeof(struct Data);
        ArrayMeta.size = sizeof(struct Array);
        StringMeta.size = sizeof(struct String);
        DictionaryMeta.size = sizeof(struct Dictionary);
        NullMeta.size = sizeof(struct Object);

        TableCreate(&ObjectMeta.cache, CACHE_DEFAULT_CAPACITY);
        TableCreate(&BooleanMeta.cache, CACHE_DEFAULT_CAPACITY);
        TableCreate(&NumberMeta.cache, CACHE_DEFAULT_CAPACITY);
        TableCreate(&BlockMeta.cache, CACHE_DEFAULT_CAPACITY);
        TableCreate(&DataMeta.cache, CACHE_DEFAULT_CAPACITY);
        TableCreate(&ArrayMeta.cache, CACHE_DEFAULT_CAPACITY);
        TableCreate(&StringMeta.cache, CACHE_DEFAULT_CAPACITY);
        TableCreate(&DictionaryMeta.cache, CACHE_DEFAULT_CAPACITY);
        TableCreate(&NullMeta.cache, CACHE_DEFAULT_CAPACITY);

        TableCreate(&ObjectMeta.methods, METHODS_DEFAULT_CAPACITY);
        TableCreate(&BooleanMeta.methods, METHODS_DEFAULT_CAPACITY);
        TableCreate(&NumberMeta.methods, METHODS_DEFAULT_CAPACITY);
        TableCreate(&BlockMeta.methods, METHODS_DEFAULT_CAPACITY);
        TableCreate(&DataMeta.methods, METHODS_DEFAULT_CAPACITY);
        TableCreate(&ArrayMeta.methods, METHODS_DEFAULT_CAPACITY);
        TableCreate(&StringMeta.methods, METHODS_DEFAULT_CAPACITY);
        TableCreate(&DictionaryMeta.methods, METHODS_DEFAULT_CAPACITY);
        TableCreate(&NullMeta.methods, METHODS_DEFAULT_CAPACITY);

        ObjectAddMethodBlock(Object, zero, zero, String("allocate"), Block(ObjectAllocate), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("create"), Block(ObjectCreate), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("destroy"), Block(ObjectDestroy), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("is-kind-of*"), Block(ObjectIsKindOf), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("is-mutable"), Block(ObjectIsMutable), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("responds-to*"), Block(ObjectRespondsTo), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("as-string"), Block(ObjectAsString), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("self"), Block(ObjectSelf), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("hash"), Block(ObjectHash), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("equals*"), Block(ObjectEquals), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("add-method*block*"), Block(ObjectAddMethodBlock), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("remove-method*"), Block(ObjectRemoveMethod), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("proto"), Block(ObjectProto), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("set-proto*"), Block(ObjectSetProto), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("info*"), Block(ObjectInfo), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("warning*"), Block(ObjectWarning), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("error*"), Block(ObjectError), zero);
        ObjectAddMethodBlock(Object, zero, zero, String("debug*"), Block(ObjectDebug), zero);

        ObjectAddMethodBlock(Boolean, Object, zero, String("create"), Block(BooleanCreate), zero);
        ObjectAddMethodBlock(Boolean, Object, zero, String("destroy"), Block(BooleanDestroy), zero);
        ObjectAddMethodBlock(Boolean, Object, zero, String("as-string"), Block(BooleanAsString), zero);
        ObjectAddMethodBlock(Boolean, Object, zero, String("compare*"), Block(BooleanCompare), zero);
        ObjectAddMethodBlock(Boolean, Object, zero, String("copy"), Block(BooleanCopy), zero);

        ObjectAddMethodBlock(Number, Object, zero, String("create"), Block(NumberCreate), zero);
        ObjectAddMethodBlock(Number, Object, zero, String("as-string"), Block(NumberAsString), zero);
        ObjectAddMethodBlock(Number, Object, zero, String("hash"), Block(NumberHash), zero);
        ObjectAddMethodBlock(Number, Object, zero, String("equals*"), Block(NumberEquals), zero);
        ObjectAddMethodBlock(Number, Object, zero, String("compare*"), Block(NumberCompare), zero);
        ObjectAddMethodBlock(Number, Object, zero, String("copy"), Block(NumberCopy), zero);

        ObjectAddMethodBlock(Block, Object, zero, String("create"), Block(BlockCreate), zero);
        ObjectAddMethodBlock(Block, Object, zero, String("as-string"), Block(BlockAsString), zero);

        ObjectAddMethodBlock(Data, Object, zero, String("create"), Block(DataCreate), zero);
        ObjectAddMethodBlock(Data, Object, zero, String("destroy"), Block(DataDestroy), zero);
        ObjectAddMethodBlock(Data, Object, zero, String("as-string"), Block(DataAsString), zero);
        ObjectAddMethodBlock(Data, Object, zero, String("hash"), Block(DataHash), zero);
        ObjectAddMethodBlock(Data, Object, zero, String("equals*"), Block(DataEquals), zero);
        ObjectAddMethodBlock(Data, Object, zero, String("copy"), Block(DataCopy), zero);
        ObjectAddMethodBlock(Data, Object, zero, String("at*count*"), Block(DataAtCount), zero);
        ObjectAddMethodBlock(Data, Object, zero, String("replace-at*count*with*"), Block(DataReplaceAtCountWith), zero);
        ObjectAddMethodBlock(Data, Object, zero, String("count"), Block(DataCount), zero);

        ObjectAddMethodBlock(Array, Object, zero, String("create"), Block(ArrayCreate), zero);
        ObjectAddMethodBlock(Array, Object, zero, String("destroy"), Block(ArrayDestroy), zero);
        ObjectAddMethodBlock(Array, Object, zero, String("as-string"), Block(ArrayAsString), zero);
        ObjectAddMethodBlock(Array, Object, zero, String("hash"), Block(ArrayHash), zero);
        ObjectAddMethodBlock(Array, Object, zero, String("equals*"), Block(ArrayEquals), zero);
        ObjectAddMethodBlock(Array, Object, zero, String("copy"), Block(ArrayCopy), zero);
        ObjectAddMethodBlock(Array, Object, zero, String("at*"), Block(ArrayAt), zero);
        ObjectAddMethodBlock(Array, Object, zero, String("count"), Block(ArrayCount), zero);
        ObjectAddMethodBlock(Array, Object, zero, String("replace-at*count*with*"), Block(ArrayReplaceAtCountWith), zero);

        ObjectAddMethodBlock(String, Object, zero, String("create"), Block(StringCreate), zero);
        ObjectAddMethodBlock(String, Object, zero, String("destroy"), Block(StringDestroy), zero);
        ObjectAddMethodBlock(String, Object, zero, String("as-string"), Block(StringAsString), zero);
        ObjectAddMethodBlock(String, Object, zero, String("hash"), Block(StringHash), zero);
        ObjectAddMethodBlock(String, Object, zero, String("equals*"), Block(StringEquals), zero);
        ObjectAddMethodBlock(String, Object, zero, String("compare*"), Block(StringCompare), zero);
        ObjectAddMethodBlock(String, Object, zero, String("copy"), Block(StringCopy), zero);
        ObjectAddMethodBlock(String, Object, zero, String("at*count*"), Block(StringAtCount), zero);
        ObjectAddMethodBlock(String, Object, zero, String("length"), Block(StringLength), zero);
        ObjectAddMethodBlock(String, Object, zero, String("replace-at*count*with*"), Block(StringReplaceAtCountWith), zero);

        ObjectAddMethodBlock(Dictionary, Object, zero, String("create"), Block(DictionaryCreate), zero);
        ObjectAddMethodBlock(Dictionary, Object, zero, String("destroy"), Block(DictionaryDestroy), zero);
        ObjectAddMethodBlock(Dictionary, Object, zero, String("as-string"), Block(DictionaryAsString), zero);
        ObjectAddMethodBlock(Dictionary, Object, zero, String("hash"), Block(DictionaryHash), zero);
        ObjectAddMethodBlock(Dictionary, Object, zero, String("equals*"), Block(DictionaryEquals), zero);
        ObjectAddMethodBlock(Dictionary, Object, zero, String("copy"), Block(DictionaryCopy), zero);
        ObjectAddMethodBlock(Dictionary, Object, zero, String("get*"), Block(DictionaryGet), zero);
        ObjectAddMethodBlock(Dictionary, Object, zero, String("set*to*"), Block(DictionarySetTo), zero);
        ObjectAddMethodBlock(Dictionary, Object, zero, String("remove*"), Block(DictionaryRemove), zero);
        ObjectAddMethodBlock(Dictionary, Object, zero, String("count"), Block(DictionaryCount), zero);

        ObjectAddMethodBlock(null, Object, zero, String("create"), Block(NullCreate), zero);
        ObjectAddMethodBlock(null, Object, zero, String("destroy"), Block(NullDestroy), zero);
        ObjectAddMethodBlock(null, Object, zero, String("as-string"), Block(NullAsString), zero);
        ObjectAddMethodBlock(null, Object, zero, String("equals*"), Block(NullEquals), zero);
        ObjectAddMethodBlock(null, Object, zero, String("copy"), Block(NullCopy), zero);

        TableCreate(&ObjectMeta.children, 64);
        TableCreate(&BooleanMeta.children, 1);
        TableCreate(&NumberMeta.children, 1);
        TableCreate(&BlockMeta.children, 1);
        TableCreate(&DataMeta.children, 1);
        TableCreate(&ArrayMeta.children, 1);
        TableCreate(&StringMeta.children, 1);
        TableCreate(&DictionaryMeta.children, 1);
        TableCreate(&NullMeta.children, 1);

        struct Entry BooleanEntry = {.key = (natural)Boolean, .value = (natural)yes, .extra = 0};
        struct Entry NumberEntry = {.key = (natural)Number, .value = (natural)yes, .extra = 0};
        struct Entry BlockEntry = {.key = (natural)Block, .value = (natural)yes, .extra = 0};
        struct Entry DataEntry = {.key = (natural)Data, .value = (natural)yes, .extra = 0};
        struct Entry ArrayEntry = {.key = (natural)Array, .value = (natural)yes, .extra = 0};
        struct Entry StringEntry = {.key = (natural)String, .value = (natural)yes, .extra = 0};
        struct Entry DictionaryEntry = {.key = (natural)Dictionary, .value = (natural)yes, .extra = 0};
        struct Entry NullEntry = {.key = (natural)null, .value = (natural)yes, .extra = 0};

        TablePut(&ObjectMeta.children, &BooleanEntry, ZERO, ZERO);
        TablePut(&ObjectMeta.children, &NumberEntry, ZERO, ZERO);
        TablePut(&ObjectMeta.children, &BlockEntry, ZERO, ZERO);
        TablePut(&ObjectMeta.children, &DataEntry, ZERO, ZERO);
        TablePut(&ObjectMeta.children, &ArrayEntry, ZERO, ZERO);
        TablePut(&ObjectMeta.children, &StringEntry, ZERO, ZERO);
        TablePut(&ObjectMeta.children, &DictionaryEntry, ZERO, ZERO);
        TablePut(&ObjectMeta.children, &NullEntry, ZERO, ZERO);

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

        NoAsString = preserve(String("no"));
        YesAsString = preserve(String("yes"));

        InvalidArgumentException = preserve(String("InvalidArgumentException"));
        InternalInconsistencyException = preserve(String("InternalInconsistencyException"));
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
        if (key) DictionarySetTo(dictionary, ZERO, zero, key, value, zero);
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


static inline natural StringHashFunction(natural key) {
    struct String* string = (struct String*)key;

    // TODO: is testing for ZERO or MORE really needed here?
    if (string == ZERO) return 0;
    if (string == MORE) return NATURAL_MAX;

    return string(string).hash;
}


static inline bool StringEqualsFunction(natural key1, natural key2) {
    struct String* string1 = (struct String*)key1;
    struct String* string2 = (struct String*)key2;
    if (string1 == string2) return true;

    // TODO: is testing for ZERO or MORE really needed here?
    if (string1 == ZERO || string2 == ZERO) return false;
    if (string1 == MORE || string2 == MORE) return false;

    if (string(string1).length != string(string2).length) return false;
    return strncmp(string(string1).characters, string(string2).characters, string(string1).length) == 0;
}
