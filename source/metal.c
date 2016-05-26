//
// Copyright (c) 2014 Konstantin Bender.
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

// --------------------------------------------------------------- Macros ------

#define MLMax(value1, value2) ((value1) > (value2) ? (value1) : (value2))
#define MLMin(value1, value2) ((value1) < (value2) ? (value1) : (value2))

#define table(pointer) (*((struct MLTable*)pointer))
#define meta(pointer) (*((struct MLMeta*)pointer))
#define object(pointer) (*((struct MLObject*)pointer))
#define boolean(pointer) (*((struct MLBoolean*)pointer))
#define number(pointer) (*((struct MLNumber*)pointer))
#define block(pointer) (*((struct MLBlock*)pointer))
#define data(pointer) (*((struct MLData*)pointer))
#define array(pointer) (*((struct MLArray*)pointer))
#define string(pointer) (*((struct MLString*)pointer))
#define dictionary(pointer) (*((struct MLDictionary*)pointer))
#define exception(pointer) (*((struct MLException*)pointer))

#define MLBootstrap __attribute__((constructor(128)))
#define MLAssert(condition, message, args...) if (!(condition)) { fprintf(stderr, "[ERROR] Assertion failure in function %s in file %s line %d: " message, __FUNCTION__, __FILE__, __LINE__, ## args); int* pointer = NULL; *pointer = 0; }

// ------------------------------------------------------------ Constants ------

static MLInteger const MLDataDefaultCapacity = 16;
static MLInteger const MLArrayDefaultCapacity = 16;
static MLInteger const MLStringDefaultCapacity = 15; // +1 for '\0'
static MLInteger const MLDictionaryDefaultCapacity = 8;
static MLInteger const MLCacheDefaultCapacity = 32;
static MLInteger const MLChildrenDefaultCapacity = 8;
static MLInteger const MLMethodsDefaultCapacity = 8;
static MLInteger const MLCollectBlockDefaultCapacity = 2048;
static MLInteger const MLSymbolTableBlockDefaultCapacity = 2048;
static MLInteger const MLStringTableBlockDefaultCapacity = 2048;
static MLInteger const MLMaxKeyAndCommandLength = 2048;

static MLNatural const MLFlagBits = 0x3;
static MLNatural const MLFlagBitsCount = 2;
static MLNatural const MLMutableFlag = 1 << 0;
static MLNatural const MLInlineFlag = 1 << 1;

static MLNatural const MLRetainCountOne = MLFlagBits + 1;
static MLNatural const MLRetainCountMax = MLNaturalMax & ~MLFlagBits;

// ----------------------------------------------------------- Structures ------

struct MLEntry {
    MLNatural probe;
    MLNatural key;
    MLNatural value;
    MLNatural extra;
};

struct MLTable {
    MLNatural mask;
    MLNatural count;
    MLNatural probeMax;
    struct MLEntry* entries;
};

struct MLMeta {
    MLVariable owner;
    MLVariable parent;
    MLNatural size;
    struct MLTable cache;
    struct MLTable methods;
    struct MLTable children;
};

struct MLObject {
    struct MLMeta* meta;
    MLNatural retainCountAndFlags;
};

struct MLBoolean {
    struct MLMeta* meta;
    MLNatural retainCountAndFlags;
};

struct MLNumber {
    struct MLMeta* meta;
    MLNatural retainCountAndFlags;
    double number;
};

struct MLBlock {
    struct MLMeta* meta;
    MLNatural retainCountAndFlags;
    void* code;
};

struct MLData {
    struct MLMeta* meta;
    MLNatural retainCountAndFlags;
    MLInteger capacity;
    MLInteger count;
    MLNatural hash;
    void* bytes;
};

struct MLArray {
    struct MLMeta* meta;
    MLNatural retainCountAndFlags;
    MLInteger capacity;
    MLInteger count;
    MLNatural hash;
    MLVariable* objects;
};

struct MLString {
    struct MLMeta* meta;
    MLNatural retainCountAndFlags;
    MLInteger capacity;
    MLInteger length;
    MLNatural hash;
    char* characters;
};

struct MLDictionary {
    struct MLMeta* meta;
    MLNatural retainCountAndFlags;
    MLInteger capacity;
    MLInteger count;
    MLNatural mask;
    MLNatural hash;
    MLVariable* entries;
};

struct MLException {
    struct MLMeta* meta;
    MLNatural retainCountAndFlags;
    MLVariable name;
    MLVariable reason;
    MLVariable info;
};

struct MLCollectBlock {
    struct MLCollectBlock* previousCollectBlock;
    MLInteger capacity;
    MLInteger count;
    MLVariable* objects;
};

struct MLPerformHandleBlock {
    struct MLPerformHandleBlock* previousPerformHandleBlock;
    jmp_buf destination;
    MLVariable exception;
    bool raised;
};

// ---------------------------------------------------------------- Types ------

typedef MLNatural (*MLHashFunction)(MLNatural);
typedef bool (*MLEqualsFunction)(MLNatural, MLNatural);

// ------------------------------------------------------------ Variables ------

static struct MLMeta MLObjectMeta;
static struct MLMeta MLBooleanMeta;
static struct MLMeta MLNumberMeta;
static struct MLMeta MLBlockMeta;
static struct MLMeta MLDataMeta;
static struct MLMeta MLArrayMeta;
static struct MLMeta MLStringMeta;
static struct MLMeta MLDictionaryMeta;
static struct MLMeta MLExceptionMeta;
static struct MLMeta MLNullMeta;

