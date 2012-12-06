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

#ifndef ML_COMMANDS_H
#define ML_COMMANDS_H

#define MLCreate(self) MLSend(self, "create")
#define MLNew(self) MLSend(self, "new")

#define MLNewWithArray(self, array) MLSend(self, "new-with-array*", array)
#define MLNewWithCapacity(self, capacity) MLSend(self, "new-with-capacity*", capacity)
#define MLNewWithString(self, string) MLSend(self, "new-with-string*", string)

#define MLName(self) MLSend(self, "name")
#define superclass(self) MLSend(self, "superclass")
#define MLSubclasses(self) MLSend(self, "subclasses")
#define MLMethods(self) MLSend(self, "methods")

#define MLInit(self) MLSend(self, "init")
#define MLInitWithArray(self, array) MLSend(self, "init-with-array*", array)
#define MLInitWithCapacity(self, capacity) MLSend(self, "init-with-capacity*", capacity)
#define MLInitWithString(self, string) MLSend(self, "init-with-string*", string)
#define MLDestroy(self) MLSend(self, "destroy")

#define MLClass(self) MLSend(self, "class")
#define MLDescription(self) MLSend(self, "description")
#define MLEquals(self, object) MLSend(self, "equals*?", object)
#define MLHash(self) MLSend(self, "hash")
#define MLCopy(self) MLSend(self, "copy")
#define MLMutableCopy(self) MLSend(self, "mutable-copy")

#define MLCompareTo(self, object) MLSend(self, "compare-to*", object)
#define MLIsLessThan(self, object) MLSend(self, "is-less-than*?", object)
#define MLIsLessThanOrEquals(self, object) MLSend(self, "is-less-than-or-equals*?", object)
#define MLIsGreaterThan(self, object) MLSend(self, "is-greater-than*?", object)
#define MLIsGreaterThanOrEquals(self, object) MLSend(self, "is-greater-than-or-equals*?", object)

#define MLIsObject(self) MLSend(self, "is-object?")
#define MLIsNull(self) MLSend(self, "is-null?")
#define MLIsClass(self) MLSend(self, "is-class?")
#define MLIsBlock(self) MLSend(self, "is-block?")
#define MLIsBoolean(self) MLSend(self, "is-boolean?")
#define MLIsNumber(self) MLSend(self, "is-number?")
#define MLIsWord(self) MLSend(self, "is-word?")
#define MLIsDate(self) MLSend(self, "is-date?")
#define MLIsData(self) MLSend(self, "is-data?")
#define MLIsArray(self) MLSend(self, "is-array?")
#define MLIsString(self) MLSend(self, "is-string?")
#define MLIsDictionary(self) MLSend(self, "is-dictionary?")

#define MLIsTruthy(self) MLSend(self, "is-truthy?")
#define MLIsFalsy(self) MLSend(self, "is-falsy?")

#define MLIsKindOf(self, class) MLSend(self, "is-kind-of*?", class)
#define MLIsInstanceOf(self, class) MLSend(self, "is-instance-of*?", class)

#define MLRetain(self) MLSend(self, "retain")
#define MLRetainCount(self) MLSend(self, "retain-count")
#define MLRelease(self) MLSend(self, "release")
#define MLAutorelease(self) MLSend(self, "autorelease")

#define MLPerformArguments(self, command, arguments) MLSend(self, "perform*arguments*", command, arguments)
#define MLSendArguments(self, command, arguments) MLSend(self, "send*arguments*", command, arguments)
#define MLRespondsTo(self, command) MLSend(self, "responds-to*?", command)

#define MLCount(self) MLSend(self, "count")
#define MLContains(self, object) MLSend(self, "contains*?", object)
#define MLContainsAll(self, objects) MLSend(self, "contains-all*?", objects)
#define MLContainsAny(self, objects) MLSend(self, "contains-any*?", objects)

