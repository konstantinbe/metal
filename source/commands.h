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

#define MLCreate(self) MLSend(self, IS("create"), null, null)
#define MLNew(self) MLSend(self, IS("new"), null, null)

#define MLNewWithArray(self, array) MLSend(self, IS("new-with-array*"), IA(array), null)
#define MLNewWithCapacity(self, capacity) MLSend(self, IS("new-with-capacity*"), IA(capacity), null)
#define MLNewWithString(self, string) MLSend(self, IS("new-with-string*"), IA(string), null)

#define MLName(self) MLSend(self, IS("name"), null, null)
#define MLSuperclass(self) MLSend(self, IS("superclass"), null, null)
#define MLSubclasses(self) MLSend(self, IS("subclasses"), null, null)
#define MLMethods(self) MLSend(self, IS("methods"), null, null)

#define MLInit(self) MLSend(self, IS("init"), null, null)
#define MLInitWithArray(self, array) MLSend(self, IS("init-with-array*"), IA(array), null)
#define MLInitWithCapacity(self, capacity) MLSend(self, IS("init-with-capacity*"), IA(capacity), null)
#define MLInitWithString(self, string) MLSend(self, IS("init-with-string*"), IA(string), null)
#define MLDestroy(self) MLSend(self, IS("destroy"), null, null)

#define MLClass(self) MLSend(self, IS("class"), null, null)
#define MLDescription(self) MLSend(self, IS("description"), null, null)
#define MLEquals(self, object) MLSend(self, IS("equals*?"), IA(object), null)
#define MLHash(self) MLSend(self, IS("hash"), null, null)
#define MLCopy(self) MLSend(self, IS("copy"), null, null)
#define MLMutableCopy(self) MLSend(self, IS("mutable-copy"), null, null)

#define MLCompareTo(self, object) MLSend(self, IS("compare-to*"), IA(object), null)
#define MLIsLessThan(self, object) MLSend(self, IS("is-less-than*?"), IA(object), null)
#define MLIsLessThanOrEquals(self, object) MLSend(self, IS("is-less-than-or-equals*?"), IA(object), null)
#define MLIsGreaterThan(self, object) MLSend(self, IS("is-greater-than*?"), IA(object), null)
#define MLIsGreaterThanOrEquals(self, object) MLSend(self, IS("is-greater-than-or-equals*?"), IA(object), null)

#define MLIsObject(self) MLSend(self, IS("is-object?"), null, null)
#define MLIsNull(self) MLSend(self, IS("is-null?"), null, null)
#define MLIsClass(self) MLSend(self, IS("is-class?"), null, null)
#define MLIsBlock(self) MLSend(self, IS("is-block?"), null, null)
#define MLIsBoolean(self) MLSend(self, IS("is-boolean?"), null, null)
#define MLIsNumber(self) MLSend(self, IS("is-number?"), null, null)
#define MLIsWord(self) MLSend(self, IS("is-word?"), null, null)
#define MLIsDate(self) MLSend(self, IS("is-date?"), null, null)
#define MLIsData(self) MLSend(self, IS("is-data?"), null, null)
#define MLIsArray(self) MLSend(self, IS("is-array?"), null, null)
#define MLIsString(self) MLSend(self, IS("is-string?"), null, null)
#define MLIsDictionary(self) MLSend(self, IS("is-dictionary?"), null, null)

#define MLIsTruthy(self) MLSend(self, IS("is-truthy?"), null, null)
#define MLIsFalsy(self) MLSend(self, IS("is-falsy?"), null, null)

#define MLIsKindOf(self, class) MLSend(self, IS("is-kind-of*?"), IA(class), null)
#define MLIsInstanceOf(self, class) MLSend(self, IS("is-instance-of*?"), IA(class), null)

#define MLRetain(self) MLSend(self, IS("retain"), null, null)
#define MLRetainCount(self) MLSend(self, IS("retain-count"), null, null)
#define MLRelease(self) MLSend(self, IS("release"), null, null)
#define MLAutorelease(self) MLSend(self, IS("autorelease"), null, null)

#define MLPerformArgumentsOptions(self, command, arguments, options) MLSend(self, IS("perform*arguments*options*"), null, null)
#define MLDispatchArgumentsOptions(self, command, arguments, options) MLSend(self, IS("send*arguments*options*"), null, null)
#define MLRespondsTo(self, command) MLSend(self, IS("responds-to*?"), IA(command), null)

#define MLCount(self) MLSend(self, IS("count"), null, null)
#define MLContains(self, object) MLSend(self, IS("contains*?"), IA(object), null)
#define MLContainsAll(self, objects) MLSend(self, IS("contains-all*?"), IA(objects), null)
#define MLContainsAny(self, objects) MLSend(self, IS("contains-any*?"), IA(objects), null)