static struct MLObject MLObjectState = {.meta = &MLObjectMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLBoolean MLBooleanState = {.meta = &MLBooleanMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLNumber MLNumberState = {.meta = &MLNumberMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLBlock MLBlockState = {.meta = &MLBlockMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLData MLDataState = {.meta = &MLDataMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLArray MLArrayState = {.meta = &MLArrayMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLString MLStringState = {.meta = &MLStringMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLDictionary MLDictionaryState = {.meta = &MLDictionaryMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLException MLExceptionState = {.meta = &MLExceptionMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLObject MLNullState = {.meta = &MLNullMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLBoolean MLYesState = {.meta = &MLBooleanMeta, .retainCountAndFlags = MLRetainCountMax};
static struct MLBoolean MLNoState = {.meta = &MLBooleanMeta, .retainCountAndFlags = MLRetainCountMax};

MLVariable const MLObject = &MLObjectState;
MLVariable const MLBoolean = &MLBooleanState;
MLVariable const MLNumber = &MLNumberState;
MLVariable const MLBlock = &MLBlockState;
MLVariable const MLData = &MLDataState;
MLVariable const MLArray = &MLArrayState;
MLVariable const MLString = &MLStringState;
MLVariable const MLDictionary = &MLDictionaryState;
MLVariable const MLException = &MLExceptionState;
MLVariable const MLNull = &MLNullState;
MLVariable const MLYes = &MLYesState;
MLVariable const MLNo = &MLNoState;

static struct MLCollectBlock* MLCollectBlockTop = MLZero;
static struct MLPerformHandleBlock* MLPerformHandleBlockTop = MLZero;

static struct MLTable MLStringTable = {.mask = 0, .count = 0, .probeMax = 0, .entries = MLZero};

static struct MLString* MLObjectClassName = MLZero;
static struct MLString* MLBooleanClassName = MLZero;
static struct MLString* MLNumberClassName = MLZero;
static struct MLString* MLBlockClassName = MLZero;
static struct MLString* MLDataClassName = MLZero;
static struct MLString* MLArrayClassName = MLZero;
static struct MLString* MLStringClassName = MLZero;
static struct MLString* MLDictionaryClassName = MLZero;
static struct MLString* MLExceptionClassName = MLZero;
static struct MLString* MLNullClassName = MLZero;
static struct MLString* MLNoAsString = MLZero;
static struct MLString* MLYesAsString = MLZero;

static struct MLString* MLInvalidArgumentException = MLZero;
static struct MLString* MLInternalInconsistencyException = MLZero;

// ---------------------------------------------------- Helper Functions -------

static void MLDataEnsureCapacity(struct MLData* data, MLInteger requiredCapacity);
static void MLArrayEnsureCapacity(struct MLArray* array, MLInteger requiredCapacity);
static void MLStringEnsureCapacity(struct MLString* string, MLInteger requiredCapacity);
static void MLDictionaryEnsureCapacity(struct MLDictionary* dictionary, MLInteger requiredCapacity);
static inline MLNatural MLRoundUpToPowerOfTwo(MLNatural number);
static inline MLNatural MLRoundDownToPowerOfTwo(MLNatural number);
static inline MLNatural MLStringHashFunction(MLNatural key);
static inline bool MLStringEqualsFunction(MLNatural key1, MLNatural key2);
static MLNatural MLDigest(MLInteger count, const void* bytes);

// ------------------------------------------------- Hash Table Functions ------

static inline struct MLTable* MLTableCreate(struct MLTable* table, MLNatural capacity) {
    capacity = MLRoundUpToPowerOfTwo(capacity);
    table->mask = capacity - 1;
    table->count = 0;
    table->probeMax = 0;
    table->entries = calloc(capacity, sizeof(struct MLEntry));
    return table;
}

static inline struct MLTable* MLTableDestroy(struct MLTable* table) {
    free(table->entries);
    memset(table, 0, sizeof(struct MLTable));
    return table;
}

static inline MLNatural MLTableGet(struct MLTable* table, struct MLEntry* entry, MLHashFunction hashFunction, MLEqualsFunction equalsFunction) {
    MLNatural const key = entry->key;
    MLNatural const mask = table->mask;
    MLNatural const count = table->count;
    MLNatural const probeMax = table->probeMax;
    struct MLEntry* const entries = table->entries;
    MLNatural const hash = hashFunction ? hashFunction(key) : key;

    for (MLNatural probe = 1, index = hash & mask; probe <= probeMax; ({ probe += 1; index = (index + 1) & mask; })) {
        struct MLEntry const current = entries[index];
        bool const isEmpty = current.probe == 0;
        bool const isFound = !isEmpty && (equalsFunction ? equalsFunction(key, current.key) : key == current.key);

        if (isFound) {
            *entry = current;
            return index;
        }
    }

    return MLNaturalMax;
}

static inline MLNatural MLTablePut(struct MLTable* table, struct MLEntry* entry, MLHashFunction hashFunction, MLEqualsFunction equalsFunction) {
    MLNatural const key = entry->key;
    MLNatural const value = entry->value;

    bool const shouldInsert = value != 0;
    bool const shouldRemove = value == 0;

    MLNatural const quarterOfCapacity = (table->mask + 1) >> 2;
    bool const isNearlyFull = table->count > quarterOfCapacity + (quarterOfCapacity << 1);
    bool const isNearlyEmpty = table->count < quarterOfCapacity;
    bool const shouldExpand = shouldInsert & isNearlyFull;
    bool const shouldContract = shouldRemove & isNearlyEmpty;

    if (shouldExpand) {
        MLNatural const capacityOld = table->mask + 1;
        MLNatural const capacityNew = capacityOld << 1;

        struct MLEntry* const entriesOld = table->entries;
        struct MLEntry* const entriesNew = calloc(capacityNew, sizeof(struct MLEntry));

        table->mask = capacityNew - 1;
        table->count = 0;
        table->probeMax = 0;
        table->entries = entriesNew;

        for (MLNatural index = 0; index < capacityOld; index += 1) {
            struct MLEntry current = entriesOld[index];
            if (current.probe == 0) continue;
            MLTablePut(table, &current, hashFunction, equalsFunction);
        }

        free(entriesOld);
    }

    if (shouldContract) {
        MLNatural const capacityOld = table->mask + 1;
        MLNatural const capacityNew = capacityOld >> 1;

        struct MLEntry* const entriesOld = table->entries;
        struct MLEntry* const entriesNew = calloc(capacityNew, sizeof(struct MLEntry));

        table->mask = capacityNew - 1;
        table->count = 0;
        table->probeMax = 0;
        table->entries = entriesNew;

        for (MLNatural index = 0; index < capacityOld; index += 1) {
            struct MLEntry current = entriesOld[index];
            if (current.probe == 0) continue;
            MLTablePut(table, &current, hashFunction, equalsFunction);
        }

        free(entriesOld);
    }

    MLNatural const mask = table->mask;
    MLNatural const count = table->count;
    MLNatural const probeMax = table->probeMax;
    struct MLEntry* const entries = table->entries;
    MLNatural const hash = hashFunction ? hashFunction(key) : key;

    for (MLNatural probe = 1, index = hash & mask; true; ({ probe += 1; index = (index + 1) & mask; })) {
        struct MLEntry const current = entries[index];
        bool const isEmpty = current.probe == 0;
        bool const isFound = !isEmpty && (equalsFunction ? equalsFunction(key, current.key) : entry->key == current.key);
        bool const needsSwap = current.probe < probe;

        if (shouldInsert && isEmpty) {
            table->count += 1;
            table->probeMax = MLMax(table->probeMax, probe);
            entries[index] = *entry;
            entries[index].probe = probe;
            entry->key = 0;
            entry->value = 0;
            entry->extra = 0;
            return index;
        }

        if (shouldInsert && isFound) {
            table->probeMax = MLMax(table->probeMax, probe);
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

    return MLNaturalMax;
}

// ------------------------------------------------------- Object Methods ------

static MLVariable MLObjectAllocate(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return calloc(1, self->meta->size);
}

static MLVariable MLObjectCreate(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLVariable mutable = MLOption("mutable", MLNo);
    MLVariable copy = MLOption("copy", MLNull);

    if (copy != MLNull) {
        MLSend(self, "fail*", MLString("InvalidOptionException | Can't create a copy of X"));
        return MLNull;
    }

    struct MLMeta* meta = self->meta;
    struct MLObject* object = self;

    if (object->retainCountAndFlags >= MLRetainCountOne) {
        object = MLSend(self, "allocate", MLOptions(MLString("mutable"), mutable));
        object->meta = meta;
    }

    object->retainCountAndFlags = MLRetainCountOne;
    if (mutable == MLYes) object->retainCountAndFlags |= MLMutableFlag;

    return MLSend(object, "collect");
}

static MLVariable MLObjectDestroy(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    free(self);
    return MLNull;
}

static MLVariable MLObjectRetain(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self->retainCountAndFlags < MLRetainCountMax) self->retainCountAndFlags += MLRetainCountOne;
    return self;
}

static MLVariable MLObjectRelease(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLNatural const retainCountAndFlags = self->retainCountAndFlags;

    if (retainCountAndFlags >= MLRetainCountMax) {
        return self;
    }

    else if (retainCountAndFlags >= 2 * MLRetainCountOne) {
        self->retainCountAndFlags -= MLRetainCountOne;
        return self;
    }

    else if (retainCountAndFlags >= MLRetainCountOne) {
        self->retainCountAndFlags -= MLRetainCountOne;
        return MLSend(self, "destroy");
    }

    else {
        fprintf(stderr, "[WARNING] Released an object with retain count 0, retain/release calls seem to be unbalanced, aborting ...\n");
        return MLNull;
    }
}

static MLVariable MLObjectCollect(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return MLCollectBlockAdd(self);
}

static MLVariable MLObjectEternize(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    self->retainCountAndFlags = MLRetainCountMax;
    return self;
}

static MLVariable MLObjectIsKindOf(struct MLObject* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    MLVariable proto = self;
    do {
        if (proto == object) return MLYes;
        proto = MLSend(proto, "proto");
    } while (proto != MLNull);
    return MLNo;
}

static MLVariable MLObjectIsMutable(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    bool const isMutable = self->retainCountAndFlags & MLMutableFlag;
    return MLBoolean(isMutable);
}

static MLVariable MLObjectRespondsTo(struct MLObject* self, MLVariable super, MLVariable command, MLVariable commandToCheck, MLVariable options, ...) {
    // TODO: optimize.
    MLVariable lookedUpSuper = MLZero;
    void* code = MLLookup(self, commandToCheck, &lookedUpSuper);
    return MLBoolean(code != MLZero);
}

static MLVariable MLObjectAsString(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self == MLObject) return MLObjectClassName;
    // TODO: put in the address of the object.
    return MLString("<Object XXX>");
}

static MLVariable MLObjectSelf(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return self;
}

static MLVariable MLObjectHash(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return MLNumber((MLNatural)self);
}

static MLVariable MLObjectEquals(struct MLObject* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    return MLBooleanMake(self == object);
}

static MLVariable MLObjectAddMethodBlock(struct MLObject* self, MLVariable super, MLVariable command, MLVariable method, MLVariable block, MLVariable options, ...) {
    if (self->meta->owner != self) {
        struct MLObject* parent = self->meta->owner;

        self->meta = calloc(1, sizeof(struct MLMeta));
        self->meta->owner = self;
        self->meta->parent = parent;

        MLTableCreate(&self->meta->cache, parent->meta->cache.count);
        MLTableCreate(&self->meta->methods, MLMethodsDefaultCapacity);
        MLTableCreate(&self->meta->children, MLChildrenDefaultCapacity);

        struct MLEntry entry = {.key = (MLNatural)self, .value = (MLNatural)MLYes, .extra = 0};
        MLTablePut(&parent->meta->children, &entry, MLZero, MLZero);
    }

    struct MLEntry entry = {.key = (MLNatural)method, .value = (MLNatural)block(block).code, .extra = 0};
    MLTablePut(&self->meta->methods, &entry, MLZero, MLZero);
    MLObjectEternize(method, MLObject, NULL, NULL);

    // TODO: clear cache of all children.

    return self;
}

static MLVariable MLObjectRemoveMethod(struct MLObject* self, MLVariable super, MLVariable command, MLVariable method, MLVariable options, ...) {
    // TODO: implement.
    return MLNull;
}

static MLVariable MLObjectProto(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    struct MLObject* parent = self->meta->parent;
    struct MLObject* owner = self->meta->owner;
    return owner == self ? parent : owner;
}

static MLVariable MLObjectSetProto(struct MLObject* self, MLVariable super, MLVariable command, MLVariable proto, MLVariable options, ...) {
    // TODO: implement.
    return MLNull;
}

static MLVariable MLObjectWarn(struct MLObject* self, MLVariable super, MLVariable command, MLVariable message, MLVariable options, ...) {
    MLVariable description = MLSend(message, "as-string");
    fprintf(stderr, "[WARNING] %s\n", string(description).characters);
    return self;
}

static MLVariable MLObjectFail(struct MLObject* self, MLVariable super, MLVariable command, MLVariable message, MLVariable options, ...) {
    MLRaise(message);
    return self;
}

static MLVariable MLObjectDebug(struct MLObject* self, MLVariable super, MLVariable command, MLVariable message, MLVariable options, ...) {
    MLVariable description = MLSend(message, "as-string");
    fprintf(stderr, "[DEBUG] %s\n", string(description).characters);
    return self;
}

// ------------------------------------------------------ Boolean Methods ------

static MLVariable MLBooleanCreate(struct MLBoolean* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLSend(self, "fail*", MLString("InvalidCommandException | Can't create a Boolean"));
    return MLNull;
}

static MLVariable MLBooleanDestroy(struct MLBoolean* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLSend(self, "fail*", MLString("InvalidCommandException | Can't destroy a Boolean"));
    return MLNull;
}

static MLVariable MLBooleanAsString(struct MLBoolean* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self == MLBoolean) return MLBooleanClassName;
    if (self == MLYes) return MLYesAsString;
    if (self == MLNo) return MLNoAsString;