#define MLIsEmpty(self) MLSend(self, "is-empty?")
#define MLIsInline(self) MLSend(self, "is-inline?")
#define MLIsMutable(self) MLSend(self, "is-mutable?")

#define MLAt(self, index) MLSend(self, "at*", index)
#define MLAtMany(self, indexes) MLSend(self, "at-many*", indexes)
#define MLAtCount(self, index, count) MLSend(self, "at*count*", index, count)

#define MLIndexOf(self, object) MLSend(self, "index-of*", object)
#define MLLastIndexOf(self, object) MLSend(self, "last-index-of*", object)
#define MLIndexesOf(self, object) MLSend(self, "indexes-of*", object)

#define MLFirst(self) MLSend(self, "first")
#define MLFirstCount(self, count) MLSend(self, "first*", count)

#define MLLast(self) MLSend(self, "last")
#define MLLastCount(self, count) MLSend(self, "last*", count)

#define MLSecond(self) MLSend(self, "second")
#define MLThird(self) MLSend(self, "third")
#define MLRest(self) MLSend(self, "rest")

#define MLWith(self, object) MLSend(self, "with*", object)
#define MLWithMany(self, objects) MLSend(self, "with-many*", objects)

#define MLWithAt(self, object, index) MLSend(self, "with*at*", object, index)
#define MLWithManyAt(self, objects, index) MLSend(self, "with-many*at*", objects, index)

#define MLWithBefore(self, object, before) MLSend(self, "with*before*", object, before)
#define MLWithManyBefore(self, objects, before) MLSend(self, "with-many*before*", objects, before)

#define MLWithAfter(self, object, after) MLSend(self, "with*after*", object, after)
#define MLWithManyAfter(self, objects, after) MLSend(self, "with-many*after*", objects, after)

#define MLWithReplacing(self, replacement, object) MLSend(self, "with*replacing*", replacement, object)
#define MLWithManyReplacing(self, replacements, object) MLSend(self, "with-many*replacing*", replacements, object)

#define MLWithReplacingAt(self, replacement, index) MLSend(self, "with*replacing-at*", replacement, index)
#define MLWithManyReplacingAt(self, replacements, index) MLSend(self, "with-many*replacing-at*", replacements, index)

#define MLWithReplacingAtCount(self, replacement, index, count) MLSend(self, "with*replacing-at*count*", replacement, index, count)
#define MLWithManyReplacingAtCount(self, replacements, index, count) MLSend(self, "with-many*replacing-at*count*", replacements, index, count)

#define MLWithout(self, object) MLSend(self, "without*", object)
#define MLWithoutMany(self, object) MLSend(self, "without-many*", object)

#define MLWithoutAt(self, index) MLSend(self, "without-at*", index)
#define MLWithoutAtMany(self, indexes) MLSend(self, "without-at-many*", indexes)
#define MLWithoutAtCount(self, index, count) MLSend(self, "without-at*count*", index, count)

#define MLReversed(self) MLSend(self, "reversed")
#define MLSorted(self) MLSend(self, "sorted")

#define MLPluck(self, key) MLSend(self, "pluck*", key)

#define MLMin(self) MLSend(self, "min")
#define MLMax(self) MLSend(self, "max")

#define MLCapacity(self) MLSend(self, "capacity")
#define MLIncreaseCapacity(self, capacity) MLSend(self, "increase-capacity*", capacity)

#define MLAdd(self, object) MLSend(self, "add*", object)
#define MLAddMany(self, objects) MLSend(self, "add-many*", objects)

#define MLInsert(self, object) MLSend(self, "insert*", object)
#define MLInsertMany(self, objects) MLSend(self, "insert-many*", objects)

#define MLInsertAt(self, object, index) MLSend(self, "insert*at*", object, index)
#define MLInsertManyAt(self, objects, index) MLSend(self, "insert-many*at*", objects, index)