#define MLIsEmpty(self) MLSend(self, IS("is-empty?"), null, null)
#define MLIsInline(self) MLSend(self, IS("is-inline?"), null, null)
#define MLIsMutable(self) MLSend(self, IS("is-mutable?"), null, null)

#define MLAt(self, index) MLSend(self, IS("at*"), IA(index), null)
#define MLAtMany(self, indexes) MLSend(self, IS("at-many*"), IA(indexes), null)
#define MLAtCount(self, index, count) MLSend(self, IS("at*count*"), IA(index, count), null)

#define MLIndexOf(self, object) MLSend(self, IS("index-of*"), IA(object), null)
#define MLLastIndexOf(self, object) MLSend(self, IS("last-index-of*"), IA(object), null)
#define MLIndexesOf(self, object) MLSend(self, IS("indexes-of*"), IA(object), null)

#define MLFirst(self) MLSend(self, IS("first"), null, null)
#define MLFirstCount(self, count) MLSend(self, IS("first*"), IA(count), null)

#define MLLast(self) MLSend(self, IS("last"), null, null)
#define MLLastCount(self, count) MLSend(self, IS("last*"), IA(count), null)

#define MLSecond(self) MLSend(self, IS("second"), null, null)
#define MLThird(self) MLSend(self, IS("third"), null, null)
#define MLRest(self) MLSend(self, IS("rest"), null, null)

#define MLWith(self, object) MLSend(self, IS("with*"), IA(object), null)
#define MLWithMany(self, objects) MLSend(self, IS("with-many*"), IA(objects), null)

#define MLWithAt(self, object, index) MLSend(self, IS("with*at*"), IA(object, index), null)
#define MLWithManyAt(self, objects, index) MLSend(self, IS("with-many*at*"), IA(objects, index), null)

#define MLWithBefore(self, object, before) MLSend(self, IS("with*before*"), IA(object, before), null)
#define MLWithManyBefore(self, objects, before) MLSend(self, IS("with-many*before*"), IA(objects, before), null)

#define MLWithAfter(self, object, after) MLSend(self, IS("with*after*"), IA(object, after), null)
#define MLWithManyAfter(self, objects, after) MLSend(self, IS("with-many*after*"), IA(objects, after), null)

#define MLWithReplacing(self, replacement, object) MLSend(self, IS("with*replacing*"), IA(replacement, object), null)
#define MLWithManyReplacing(self, replacements, object) MLSend(self, IS("with-many*replacing*"), IA(replacements, object), null)

#define MLWithReplacingAt(self, replacement, index) MLSend(self, IS("with*replacing-at*"), IA(replacement, index), null)
#define MLWithManyReplacingAt(self, replacements, index) MLSend(self, IS("with-many*replacing-at*"), IA(replacements, index), null)

#define MLWithReplacingAtCount(self, replacement, index, count) MLSend(self, IS("with*replacing-at*count*"), IA(replacement, index, count), null)
#define MLWithManyReplacingAtCount(self, replacements, index, count) MLSend(self, IS("with-many*replacing-at*count*"), IA(replacements, index, count), null)

#define MLWithout(self, object) MLSend(self, IS("without*"), IA(object), null)
#define MLWithoutMany(self, object) MLSend(self, IS("without-many*"), IA(object), null)

#define MLWithoutAt(self, index) MLSend(self, IS("without-at*"), IA(index), null)
#define MLWithoutAtMany(self, indexes) MLSend(self, IS("without-at-many*"), IA(indexes), null)
#define MLWithoutAtCount(self, index, count) MLSend(self, IS("without-at*count*"), IA(index, count), null)

#define MLReversed(self) MLSend(self, IS("reversed"), null, null)
#define MLSorted(self) MLSend(self, IS("sorted"), null, null)

#define MLPluck(self, key) MLSend(self, IS("pluck*"), IA(key), null)

#define MLMin(self) MLSend(self, IS("min"), null, null)
#define MLMax(self) MLSend(self, IS("max"), null, null)

#define MLCapacity(self) MLSend(self, IS("capacity"), null, null)
#define MLIncreaseCapacity(self, capacity) MLSend(self, IS("increase-capacity*"), IA(capacity), null)

#define MLAdd(self, object) MLSend(self, IS("add*"), IA(object), null)
#define MLAddMany(self, objects) MLSend(self, IS("add-many*"), IA(objects), null)

#define MLInsert(self, object) MLSend(self, IS("insert*"), IA(object), null)
#define MLInsertMany(self, objects) MLSend(self, IS("insert-many*"), IA(objects), null)

#define MLInsertAt(self, object, index) MLSend(self, IS("insert*at*"), IA(object, index), null)
#define MLInsertManyAt(self, objects, index) MLSend(self, IS("insert-many*at*"), IA(objects, index), null)