    MLSend(self, "fail*", MLString("MLInternalInconsistencyException | Can't return boolean as-string, self is neither MLYes, nor MLNo, nor Boolean"));
    return MLNull;
}

static MLVariable MLBooleanCompare(struct MLBoolean* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    if (self == MLBoolean) return MLNull;
    if (object != MLYes && object != MLNo) return MLNull;

    if (self == MLNo && object == MLYes) return MLNumber(-1);
    if (self == MLYes && object == MLNo) return MLNumber(+1);

    return MLNumber(0);
}

static MLVariable MLBooleanCopy(struct MLBoolean* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return self;
}

// ------------------------------------------------------- Number Methods ------

static MLVariable MLNumberCreate(struct MLNumber* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLSend(self, "fail*", MLString("InvalidCommandException | Can't create a Number"));
    return MLNull;
}

static MLVariable MLNumberAsString(struct MLNumber* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self == MLNumber) return MLNumberClassName;

    // TODO: tweak to not print trailing MLZeros.
    MLDecimal MLIntegerPart = 0;
    MLDecimal fractionalPart = modf(self->number, &MLIntegerPart);
    MLInteger const bufferSize = 1024 * 1024;
    char buffer[bufferSize + 1];
    int length = 0;

    if (fractionalPart == 0) {
        length = snprintf(buffer, bufferSize + 1, "%li", (MLInteger)self->number);
    }
    else {
        length = snprintf(buffer, bufferSize + 1, "%f", self->number);
    }

   MLVariable const string = MLStringMake(length, buffer);
    return MLSend(string, "collect");
}

static MLVariable MLNumberHash(struct MLNumber* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return self;
}

static MLVariable MLNumberEquals(struct MLNumber* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    return MLDecimalFrom(self) == MLDecimalFrom(object) ? MLYes : MLNo;
}

static MLVariable MLNumberCompare(struct MLNumber* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    MLDecimal const number1 = MLDecimalFrom(self);
    MLDecimal const number2 = MLDecimalFrom(object);

    if (number1 < number2) return MLNumber(-1);
    if (number1 > number2) return MLNumber(+1);

    return MLNumber(0);
}

static MLVariable MLNumberCopy(struct MLNumber* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return MLSend(self, "retain");
}

// -------------------------------------------------------- Block Methods ------

static MLVariable MLBlockCreate(struct MLBlock* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLSend(self, "fail*", MLString("InvalidCommandException | Can't create a Block"));
    return MLNull;
}

static MLVariable MLBlockAsString(struct MLBlock* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self == MLBlock) return MLBlockClassName;

    static struct MLString* string = MLZero;
    if (string == MLZero) {
        // TODO: put in the address of the block.
        string = MLStringUncollected("<Block XXX>");
        MLSend(string, "eternize");
    }
    return string;
}

// --------------------------------------------------------- MLData Methods ------

static MLVariable MLDataCreate(struct MLData* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLVariable mutable = MLOption("mutable", MLNo);
    MLVariable capacity = MLOption("capacity", MLNumber(1));
    MLVariable copy = MLOption("copy", MLNull);

    // TODO: copy if needed.

    self = MLSuper(self, "create", MLString("mutable"), mutable);
    self->capacity = MLIntegerFrom(capacity);
    self->capacity = MLMax(self->capacity, MLDataDefaultCapacity);
    self->capacity = MLRoundUpToPowerOfTwo(self->capacity);
    self->count = 0;
    self->bytes = calloc(self->capacity, 1);

    return self;
}

static MLVariable MLDataDestroy(struct MLData* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    free(self->bytes);
    return MLSuper(self, "destroy");
}

static MLVariable MLDataAsString(struct MLData* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self == MLData) return MLDataClassName;

    // TODO: implement Base64 encoding.
    char* table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    return MLString("<Data XXX>");
}

static MLVariable MLDataHash(struct MLData* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLNatural const digest = MLDigest(self->count, self->bytes);
    return MLNumber((MLDecimal)digest);
}

static MLVariable MLDataEquals(struct MLData* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    if (self == object) return MLYes;
    if (MLSend(object, "is-kind-of*", MLData) == MLNo) return MLNo;

    struct MLData* data1 = self;
    struct MLData* data2 = object;

    if (data1->count != data2->count) return MLNo;

    MLInteger const result = memcmp(data1->bytes, data2->bytes, data1->count);
    return result == 0 ? MLYes : MLNo;
}

static MLVariable MLDataCopy(struct MLData* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self->capacity < 0) return MLSend(self, "retain");
    // TODO: make an immutable copy.
    return MLNull;
}

static MLVariable MLDataAtCount(struct MLData* self, MLVariable super, MLVariable command, MLVariable index, MLVariable count, MLVariable options, ...) {
    // TODO: implement.
    return MLNull;
}

static MLVariable MLDataReplaceAtCountWith(struct MLData* self, MLVariable super, MLVariable command, MLVariable index, MLVariable count, MLVariable data, MLVariable options, ...) {
    // TODO: implement.
    return self;
}

static MLVariable MLDataCount(struct MLData* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return MLNumber(self->count);
}

// -------------------------------------------------------- Array Methods ------

static MLVariable MLArrayCreate(struct MLArray* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLVariable mutable = MLOption("mutable", MLNo);
    MLVariable capacity = MLOption("capacity", MLNumber(1));
    MLVariable copy = MLOption("copy", MLNull);

    // TODO: copy if needed.

    self = MLSuper(self, "create", MLString("mutable"), mutable);
    self->capacity = MLIntegerFrom(capacity);
    self->capacity = MLMax(self->capacity, MLArrayDefaultCapacity);
    self->capacity = MLRoundUpToPowerOfTwo(self->capacity);
    self->count = 0;
    self->objects = calloc(self->capacity, sizeof(MLVariable));

    return self;
}

static MLVariable MLArrayDestroy(struct MLArray* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    for (int i = 0; i < self->count; i += 1) {
        MLSend(self->objects[i], "release");
    }
    free(self->objects);
    return MLSuper(self, "destroy");
}

static MLVariable MLArrayAsString(struct MLArray* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self == MLArray) return MLArrayClassName;
    return MLString("<Array XXX>");
}

static MLVariable MLArrayHash(struct MLArray* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    // TODO: implement.
    return MLNull;
}

static MLVariable MLArrayEquals(struct MLArray* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    if (self == object) return MLYes;
    if (MLSend(object, "is-kind-of*", MLArray) == MLNo) return MLNo;

    struct MLArray* array1 = self;
    struct MLArray* array2 = object;

    if (array1->count != array2->count) return MLNo;

    for (int index = 0; index < array1->count; index += 1) {
        MLVariable object1 = array1->objects[index];
        MLVariable object2 = array2->objects[index];
        if (MLSend(object1, "equals*", object2) == MLNo) return MLNo;
    }

    return MLYes;
}

static MLVariable MLArrayCopy(struct MLArray* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLVariable mutable = MLOption(MLString("mutable"), MLNo);
    // TODO: don't copy if mutable = MLNo and object is immutable
    return MLSend(self, "create", MLOptions(MLString("mutable"), mutable, MLString("copy"), MLYes));
}

static MLVariable MLArrayAt(struct MLArray* self, MLVariable super, MLVariable command, MLVariable index, MLVariable options, ...) {
    MLInteger const MLIntegerIndex = MLIntegerFrom(index);
    MLInteger const MLIntegerCount = self->count;

    if (MLIntegerIndex < 0 || MLIntegerIndex >= MLIntegerCount) {
        MLSend(self, "fail*", MLString("RangeException | Can't access object at index X"));
    }

    return self->objects[MLIntegerIndex];
}

static MLVariable MLArrayCount(struct MLArray* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return MLNumber(self->count);
}

static MLVariable MLArrayReplaceAtCountWith(struct MLArray* self, MLVariable super, MLVariable command, MLVariable index, MLVariable count, MLVariable objects, MLVariable options, ...) {
    MLInteger const MLIntegerIndex = MLIntegerFrom(index);
    MLInteger const MLIntegerCount = MLIntegerFrom(count);

    // Make sure array is mutable:
    if (MLSend(self, "is-mutable") == MLNo) {
        MLSend(self, "fail*", MLString("ImmutableException | Can't replace X objects at index Y with Z, object isn't mutable"));
    }

    // Validate arguments:
    if (MLIntegerIndex < 0 || MLIntegerIndex > self->count) {
        MLSend(self, "fail*", MLString("RangeException | Can't replace X objects at index Y with Z, index Y is out of range [0, B]"));
    }

    if (MLIntegerCount < 0) {
        MLSend(self, "fail*", MLString("RangeException | Can't replace X objects at index Y with Z, count Y out of range [0, ∞]"));
    }

    // Gather information:
    MLInteger const revisedCount = MLMin(self->count - MLIntegerIndex, MLIntegerCount);
    MLInteger const countOfObjects = MLIntegerFrom(MLSend(objects, "count"));
    MLInteger const requiredCapacity = self->count + countOfObjects - revisedCount;

    // Ensure enough capacity:
    if (self->capacity < requiredCapacity) {
        MLArrayEnsureCapacity(self, requiredCapacity);
    }

    // Remove & release objects to be replaced:
    for (MLInteger i = MLIntegerIndex; i < MLIntegerIndex + revisedCount; i += 1) {
        MLSend(self->objects[i], "release");
        self->objects[i] = MLZero;
    }

    // Make room for new objects:
    for (MLInteger j = self->count - 1; j >= MLIntegerIndex + revisedCount; j -= 1) {
        self->objects[j + countOfObjects - revisedCount] = self->objects[j];
        self->objects[j] = MLZero;
    }

    // Insert & retain new objects:
    for (MLInteger k = 0; k < countOfObjects; k += 1) {
       MLVariable const object = MLSend(objects, "at*", MLNumber(k));
        self->objects[k + MLIntegerIndex] = MLSend(object, "retain");
    }

    // Update own properties:
    self->count = self->count + countOfObjects - revisedCount;

    // Done.
    return self;
}

// ------------------------------------------------------- String Methods ------

static MLVariable MLStringCreate(struct MLString* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLVariable mutable = MLOption("mutable", MLNo);
    MLVariable capacity = MLOption("capacity", MLNumber(1));
    MLVariable copy = MLOption("copy", MLNull);

    // TODO: copy if needed.

    self = MLSuper(self, "create", MLString("mutable"), mutable);
    self->capacity = MLIntegerFrom(capacity);
    self->capacity = MLMax(self->capacity, MLStringDefaultCapacity);
    self->capacity = MLRoundUpToPowerOfTwo(self->capacity + 1) - 1;
    self->length = 0;
    self->characters = calloc(self->capacity + 1, sizeof(char));

    return self;
}

static MLVariable MLStringDestroy(struct MLString* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self->capacity < 0) {
        struct MLEntry entry = {.key = (MLNatural)self, .value = 0, .extra = 0};
        MLTablePut(&MLStringTable, &entry, MLStringHashFunction, MLStringEqualsFunction);
    }