#define MLInsertBefore(self, object, before) MLSend(self, "insert*before*", object, before)
#define MLInsertManyBefore(self, objects, before) MLSend(self, "insert-many*before*", objects, before)

#define MLInsertAfter(self, object, after) MLSend(self, "insert*after*", object, after)
#define MLInsertManyAfter(self, object, after) MLSend(self, "insert-many*after*", object, after)

#define MLReplaceWith(self, object, replacement) MLSend(self, "replace*with*", object, replacement)
#define MLReplaceWithMany(self, object, replacements) MLSend(self, "replace*with-many*", object, replacements)

#define MLReplaceAtWith(self, index, replacement) MLSend(self, "replace-at*with*", index, replacement)
#define MLReplaceAtWithMany(self, index, replacements) MLSend(self, "replace-at*with-many*", index, replacements)

#define MLReplaceAtCountWith(self, index, count, replacement) MLSend(self, "replace-at*count*with*", index, count, replacement)
#define MLReplaceAtCountWithMany(self, index, count, replacements) MLSend(self, "replace-at*count*with-many*", index, count, replacements)

#define MLRemove(self, object) MLSend(self, "remove*", object)
#define MLRemoveMany(self, objects) MLSend(self, "remove-many*", objects)

#define MLRemoveAt(self, index) MLSend(self, "remove-at*", index)
#define MLRemoveAtMany(self, indexes) MLSend(self, "remove-at-many*", indexes)
#define MLRemoveAtCount(self, index, count) MLSend(self, "remove-at*count*", index, count)
#define MLRemoveAll(self) MLSend(self, "remove-all")

#define MLRemoveFirst(self, object) MLSend(self, "remove-first", object)
#define MLRemoveFirstCount(self, objects) MLSend(self, "remove-first*", objects)

#define MLRemoveLast(self, object) MLSend(self, "remove-last", object)
#define MLRemoveLastCount(self, objects) MLSend(self, "remove-last*", objects)

#define MLReverse(self) MLSend(self, "reverse")
#define MLSort(self) MLSend(self, "sort")

#define MLKeys(self) MLSend(self, "keys")
#define MLValues(self) MLSend(self, "values")

#define MLGet(self, key) MLSend(self, "get*", key)
#define MLGetMany(self, keys) MLSend(self, "get-many*", keys)

#define MLWithValue(self, key, value) MLSend(self, "with*value*", key, value)

#define MLPutValue(self, key, value) MLSend(self, "put*value*", key, value)
#define MLPutMany(self, valuesByKeys) MLSend(self, "put-many*", valuesByKeys)

#define MLIsYes(self) MLSend(self, "is-yes?")
#define MLIsNo(self) MLSend(self, "is-no?")

#define MLIsNaN(self) MLSend(self, "is-nan?")
#define MLIsFinite(self) MLSend(self, "is-finite?")
#define MLIsInfinite(self) MLSend(self, "is-infinite?")

#define MLCodeAt(self, index) MLSend(self, "code-at*", index)
#define MLCodesAt(self, indexes) MLSend(self, "codes-at*", indexes)

#define MLCharacters(self) MLSend(self, "characters")
#define MLCodes(self) MLSend(self, "codes")

#define MLLines(self) MLSend(self, "lines")
#define MLParagraphs(self) MLSend(self, "paragraphs")

#define MLBeginsWith(self, prefix) MLSend(self, "begins-with*", prefix)
#define MLEndsWith(self, suffix) MLSend(self, "ends-with*", suffix)

#define MLUppercased(self) MLSend(self, "uppercased")
#define MLLowercased(self) MLSend(self, "lowercased")
#define MLCapitalized(self) MLSend(self, "capitalized")

#define MLPrepend(self, prefix) MLSend(self, "prepend*", prefix)
#define MLAppend(self, suffix) MLSend(self, "append*", suffix)

#define MLDrain(self) MLSend(self, "drain")

#endif