#define MLInsertBefore(self, object, before) MLSend(self, IS("insert*before*"), IA(object, before), null)
#define MLInsertManyBefore(self, objects, before) MLSend(self, IS("insert-many*before*"), IA(objects, before), null)

#define MLInsertAfter(self, object, after) MLSend(self, IS("insert*after*"), IA(object, after), null)
#define MLInsertManyAfter(self, object, after) MLSend(self, IS("insert-many*after*"), IA(object, after), null)

#define MLReplaceWith(self, object, replacement) MLSend(self, IS("replace*with*"), IA(object, replacement), null)
#define MLReplaceWithMany(self, object, replacements) MLSend(self, IS("replace*with-many*"), IA(object, replacements), null)

#define MLReplaceAtWith(self, index, replacement) MLSend(self, IS("replace-at*with*"), IA(index, replacement), null)
#define MLReplaceAtWithMany(self, index, replacements) MLSend(self, IS("replace-at*with-many*"), IA(index, replacements), null)

#define MLReplaceAtCountWith(self, index, count, replacement) MLSend(self, IS("replace-at*count*with*"), IA(index, replacement, count), null)
#define MLReplaceAtCountWithMany(self, index, count, replacements) MLSend(self, IS("replace-at*count*with-many*"), IA(index, count, replacements), null)

#define MLRemove(self, object) MLSend(self, IS("remove*"), IA(object), null)
#define MLRemoveMany(self, objects) MLSend(self, IS("remove-many*"), IA(objects), null)

#define MLRemoveAt(self, index) MLSend(self, IS("remove-at*"), IA(index), null)
#define MLRemoveAtMany(self, indexes) MLSend(self, IS("remove-at-many*"), IA(indexes), null)
#define MLRemoveAtCount(self, index, count) MLSend(self, IS("remove-at*count*"), IA(index, count), null)
#define MLRemoveAll(self) MLSend(self, IS("remove-all"), null, null)

#define MLRemoveFirst(self, object) MLSend(self, IS("remove-first"), IA(object), null)
#define MLRemoveFirstCount(self, objects) MLSend(self, IS("remove-first*"), IA(objects), null)

#define MLRemoveLast(self, object) MLSend(self, IS("remove-last"), IA(object), null)
#define MLRemoveLastCount(self, objects) MLSend(self, IS("remove-last*"), IA(objects), null)

#define MLReverse(self) MLSend(self, IS("reverse"), null, null)
#define MLSort(self) MLSend(self, IS("sort"), null, null)

#define MLKeys(self) MLSend(self, IS("keys"), null, null)
#define MLValues(self) MLSend(self, IS("values"), null, null)

#define MLGet(self, key) MLSend(self, IS("get*"), IA(key), null)
#define MLGetMany(self, keys) MLSend(self, IS("get-many*"), IA(keys), null)

#define MLWithValue(self, key, value) MLSend(self, IS("with*value*"), IA(key, value), null)

#define MLPutValue(self, key, value) MLSend(self, IS("put*value*"), IA(key, value), null)
#define MLPutMany(self, valuesByKeys) MLSend(self, IS("put-many*"), IA(valuesByKeys), null)

#define MLIsYes(self) MLSend(self, IS("is-yes?"), null, null)
#define MLIsNo(self) MLSend(self, IS("is-no?"), null, null)

#define MLIsNaN(self) MLSend(self, IS("is-nan?"), null, null)
#define MLIsFinite(self) MLSend(self, IS("is-finite?"), null, null)
#define MLIsInfinite(self) MLSend(self, IS("is-infinite?"), null, null)

#define MLCodeAt(self, index) MLSend(self, IS("code-at*"), IA(index), null)
#define MLCodesAt(self, indexes) MLSend(self, IS("codes-at*"), IA(indexes), null)

#define MLCharacters(self) MLSend(self, IS("characters"), null, null)
#define MLCodes(self) MLSend(self, IS("codes"), null, null)

#define MLLines(self) MLSend(self, IS("lines"), null, null)
#define MLParagraphs(self) MLSend(self, IS("paragraphs"), null, null)

#define MLBeginsWith(self, prefix) MLSend(self, IS("begins-with*"), IA(prefix), null)
#define MLEndsWith(self, suffix) MLSend(self, IS("ends-with*"), IA(suffix), null)

#define MLUppercased(self) MLSend(self, IS("uppercased"), null, null)
#define MLLowercased(self) MLSend(self, IS("lowercased"), null, null)
#define MLCapitalized(self) MLSend(self, IS("capitalized"), null, null)

#define MLPrepend(self, prefix) MLSend(self, IS("prepend*"), IA(prefix), null)
#define MLAppend(self, suffix) MLSend(self, IS("append*"), IA(suffix), null)

#define MLDrain(self) MLSend(self, IS("drain"), null, null)

#endif