    free(self->characters);
    return MLSuper(self, "destroy");
}

static MLVariable MLStringAsString(struct MLString* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self == MLString) return MLStringClassName;
   MLVariable const copy = MLSend(self, "copy");
    return MLSend(copy, "autorelease");
}

static MLVariable MLStringHash(struct MLString* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLNatural const hashFunction = MLDigest(self->length, self->characters);
    return MLNumber((MLDecimal)hashFunction);
}

static MLVariable MLStringEquals(struct MLString* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    if (self == object) return MLYes;
    if (MLSend(object, "is-kind-of*", MLString) == MLNo) return MLNo;

    struct MLString* string1 = self;
    struct MLString* string2 = object;

    if (string1->length != string2->length) return MLNo;

    MLInteger const result = strncmp(string1->characters, string2->characters, string1->length);
    return result == 0 ? MLYes : MLNo;
}

static MLVariable MLStringCompare(struct MLString* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    if (self == object) return MLYes;
    if (MLSend(object, "is-kind-of*", MLString) == MLNo) return MLNull;

    struct MLString* string1 = self;
    struct MLString* string2 = object;

    MLInteger const result = strncmp(string1->characters, string2->characters, string1->length);
    return MLNumber(result);
}

static MLVariable MLStringCopy(struct MLString* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self->capacity < 0) return MLSend(self, "retain");
    // TODO: make an immutable copy.
    return MLNull;
}

static MLVariable MLStringAtCount(struct MLString* self, MLVariable super, MLVariable command, MLVariable index, MLVariable count, MLVariable options, ...) {
    // TODO: implement.
    return MLNull;
}

static MLVariable MLStringLength(struct MLString* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return MLNumber(self->length);
}

static MLVariable MLStringReplaceAtCountWith(struct MLString* self, MLVariable super, MLVariable command, MLVariable index, MLVariable count, MLVariable string, MLVariable options, ...) {
    // TODO: implement.
    return self;
}

// --------------------------------------------------- Dictionary Methods ------

static MLVariable MLDictionaryCreate(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLVariable mutable = MLOption("mutable", MLNo);
    MLVariable capacity = MLOption("capacity", MLNumber(1));
    MLVariable copy = MLOption("copy", MLNull);

    // TODO: copy if needed.

    self = MLSuper(self, "create", MLString("mutable"), mutable);
    self->capacity = MLMax(MLIntegerFrom(capacity), MLDictionaryDefaultCapacity);
    self->capacity = MLRoundUpToPowerOfTwo(self->capacity);
    self->count = 0;
    self->mask = self->capacity - 1;
    self->entries = calloc(self->capacity * 2, sizeof(MLVariable));

    return self;
}

static MLVariable MLDictionaryDestroy(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    for (int i = 0; i < self->count; i += 2) {
        MLVariable key = self->entries[i];
        MLVariable value = self->entries[i + 1];

        if (key != MLZero && key != MLMore) {
            MLSend(key, "release");
            MLSend(value, "release");
        }

        self->entries[i] = MLZero;
        self->entries[i + 1] = MLZero;
    }
    free(self->entries);
    return MLSuper(self, "destroy");
}

static MLVariable MLDictionaryAsString(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self == MLDictionary) return MLDictionaryClassName;
    // TODO: implement.
    return MLString("<Dictionary XXX>");
}

static MLVariable MLDictionaryHash(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    // TODO: implement.
    return MLNull;
}

static MLVariable MLDictionaryEquals(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    if (self == object) return MLYes;
    if (MLSend(object, "is-kind-of*", MLDictionary) == MLNo) return MLNo;

    struct MLDictionary* dictionary1 = self;
    struct MLDictionary* dictionary2 = object;

    if (dictionary1->count != dictionary2->count) return MLNo;

    for (int index = 0; index < dictionary1->count; index += 1) {
        MLVariable key = dictionary1->entries[index * 2];
        if (key != MLZero) {
            MLVariable value1 = dictionary1->entries[index * 2 + 1];
            MLVariable value2 = MLSend(dictionary2, "get*", key);
            if (MLSend(value1, "equals*", value2) == MLNo) return MLNo;
        }
    }

    return MLYes;
}

static MLVariable MLDictionaryCopy(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self->capacity < 0) return MLSend(self, "retain");
    // TODO: make an immutable copy.
    return MLNull;
}

static MLVariable MLDictionaryGet(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable key, MLVariable options, ...) {
    MLNatural const mask = self->mask;
    MLNatural const hash = MLNaturalFrom(MLSend(key, "hash"));
    MLNatural index = hash & mask;

    for (MLNatural i = 0; i <= mask; i += 1) {
        MLInteger const indexOfKey = index * 2;
        MLInteger const indexOfValue = indexOfKey + 1;
       MLVariable const keyAtIndex = self->entries[indexOfKey];
       MLVariable const valueAtIndex = self->entries[indexOfValue];

        if (keyAtIndex == MLMore) {
            continue;
        }

        if (keyAtIndex == MLZero) {
            return MLNull;
        }

        if (MLSend(keyAtIndex, "equals*", key) == MLYes) {
            return valueAtIndex;
        }

        index = (index + 1) & mask;
    }

    return MLNull;
}

static MLVariable MLDictionarySetTo(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable key, MLVariable value, MLVariable options, ...) {
    MLDictionaryEnsureCapacity(self, self->count + 1);

    MLNatural const mask = self->mask;
    MLNatural const hash = MLNaturalFrom(MLSend(key, "hash"));
    MLNatural index = hash & mask;

    for (MLNatural i = 0; i <= mask; i += 1) {
        MLInteger const indexOfKey = index * 2;
        MLInteger const indexOfValue = indexOfKey + 1;
       MLVariable const keyAtIndex = self->entries[indexOfKey];

        if (keyAtIndex == MLZero || keyAtIndex == MLMore) {
            MLSend(key, "retain");
            MLSend(value, "retain");
            self->entries[indexOfKey] = key;
            self->entries[indexOfValue] = value;
            self->count += 1;
            break;
        }

        if (MLSend(keyAtIndex, "equals*", key) == MLYes) {
            MLSend(key, "retain");
            MLSend(value, "retain");
            MLSend(self->entries[indexOfKey], "release");
            MLSend(self->entries[indexOfValue], "release");
            self->entries[indexOfKey] = key;
            self->entries[indexOfValue] = value;
            break;
        }

        index = (index + 1) & mask;
    }

    return self;
}

static MLVariable MLDictionaryRemove(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable key, MLVariable options, ...) {
    MLNatural const mask = self->mask;
    MLNatural const hash = MLNaturalFrom(MLSend(key, "hash"));
    MLNatural index = hash & mask;

    for (MLNatural i = 0; i <= mask; i += 1) {
        MLInteger const indexOfKey = index * 2;
        MLInteger const indexOfValue = indexOfKey + 1;
       MLVariable const keyAtIndex = self->entries[indexOfKey];
       MLVariable const valueAtIndex = self->entries[indexOfValue];

        if (keyAtIndex == MLMore) {
            continue;
        }

        if (keyAtIndex == MLZero) {
            return self;
        }

        if (MLSend(keyAtIndex, "equals*", key) == MLYes) {
            MLSend(keyAtIndex, "release");
            MLSend(valueAtIndex, "release");

            self->entries[indexOfKey] = MLMore;
            self->entries[indexOfValue] = MLZero;

            return self;
        }

        index = (index + 1) & mask;
    }

    return self;
}

static MLVariable MLDictionaryCount(struct MLDictionary* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return MLNumber(self->count);
}

// ---------------------------------------------------- Exception Methods ------

static MLVariable MLExceptionCreate(struct MLException* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLVariable mutable = MLOption("name", MLString("UnknownException"));
    MLVariable capacity = MLOption("capacity", MLNumber(1));
    MLVariable copy = MLOption("copy", MLNull);

    self = MLSuper(self, "create");
    // TODO: implement.
    return self;
}

static MLVariable MLExceptionDestroy(struct MLException* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    // TODO: implement.
    return MLSuper(self, "destroy");
}

static MLVariable MLExceptionAsString(struct MLException* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    if (self == MLException) return MLExceptionClassName;
    // TODO: implement.
    return MLNull;
}

static MLVariable MLExceptionHash(struct MLException* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    // TODO: implement.
    return MLNumber(0);
}

static MLVariable MLExceptionEquals(struct MLException* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    if (self == object) return MLYes;
    if (MLSend(object, "is-kind-of*", MLException) == MLNo) return MLNo;

    struct MLException* exception1 = self;
    struct MLException* exception2 = object;

    if (exception1 == exception2) return MLYes;

    // TODO: implement.
    return MLNo;
}

static MLVariable MLExceptionCopy(struct MLException* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return MLSend(self, "retain");
}

static MLVariable MLExceptionName(struct MLException* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return self->name;
}

static MLVariable MLExceptionReason(struct MLException* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return self->reason;
}

static MLVariable MLExceptionInfo(struct MLException* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return self->info;
}

static MLVariable MLExceptionRaiseReasonInfo(struct MLException* self, MLVariable super, MLVariable command, MLVariable name, MLVariable reason, MLVariable info, MLVariable options, ...) {
    return MLNull;
}

// --------------------------------------------------------- Null Methods ------

static MLVariable NullCreate(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLSend(self, "fail*", MLString("InvalidCommandException | Can't create a Null"));
    return MLNull;
}

static MLVariable NullDestroy(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    MLSend(self, "fail*", MLString("InvalidCommandException | Can't destroy a Null"));
    return MLNull;
}

static MLVariable NullAsString(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return MLNullClassName;
}

static MLVariable NullEquals(struct MLObject* self, MLVariable super, MLVariable command, MLVariable object, MLVariable options, ...) {
    return object == MLNull ? MLYes : MLNo;
}

static MLVariable NullCopy(struct MLObject* self, MLVariable super, MLVariable command, MLVariable options, ...) {
    return self;
}

// ---------------------------------------------------- Factory Functions ------

MLVariable MLBooleanMake(long boolean) {
    return boolean ? MLYes : MLNo;
}

MLVariable MLNumberMake(double value) {
    struct MLNumber* number = calloc(1, sizeof(struct MLNumber));
    number->meta = &MLNumberMeta;
    number->retainCountAndFlags = MLRetainCountOne;
    number->number = value;
    return number;
}

MLVariable MLBlockMake(void* code) {
    MLAssert(code != MLZero, "When making a block, code must be != MLZero");
    struct MLBlock* block = calloc(1, sizeof(struct MLBlock));
    block->meta = &MLBlockMeta;
    block->retainCountAndFlags = MLRetainCountOne;
    block->code = code;
    return block;
}

MLVariable MLDataMake(long count, const void* bytes) {
    MLAssert(count >= 0, "When making a data object, count must be >= 0");
    struct MLData* data = calloc(1, sizeof(struct MLData));
    data->meta = &MLDataMeta;
    data->retainCountAndFlags = MLRetainCountOne;
    data->capacity = -1;
    data->count = count;
    data->bytes = calloc(count, 1);
    memcpy(data->bytes, bytes, count);
    return data;
}

MLVariable MLArrayMake(long count, ...) {
    MLAssert(count >= 0, "When making an array, count must be >= 0");

    // Create array:
    struct MLArray* array = calloc(1, sizeof(struct MLArray));
    array->meta = &MLArrayMeta;
    array->retainCountAndFlags = MLRetainCountOne;
    array->capacity = -1;
    array->count = count;
    array->objects = calloc(count, sizeof(MLVariable));

    // Collect objects:
    va_list arguments;
    va_start(arguments, count);
    for (int i = 0; i < count; i += 1) {
        MLVariable object = va_arg(arguments, MLVariable);
        array->objects[i] = object;

        // TODO: retain here instaed of the extra pass below.

    }

    // Make mutable if needed:
    if (count > 0 && array->objects[count - 1] == MLMore) {
        array->retainCountAndFlags |= MLMutableFlag;
        array->objects[count - 1] = MLZero;
        array->count = count - 1;
        array->capacity = count;
        MLArrayEnsureCapacity(array, count);
    }

    // Retain objects:
    for (int i = 0; i < array->count; i += 1) {
        MLSend(array->objects[i], "retain");
    }

    // Done:
    va_end(arguments);
    return array;
}

MLVariable MLStringMake(long length, const char* characters) {
    MLAssert(length >= 0, "When making a string, length must be >= 0");

    MLNatural const hash = MLDigest(length, characters);
    bool const couldBeCommandOrKey = length <= MLMaxKeyAndCommandLength;

    if (couldBeCommandOrKey) {
        struct MLString proxy = {.meta = &MLStringMeta, .retainCountAndFlags = MLRetainCountMax, .capacity = -1, .length = length, .hash = hash, .characters = (char*)characters};
        struct MLEntry entry = {.key = (MLNatural)&proxy, .value = 0, .extra = 0};

        MLTableGet(&MLStringTable, &entry, MLStringHashFunction, MLStringEqualsFunction);
        struct MLString* string = (struct MLString*)entry.value;

        // Can't use retain() here or send 'retain' message becuase MLStringMake() is used
        // by the message sending mechanism. Calling the method implementation directly
        // is safe here, because we know for sure that we are dealing with a string object.
        if (string != MLZero) return MLObjectRetain((struct MLObject *)string, MLObject, NULL, NULL);
    }

    struct MLString* string = calloc(1, sizeof(struct MLString));
    string->meta = &MLStringMeta;
    string->retainCountAndFlags = MLRetainCountOne;
    string->capacity = -1;
    string->length = length;
    string->hash = hash;
    string->characters = calloc(length + 1, sizeof(char));
    strncpy(string->characters, characters, length);

    if (couldBeCommandOrKey) {
        struct MLEntry entry = {.key = (MLNatural)string, .value = (MLNatural)string, .extra = 0};
        MLTablePut(&MLStringTable, &entry, MLStringHashFunction, MLStringEqualsFunction);
    }

    return string;
}

MLVariable MLDictionaryMake(long count, ...) {
    MLAssert(count >= 0, "When making a dictionary, count must be >= 0");

    struct MLDictionary* dictionary = calloc(1, sizeof(struct MLDictionary));
    dictionary->meta = &MLDictionaryMeta;
    dictionary->retainCountAndFlags = MLRetainCountOne;
    dictionary->capacity = 0;
    dictionary->count = 0;
    dictionary->mask = 0;

    MLDictionaryEnsureCapacity(dictionary, count);

    va_list arguments;
    va_start(arguments, count);

    for (int i = 0; i < count - 1; i += 2) {
        MLVariable key = va_arg(arguments, MLVariable);
        MLVariable value = va_arg(arguments, MLVariable);
        MLDictionarySetTo(dictionary, NULL, MLNull, key, value, MLZero);
    }

    // Make mutable if needed:
    if (va_arg(arguments, MLVariable) == MLMore) {
        dictionary->retainCountAndFlags |= MLMutableFlag;
    }

    // Done:
   return dictionary;
}

// ------------------------------------------------- Conversion Functions ------

MLInteger MLIntegerFrom(MLVariable number) {
    return (MLInteger)number(number).number;
}

MLNatural MLNaturalFrom(MLVariable number) {
    return (MLNatural)number(number).number;
}

MLDecimal MLDecimalFrom(MLVariable number) {
    return (MLDecimal)number(number).number;
}

// ---------------------------------------------- Collect-Block Functions ------

void* MLCollectBlockPush() {
    struct MLCollectBlock* collectBlock = calloc(1, sizeof(struct MLCollectBlock));
    collectBlock->previousCollectBlock = MLCollectBlockTop;
    collectBlock->capacity = MLCollectBlockDefaultCapacity;
    collectBlock->count = 0;
    collectBlock->objects = calloc(sizeof(MLVariable), MLCollectBlockDefaultCapacity);
    MLCollectBlockTop = collectBlock;
    return MLCollectBlockTop;
}

void* MLCollectBlockPop(void* collectBlock) {
    // TODO: remove all child collect blocks from the stack
    // and release its objects if landed here through exception.
    MLAssert(collectBlock == MLCollectBlockTop, "When popping a collect block, the top-most one must be the same as the one passed in");
    struct MLCollectBlock* collectBlockToPop = collectBlock;
    MLCollectBlockTop = collectBlockToPop->previousCollectBlock;
    for (int index = 0; index < collectBlockToPop->count; index += 1) {
        MLSend(collectBlockToPop->objects[index], "release");
    }
    free(collectBlockToPop->objects);
    free(collectBlockToPop);
    return MLZero;
}

MLVariable MLCollectBlockAdd(MLVariable object) {
    if (object(object).retainCountAndFlags >= MLRetainCountMax) return object;
    if (MLCollectBlockTop == MLZero) {
        fprintf(stderr, "[WARNING] No collect block found, leaking ...\n");
        return object;
    }

    MLInteger const capacity = MLCollectBlockTop->capacity;
    MLInteger const count = MLCollectBlockTop->count;

    if (count >= capacity) {
        MLAssert(capacity > 0, "When increasing the capacity of a collect pool, capacity must be > 0");
        MLVariable* objects = calloc(capacity * 2, sizeof(MLVariable));
        memcpy(objects, MLCollectBlockTop->objects, capacity * sizeof(MLVariable));
        free(MLCollectBlockTop->objects);
        MLCollectBlockTop->objects = objects;
        MLCollectBlockTop->capacity = capacity * 2;
    }

    MLCollectBlockTop->objects[count] = object;
    MLCollectBlockTop->count += 1;

    return object;
}

// --------------------------------------- Perform-Handle-Block Functions ------

void* MLPerformHandleBlockPush() {
    struct MLPerformHandleBlock* performHandleBlock = calloc(1, sizeof(struct MLPerformHandleBlock));
    performHandleBlock->previousPerformHandleBlock = MLPerformHandleBlockTop;
    performHandleBlock->exception = MLNull;
    performHandleBlock->raised = false;
    MLPerformHandleBlockTop = performHandleBlock;
    return performHandleBlock;
}

void* MLPerformHandleBlockPop(void* performHandleBlock) {
    MLAssert(performHandleBlock == MLPerformHandleBlockTop, "When popping a perform-handle block, the top-most one must be the same as the one passed in");
    struct MLPerformHandleBlock* performHandleBlockToPop = performHandleBlock;
    MLPerformHandleBlockTop = performHandleBlockToPop->previousPerformHandleBlock;
    MLSend(performHandleBlockToPop->exception, "release");
    free(performHandleBlockToPop);
    return MLZero;
}

void* MLPerformHandleBlockPerform(void* performHandleBlock) {
    MLAssert(performHandleBlock == MLPerformHandleBlockTop, "When beginning a perform block, the top-most perform-handle block must be the same as the one passed in");
    return MLPerformHandleBlockTop->destination;
}

MLVariable MLPerformHandleBlockHandle(void* performHandleBlock) {
    MLAssert(performHandleBlock == MLPerformHandleBlockTop, "When beginning a handle block, the top-most perform-handle block must be the same as the one passed in");
    return MLPerformHandleBlockTop->exception;
}

// ---------------------------------------------------- Keyword Functions ------

MLVariable MLImport(const char* name) {
    // TODO: implement.
    return MLNull;
}

MLVariable MLExport(const char* name, void* code) {
    // TODO: implement.
    return MLNull;
}

void* MLLookup(MLVariable object, MLVariable command, MLVariable* super) {
    struct MLMeta* const meta = object(object).meta;
    struct MLTable* const cache = &meta->cache;
    struct MLTable* const methods = &meta->methods;
    struct MLObject* const parent = meta->parent;
    void* code = MLZero;

    // TODO: look up in cache, old implementation:
    // void* code = HashMapGet(cache, key, MLZero, MLZero);
    // if (code != MLZero) return code;

    // Look up in own methods:
    MLAssert(string(command).length <= MLMaxKeyAndCommandLength, "When looking up a method for a given command, the length of the command must be <= MLMaxKeyAndCommandLength");
    struct MLEntry entry = {.key = (MLNatural)command, .value = 0, .extra = 0};
    MLTableGet(methods, &entry, MLZero, MLZero);

    code = (void *)entry.value;
    if (code) *super = parent;

    // Continue searching in parent's methods if needed:
    if (!code && parent != MLNull) {
        code = MLLookup(parent, command, super);
    }

    // TOOD: add fallback.

    // Make sure method was found:
    MLAssert(code != MLZero, "At this point, code must be either the found method or a fallback method but should never be MLZero");

    // TODO: Cache found method, old implementation:
    // HashMapPut(cache, key, code, MLZero, MLZero);
    // preserve(command);

    // Return found & now cached method:
    return code;
}

void MLRaise(MLVariable exception) {
    MLSend(exception, "retain");

    struct MLPerformHandleBlock* performHandleBlock = MLPerformHandleBlockTop;
    if (performHandleBlock != MLZero && performHandleBlock->raised) {
        MLPerformHandleBlockPop(performHandleBlock);
        performHandleBlock = MLPerformHandleBlockTop;
    }

    if (performHandleBlock == MLZero) {
        MLVariable description = MLSend(exception, "as-string");
        fprintf(stderr, "[ERROR] %s\n", string(description).characters);
    }

    performHandleBlock->exception = exception;
    performHandleBlock->raised = true;

    longjmp(performHandleBlock->destination, 1);
}

void MLLog(MLVariable object) {
    MLCollect {
        MLVariable description = MLSend(object, "as-string");
        fprintf(stderr, "%s\n", string(description).characters);
    }
}

// -------------------------------------------------------- Bootstrapping ------

static void MLBootstrap Metal() {
    MLCollect {
        MLTableCreate(&MLStringTable, MLStringTableBlockDefaultCapacity);

        MLObjectMeta.owner = &MLObjectState;
        MLBooleanMeta.owner = &MLBooleanState;
        MLNumberMeta.owner = &MLNumberState;
        MLBlockMeta.owner = &MLBlockState;
        MLDataMeta.owner = &MLDataState;
        MLArrayMeta.owner = &MLArrayState;
        MLStringMeta.owner = &MLStringState;
        MLDictionaryMeta.owner = &MLDictionaryState;
        MLExceptionMeta.owner = &MLExceptionState;
        MLNullMeta.owner = &MLNullState;

        MLObjectMeta.parent = MLNull;
        MLBooleanMeta.parent = &MLObjectState;
        MLNumberMeta.parent = &MLObjectState;
        MLBlockMeta.parent = &MLObjectState;
        MLDataMeta.parent = &MLObjectState;
        MLArrayMeta.parent = &MLObjectState;
        MLStringMeta.parent = &MLObjectState;
        MLDictionaryMeta.parent = &MLObjectState;
        MLExceptionMeta.parent = &MLExceptionState;
        MLNullMeta.parent = &MLObjectState;

        MLObjectMeta.size = sizeof(struct MLObject);
        MLBooleanMeta.size = sizeof(struct MLBoolean);
        MLNumberMeta.size = sizeof(struct MLNumber);
        MLBlockMeta.size = sizeof(struct MLBlock);
        MLDataMeta.size = sizeof(struct MLData);
        MLArrayMeta.size = sizeof(struct MLArray);
        MLStringMeta.size = sizeof(struct MLString);
        MLDictionaryMeta.size = sizeof(struct MLDictionary);
        MLExceptionMeta.size = sizeof(struct MLException);
        MLNullMeta.size = sizeof(struct MLObject);

        MLTableCreate(&MLObjectMeta.cache, MLCacheDefaultCapacity);
        MLTableCreate(&MLBooleanMeta.cache, MLCacheDefaultCapacity);
        MLTableCreate(&MLNumberMeta.cache, MLCacheDefaultCapacity);
        MLTableCreate(&MLBlockMeta.cache, MLCacheDefaultCapacity);
        MLTableCreate(&MLDataMeta.cache, MLCacheDefaultCapacity);
        MLTableCreate(&MLArrayMeta.cache, MLCacheDefaultCapacity);
        MLTableCreate(&MLStringMeta.cache, MLCacheDefaultCapacity);
        MLTableCreate(&MLDictionaryMeta.cache, MLCacheDefaultCapacity);
        MLTableCreate(&MLExceptionMeta.cache, MLCacheDefaultCapacity);
        MLTableCreate(&MLNullMeta.cache, MLCacheDefaultCapacity);

        MLTableCreate(&MLObjectMeta.methods, MLMethodsDefaultCapacity);
        MLTableCreate(&MLBooleanMeta.methods, MLMethodsDefaultCapacity);
        MLTableCreate(&MLNumberMeta.methods, MLMethodsDefaultCapacity);
        MLTableCreate(&MLBlockMeta.methods, MLMethodsDefaultCapacity);
        MLTableCreate(&MLDataMeta.methods, MLMethodsDefaultCapacity);
        MLTableCreate(&MLArrayMeta.methods, MLMethodsDefaultCapacity);
        MLTableCreate(&MLStringMeta.methods, MLMethodsDefaultCapacity);
        MLTableCreate(&MLDictionaryMeta.methods, MLMethodsDefaultCapacity);
        MLTableCreate(&MLExceptionMeta.methods, MLMethodsDefaultCapacity);
        MLTableCreate(&MLNullMeta.methods, MLMethodsDefaultCapacity);

        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("allocate"), MLBlockUncollected(MLObjectAllocate), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("create"), MLBlockUncollected(MLObjectCreate), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("destroy"), MLBlockUncollected(MLObjectDestroy), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("retain"), MLBlockUncollected(MLObjectRetain), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("release"), MLBlockUncollected(MLObjectRelease), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("collect"), MLBlockUncollected(MLObjectCollect), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("eternize"), MLBlockUncollected(MLObjectEternize), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("is-kind-of*"), MLBlockUncollected(MLObjectIsKindOf), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("is-mutable"), MLBlockUncollected(MLObjectIsMutable), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("responds-to*"), MLBlockUncollected(MLObjectRespondsTo), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(MLObjectAsString), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("self"), MLBlockUncollected(MLObjectSelf), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("hash"), MLBlockUncollected(MLObjectHash), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("equals*"), MLBlockUncollected(MLObjectEquals), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("add-method*block*"), MLBlockUncollected(MLObjectAddMethodBlock), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("remove-method*"), MLBlockUncollected(MLObjectRemoveMethod), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("proto"), MLBlockUncollected(MLObjectProto), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("set-proto*"), MLBlockUncollected(MLObjectSetProto), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("warn*"), MLBlockUncollected(MLObjectWarn), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("fail*"), MLBlockUncollected(MLObjectFail), MLZero);
        MLObjectAddMethodBlock(MLObject, MLZero, MLZero, MLStringUncollected("debug*"), MLBlockUncollected(MLObjectDebug), MLZero);

        MLObjectAddMethodBlock(MLBoolean, MLObject, MLZero, MLStringUncollected("create"), MLBlockUncollected(MLBooleanCreate), MLZero);
        MLObjectAddMethodBlock(MLBoolean, MLObject, MLZero, MLStringUncollected("destroy"), MLBlockUncollected(MLBooleanDestroy), MLZero);
        MLObjectAddMethodBlock(MLBoolean, MLObject, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(MLBooleanAsString), MLZero);
        MLObjectAddMethodBlock(MLBoolean, MLObject, MLZero, MLStringUncollected("compare*"), MLBlockUncollected(MLBooleanCompare), MLZero);
        MLObjectAddMethodBlock(MLBoolean, MLObject, MLZero, MLStringUncollected("copy"), MLBlockUncollected(MLBooleanCopy), MLZero);

        MLObjectAddMethodBlock(MLNumber, MLObject, MLZero, MLStringUncollected("create"), MLBlockUncollected(MLNumberCreate), MLZero);
        MLObjectAddMethodBlock(MLNumber, MLObject, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(MLNumberAsString), MLZero);
        MLObjectAddMethodBlock(MLNumber, MLObject, MLZero, MLStringUncollected("hash"), MLBlockUncollected(MLNumberHash), MLZero);
        MLObjectAddMethodBlock(MLNumber, MLObject, MLZero, MLStringUncollected("equals*"), MLBlockUncollected(MLNumberEquals), MLZero);
        MLObjectAddMethodBlock(MLNumber, MLObject, MLZero, MLStringUncollected("compare*"), MLBlockUncollected(MLNumberCompare), MLZero);
        MLObjectAddMethodBlock(MLNumber, MLObject, MLZero, MLStringUncollected("copy"), MLBlockUncollected(MLNumberCopy), MLZero);

        MLObjectAddMethodBlock(MLBlock, MLObject, MLZero, MLStringUncollected("create"), MLBlockUncollected(MLBlockCreate), MLZero);
        MLObjectAddMethodBlock(MLBlock, MLObject, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(MLBlockAsString), MLZero);

        MLObjectAddMethodBlock(MLData, MLObject, MLZero, MLStringUncollected("create"), MLBlockUncollected(MLDataCreate), MLZero);
        MLObjectAddMethodBlock(MLData, MLObject, MLZero, MLStringUncollected("destroy"), MLBlockUncollected(MLDataDestroy), MLZero);
        MLObjectAddMethodBlock(MLData, MLObject, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(MLDataAsString), MLZero);
        MLObjectAddMethodBlock(MLData, MLObject, MLZero, MLStringUncollected("hash"), MLBlockUncollected(MLDataHash), MLZero);
        MLObjectAddMethodBlock(MLData, MLObject, MLZero, MLStringUncollected("equals*"), MLBlockUncollected(MLDataEquals), MLZero);
        MLObjectAddMethodBlock(MLData, MLObject, MLZero, MLStringUncollected("copy"), MLBlockUncollected(MLDataCopy), MLZero);
        MLObjectAddMethodBlock(MLData, MLObject, MLZero, MLStringUncollected("at*count*"), MLBlockUncollected(MLDataAtCount), MLZero);
        MLObjectAddMethodBlock(MLData, MLObject, MLZero, MLStringUncollected("replace-at*count*with*"), MLBlockUncollected(MLDataReplaceAtCountWith), MLZero);
        MLObjectAddMethodBlock(MLData, MLObject, MLZero, MLStringUncollected("count"), MLBlockUncollected(MLDataCount), MLZero);

        MLObjectAddMethodBlock(MLArray, MLObject, MLZero, MLStringUncollected("create"), MLBlockUncollected(MLArrayCreate), MLZero);
        MLObjectAddMethodBlock(MLArray, MLObject, MLZero, MLStringUncollected("destroy"), MLBlockUncollected(MLArrayDestroy), MLZero);
        MLObjectAddMethodBlock(MLArray, MLObject, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(MLArrayAsString), MLZero);
        MLObjectAddMethodBlock(MLArray, MLObject, MLZero, MLStringUncollected("hash"), MLBlockUncollected(MLArrayHash), MLZero);
        MLObjectAddMethodBlock(MLArray, MLObject, MLZero, MLStringUncollected("equals*"), MLBlockUncollected(MLArrayEquals), MLZero);
        MLObjectAddMethodBlock(MLArray, MLObject, MLZero, MLStringUncollected("copy"), MLBlockUncollected(MLArrayCopy), MLZero);
        MLObjectAddMethodBlock(MLArray, MLObject, MLZero, MLStringUncollected("at*"), MLBlockUncollected(MLArrayAt), MLZero);
        MLObjectAddMethodBlock(MLArray, MLObject, MLZero, MLStringUncollected("count"), MLBlockUncollected(MLArrayCount), MLZero);
        MLObjectAddMethodBlock(MLArray, MLObject, MLZero, MLStringUncollected("replace-at*count*with*"), MLBlockUncollected(MLArrayReplaceAtCountWith), MLZero);

        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("create"), MLBlockUncollected(MLStringCreate), MLZero);
        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("destroy"), MLBlockUncollected(MLStringDestroy), MLZero);
        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(MLStringAsString), MLZero);
        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("hash"), MLBlockUncollected(MLStringHash), MLZero);
        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("equals*"), MLBlockUncollected(MLStringEquals), MLZero);
        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("compare*"), MLBlockUncollected(MLStringCompare), MLZero);
        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("copy"), MLBlockUncollected(MLStringCopy), MLZero);
        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("at*count*"), MLBlockUncollected(MLStringAtCount), MLZero);
        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("length"), MLBlockUncollected(MLStringLength), MLZero);
        MLObjectAddMethodBlock(MLString, MLObject, MLZero, MLStringUncollected("replace-at*count*with*"), MLBlockUncollected(MLStringReplaceAtCountWith), MLZero);

        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("create"), MLBlockUncollected(MLDictionaryCreate), MLZero);
        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("destroy"), MLBlockUncollected(MLDictionaryDestroy), MLZero);
        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(MLDictionaryAsString), MLZero);
        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("hash"), MLBlockUncollected(MLDictionaryHash), MLZero);
        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("equals*"), MLBlockUncollected(MLDictionaryEquals), MLZero);
        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("copy"), MLBlockUncollected(MLDictionaryCopy), MLZero);
        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("get*"), MLBlockUncollected(MLDictionaryGet), MLZero);
        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("set*to*"), MLBlockUncollected(MLDictionarySetTo), MLZero);
        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("remove*"), MLBlockUncollected(MLDictionaryRemove), MLZero);
        MLObjectAddMethodBlock(MLDictionary, MLObject, MLZero, MLStringUncollected("count"), MLBlockUncollected(MLDictionaryCount), MLZero);

        MLObjectAddMethodBlock(MLException, MLObject, MLZero, MLStringUncollected("create"), MLBlockUncollected(MLExceptionCreate), MLZero);
        MLObjectAddMethodBlock(MLException, MLObject, MLZero, MLStringUncollected("destroy"), MLBlockUncollected(MLExceptionDestroy), MLZero);
        MLObjectAddMethodBlock(MLException, MLObject, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(MLExceptionAsString), MLZero);
        MLObjectAddMethodBlock(MLException, MLObject, MLZero, MLStringUncollected("equals*"), MLBlockUncollected(MLExceptionEquals), MLZero);
        MLObjectAddMethodBlock(MLException, MLObject, MLZero, MLStringUncollected("copy"), MLBlockUncollected(MLExceptionCopy), MLZero);
        MLObjectAddMethodBlock(MLException, MLObject, MLZero, MLStringUncollected("name"), MLBlockUncollected(MLExceptionName), MLZero);
        MLObjectAddMethodBlock(MLException, MLObject, MLZero, MLStringUncollected("reason"), MLBlockUncollected(MLExceptionReason), MLZero);
        MLObjectAddMethodBlock(MLException, MLObject, MLZero, MLStringUncollected("info"), MLBlockUncollected(MLExceptionInfo), MLZero);
        MLObjectAddMethodBlock(MLException, MLObject, MLZero, MLStringUncollected("raise*reason*info*"), MLBlockUncollected(MLExceptionRaiseReasonInfo), MLZero);

        MLObjectAddMethodBlock(MLNull, MLObject, MLZero, MLStringUncollected("create"), MLBlockUncollected(NullCreate), MLZero);
        MLObjectAddMethodBlock(MLNull, MLObject, MLZero, MLStringUncollected("destroy"), MLBlockUncollected(NullDestroy), MLZero);
        MLObjectAddMethodBlock(MLNull, MLObject, MLZero, MLStringUncollected("as-string"), MLBlockUncollected(NullAsString), MLZero);
        MLObjectAddMethodBlock(MLNull, MLObject, MLZero, MLStringUncollected("equals*"), MLBlockUncollected(NullEquals), MLZero);
        MLObjectAddMethodBlock(MLNull, MLObject, MLZero, MLStringUncollected("copy"), MLBlockUncollected(NullCopy), MLZero);

        MLTableCreate(&MLObjectMeta.children, 64);
        MLTableCreate(&MLBooleanMeta.children, 1);
        MLTableCreate(&MLNumberMeta.children, 1);
        MLTableCreate(&MLBlockMeta.children, 1);
        MLTableCreate(&MLDataMeta.children, 1);
        MLTableCreate(&MLArrayMeta.children, 1);
        MLTableCreate(&MLStringMeta.children, 1);
        MLTableCreate(&MLDictionaryMeta.children, 1);
        MLTableCreate(&MLExceptionMeta.children, 1);
        MLTableCreate(&MLNullMeta.children, 1);

        struct MLEntry booleanEntry = {.key = (MLNatural)MLBoolean, .value = (MLNatural)MLYes, .extra = 0};
        struct MLEntry numberEntry = {.key = (MLNatural)MLNumber, .value = (MLNatural)MLYes, .extra = 0};
        struct MLEntry blockEntry = {.key = (MLNatural)MLBlock, .value = (MLNatural)MLYes, .extra = 0};
        struct MLEntry dataEntry = {.key = (MLNatural)MLData, .value = (MLNatural)MLYes, .extra = 0};
        struct MLEntry arrayEntry = {.key = (MLNatural)MLArray, .value = (MLNatural)MLYes, .extra = 0};
        struct MLEntry stringEntry = {.key = (MLNatural)MLString, .value = (MLNatural)MLYes, .extra = 0};
        struct MLEntry dictionaryEntry = {.key = (MLNatural)MLDictionary, .value = (MLNatural)MLYes, .extra = 0};
        struct MLEntry exceptionEntry = {.key = (MLNatural)MLException, .value = (MLNatural)MLYes, .extra = 0};
        struct MLEntry nullEntry = {.key = (MLNatural)MLNull, .value = (MLNatural)MLYes, .extra = 0};

        MLTablePut(&MLObjectMeta.children, &booleanEntry, MLZero, MLZero);
        MLTablePut(&MLObjectMeta.children, &numberEntry, MLZero, MLZero);
        MLTablePut(&MLObjectMeta.children, &blockEntry, MLZero, MLZero);
        MLTablePut(&MLObjectMeta.children, &dataEntry, MLZero, MLZero);
        MLTablePut(&MLObjectMeta.children, &arrayEntry, MLZero, MLZero);
        MLTablePut(&MLObjectMeta.children, &stringEntry, MLZero, MLZero);
        MLTablePut(&MLObjectMeta.children, &dictionaryEntry, MLZero, MLZero);
        MLTablePut(&MLObjectMeta.children, &exceptionEntry, MLZero, MLZero);
        MLTablePut(&MLObjectMeta.children, &nullEntry, MLZero, MLZero);

        // TODO: implement.

        MLObjectClassName = MLSend(MLStringUncollected("Object"), "eternize");
        MLBooleanClassName = MLSend(MLStringUncollected("Boolean"), "eternize");
        MLNumberClassName = MLSend(MLStringUncollected("Number"), "eternize");
        MLBlockClassName = MLSend(MLStringUncollected("Block"), "eternize");
        MLDataClassName = MLSend(MLStringUncollected("Data"), "eternize");
        MLArrayClassName = MLSend(MLStringUncollected("Array"), "eternize");
        MLStringClassName = MLSend(MLStringUncollected("String"), "eternize");
        MLDictionaryClassName = MLSend(MLStringUncollected("Dictionary"), "eternize");
        MLDictionaryClassName = MLSend(MLStringUncollected("Exception"), "eternize");
        MLNullClassName = MLSend(MLStringUncollected("null"), "eternize");
        MLNoAsString = MLSend(MLStringUncollected("no"), "eternize");
        MLYesAsString = MLSend(MLStringUncollected("yes"), "eternize");
        MLInvalidArgumentException = MLSend(MLStringUncollected("MLInvalidArgumentException"), "eternize");
        MLInternalInconsistencyException = MLSend(MLStringUncollected("MLInternalInconsistencyException"), "eternize");
    }
}

// ------------------------------------------------------------- Private -------

static void MLDataEnsureCapacity(struct MLData* data, MLInteger requiredCapacity) {
    if (requiredCapacity <= MLDataDefaultCapacity) requiredCapacity = MLDataDefaultCapacity;
    if (requiredCapacity <= data->capacity) return;

    data->capacity = MLRoundUpToPowerOfTwo(requiredCapacity);
    data->bytes = realloc(data->bytes, data->capacity);
}

static void MLArrayEnsureCapacity(struct MLArray* array, MLInteger requiredCapacity) {
    if (requiredCapacity <= MLArrayDefaultCapacity) requiredCapacity = MLArrayDefaultCapacity;
    if (requiredCapacity <= array->capacity) return;

    array->capacity = MLRoundUpToPowerOfTwo(requiredCapacity);
    array->objects = realloc(array->objects, sizeof(MLVariable) * array->capacity);
}

static void MLStringEnsureCapacity(struct MLString* string, MLInteger requiredCapacity) {
    if (requiredCapacity <= MLStringDefaultCapacity) requiredCapacity = MLStringDefaultCapacity;
    if (requiredCapacity <= string->capacity) return;

    string->capacity = MLRoundUpToPowerOfTwo(requiredCapacity + 1) - 1;
    string->characters = realloc(string->characters, sizeof(char) * (string->capacity + 1));
}

static void MLDictionaryEnsureCapacity(struct MLDictionary* dictionary, MLInteger requiredCapacity) {
    if (requiredCapacity <= MLDictionaryDefaultCapacity) requiredCapacity = MLDictionaryDefaultCapacity;

    MLInteger const oldCapacity = dictionary->capacity;
    MLInteger const oneFourthOfOldCapacity = oldCapacity >> 2;
    MLInteger const threeFourthOfOldCapacity = oldCapacity - oneFourthOfOldCapacity;
    MLInteger const newCapacity = MLRoundUpToPowerOfTwo(requiredCapacity + (requiredCapacity >> 1));

    if (requiredCapacity < threeFourthOfOldCapacity) return;

    MLVariable* oldEntries = dictionary->entries;
    MLVariable* newEntries = calloc(sizeof(MLVariable), 2 * newCapacity);

    dictionary->capacity = newCapacity;
    dictionary->count = 0;
    dictionary->mask = dictionary->capacity - 1;
    dictionary->entries = newEntries;

    for (MLInteger index = 0; index < oldCapacity; index += 1) {
        MLVariable key = oldEntries[index * 2];
        MLVariable value = oldEntries[index * 2 + 1];
        if (key) MLDictionarySetTo(dictionary, MLZero, MLZero, key, value, MLZero);
    }

    free(oldEntries);
}

static inline MLNatural MLRoundUpToPowerOfTwo(MLNatural number) {
    uint64_t value = number;
    value -= 1;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    value += 1;
    return (MLNatural)value;
}

static inline MLNatural MLRoundDownToPowerOfTwo(MLNatural number) {
    uint64_t value = number;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    value -= value >> 1;
    return (MLNatural)value;
}

static MLNatural MLDigest(MLInteger count, const void* bytes) {
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

    MLNatural hash = (MLNatural)h;
    if (hash == MLNaturalMax) hash = MLNaturalMax - 1;
    if (hash == MLNaturalMin) hash = MLNaturalMin + 1;
    return hash;
}

static inline MLNatural MLStringHashFunction(MLNatural key) {
    struct MLString* string = (struct MLString*)key;

    // TODO: is testing for MLZero or MLMore really needed here?
    if (string == MLZero) return 0;
    if (string == MLMore) return MLNaturalMax;

    return string(string).hash;
}

static inline bool MLStringEqualsFunction(MLNatural key1, MLNatural key2) {
    struct MLString* string1 = (struct MLString*)key1;
    struct MLString* string2 = (struct MLString*)key2;
    if (string1 == string2) return true;

    // TODO: is testing for MLZero or MLMore really needed here?
    if (string1 == MLZero || string2 == MLZero) return false;
    if (string1 == MLMore || string2 == MLMore) return false;

    if (string(string1).length != string(string2).length) return false;
    return strncmp(string(string1).characters, string(string2).characters, string(string1).length) == 0;
}
