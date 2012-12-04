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

#define MLCreate(self) MLSend(self, IS("create"), null)
#define MLNew(self) MLSend(self, IS("new"), null)

#define MLNewWithArray(self, array) MLSend(self, IS("new-with-array*"), IA(array))
#define MLNewWithCapacity(self, capacity) MLSend(self, IS("new-with-capacity*"), IA(capacity))
#define MLNewWithString(self, string) MLSend(self, IS("new-with-string*"), IA(string))

#define MLName(self) MLSend(self, IS("name"), null)
#define MLSuperclass(self) MLSend(self, IS("superclass"), null)
#define MLSubclasses(self) MLSend(self, IS("subclasses"), null)
#define MLMethods(self) MLSend(self, IS("methods"), null)

#define MLInit(self) MLSend(self, IS("init"), null)
#define MLInitWithArray(self, array) MLSend(self, IS("init-with-array*"), IA(array))
#define MLInitWithCapacity(self, capacity) MLSend(self, IS("init-with-capacity*"), IA(capacity))
#define MLInitWithString(self, string) MLSend(self, IS("init-with-string*"), IA(string))
#define MLDestroy(self) MLSend(self, IS("destroy"), null)

#define MLClass(self) MLSend(self, IS("class"), null)
#define MLDescription(self) MLSend(self, IS("description"), null)
#define MLEquals(self, object) MLSend(self, IS("equals*?"), IA(object))
#define MLHash(self) MLSend(self, IS("hash"), null)
#define MLCopy(self) MLSend(self, IS("copy"), null)
#define MLMutableCopy(self) MLSend(self, IS("mutable-copy"), null)

#define MLCompareTo(self, object) MLSend(self, IS("compare-to*"), IA(object))
#define MLIsLessThan(self, object) MLSend(self, IS("is-less-than*?"), IA(object))
#define MLIsLessThanOrEquals(self, object) MLSend(self, IS("is-less-than-or-equals*?"), IA(object))
#define MLIsGreaterThan(self, object) MLSend(self, IS("is-greater-than*?"), IA(object))
#define MLIsGreaterThanOrEquals(self, object) MLSend(self, IS("is-greater-than-or-equals*?"), IA(object))

#define MLIsObject(self) MLSend(self, IS("is-object?"), null)
#define MLIsNull(self) MLSend(self, IS("is-null?"), null)
#define MLIsClass(self) MLSend(self, IS("is-class?"), null)
#define MLIsBlock(self) MLSend(self, IS("is-block?"), null)
#define MLIsBoolean(self) MLSend(self, IS("is-boolean?"), null)
#define MLIsNumber(self) MLSend(self, IS("is-number?"), null)
#define MLIsWord(self) MLSend(self, IS("is-word?"), null)
#define MLIsDate(self) MLSend(self, IS("is-date?"), null)
#define MLIsData(self) MLSend(self, IS("is-data?"), null)
#define MLIsArray(self) MLSend(self, IS("is-array?"), null)
#define MLIsString(self) MLSend(self, IS("is-string?"), null)
#define MLIsDictionary(self) MLSend(self, IS("is-dictionary?"), null)

#define MLIsTruthy(self) MLSend(self, IS("is-truthy?"), null)
#define MLIsFalsy(self) MLSend(self, IS("is-falsy?"), null)

#define MLIsKindOf(self, class) MLSend(self, IS("is-kind-of*?"), IA(class))
#define MLIsInstanceOf(self, class) MLSend(self, IS("is-instance-of*?"), IA(class))

#define MLRetain(self) MLSend(self, IS("retain"), null)
#define MLRetainCount(self) MLSend(self, IS("retain-count"), null)
#define MLRelease(self) MLSend(self, IS("release"), null)
#define MLAutorelease(self) MLSend(self, IS("autorelease"), null)

#define MLPerformArguments(self, command, arguments) MLSend(self, IS("perform*arguments*"), null)
#define MLSendArguments(self, command, arguments) MLSend(self, IS("send*arguments*"), null)
#define MLRespondsTo(self, command) MLSend(self, IS("responds-to*?"), IA(command))

#define MLCount(self) MLSend(self, IS("count"), null)
#define MLContains(self, object) MLSend(self, IS("contains*?"), IA(object))
#define MLContainsAll(self, objects) MLSend(self, IS("contains-all*?"), IA(objects))
#define MLContainsAny(self, objects) MLSend(self, IS("contains-any*?"), IA(objects))

#define MLIsEmpty(self) MLSend(self, IS("is-empty?"), null)
#define MLIsInline(self) MLSend(self, IS("is-inline?"), null)
#define MLIsMutable(self) MLSend(self, IS("is-mutable?"), null)

#define MLAt(self, index) MLSend(self, IS("at*"), IA(index))
#define MLAtMany(self, indexes) MLSend(self, IS("at-many*"), IA(indexes))
#define MLAtCount(self, index, count) MLSend(self, IS("at*count*"), IA(index, count))

#define MLIndexOf(self, object) MLSend(self, IS("index-of*"), IA(object))
#define MLLastIndexOf(self, object) MLSend(self, IS("last-index-of*"), IA(object))
#define MLIndexesOf(self, object) MLSend(self, IS("indexes-of*"), IA(object))

#define MLFirst(self) MLSend(self, IS("first"), null)
#define MLFirstCount(self, count) MLSend(self, IS("first*"), IA(count))

#define MLLast(self) MLSend(self, IS("last"), null)
#define MLLastCount(self, count) MLSend(self, IS("last*"), IA(count))

#define MLSecond(self) MLSend(self, IS("second"), null)
#define MLThird(self) MLSend(self, IS("third"), null)
#define MLRest(self) MLSend(self, IS("rest"), null)

#define MLWith(self, object) MLSend(self, IS("with*"), IA(object))
#define MLWithMany(self, objects) MLSend(self, IS("with-many*"), IA(objects))

#define MLWithAt(self, object, index) MLSend(self, IS("with*at*"), IA(object, index))
#define MLWithManyAt(self, objects, index) MLSend(self, IS("with-many*at*"), IA(objects, index))

#define MLWithBefore(self, object, before) MLSend(self, IS("with*before*"), IA(object, before))
#define MLWithManyBefore(self, objects, before) MLSend(self, IS("with-many*before*"), IA(objects, before))

#define MLWithAfter(self, object, after) MLSend(self, IS("with*after*"), IA(object, after))
#define MLWithManyAfter(self, objects, after) MLSend(self, IS("with-many*after*"), IA(objects, after))

#define MLWithReplacing(self, replacement, object) MLSend(self, IS("with*replacing*"), IA(replacement, object))
#define MLWithManyReplacing(self, replacements, object) MLSend(self, IS("with-many*replacing*"), IA(replacements, object))

#define MLWithReplacingAt(self, replacement, index) MLSend(self, IS("with*replacing-at*"), IA(replacement, index))
#define MLWithManyReplacingAt(self, replacements, index) MLSend(self, IS("with-many*replacing-at*"), IA(replacements, index))

#define MLWithReplacingAtCount(self, replacement, index, count) MLSend(self, IS("with*replacing-at*count*"), IA(replacement, index, count))
#define MLWithManyReplacingAtCount(self, replacements, index, count) MLSend(self, IS("with-many*replacing-at*count*"), IA(replacements, index, count))

#define MLWithout(self, object) MLSend(self, IS("without*"), IA(object))
#define MLWithoutMany(self, object) MLSend(self, IS("without-many*"), IA(object))

#define MLWithoutAt(self, index) MLSend(self, IS("without-at*"), IA(index))
#define MLWithoutAtMany(self, indexes) MLSend(self, IS("without-at-many*"), IA(indexes))
#define MLWithoutAtCount(self, index, count) MLSend(self, IS("without-at*count*"), IA(index, count))

#define MLReversed(self) MLSend(self, IS("reversed"), null)
#define MLSorted(self) MLSend(self, IS("sorted"), null)

#define MLPluck(self, key) MLSend(self, IS("pluck*"), IA(key))

#define MLMin(self) MLSend(self, IS("min"), null)
#define MLMax(self) MLSend(self, IS("max"), null)

#define MLCapacity(self) MLSend(self, IS("capacity"), null)
#define MLIncreaseCapacity(self, capacity) MLSend(self, IS("increase-capacity*"), IA(capacity))

#define MLAdd(self, object) MLSend(self, IS("add*"), IA(object))
#define MLAddMany(self, objects) MLSend(self, IS("add-many*"), IA(objects))

#define MLInsert(self, object) MLSend(self, IS("insert*"), IA(object))
#define MLInsertMany(self, objects) MLSend(self, IS("insert-many*"), IA(objects))

#define MLInsertAt(self, object, index) MLSend(self, IS("insert*at*"), IA(object, index))
#define MLInsertManyAt(self, objects, index) MLSend(self, IS("insert-many*at*"), IA(objects, index))

#define MLInsertBefore(self, object, before) MLSend(self, IS("insert*before*"), IA(object, before))
#define MLInsertManyBefore(self, objects, before) MLSend(self, IS("insert-many*before*"), IA(objects, before))

#define MLInsertAfter(self, object, after) MLSend(self, IS("insert*after*"), IA(object, after))
#define MLInsertManyAfter(self, object, after) MLSend(self, IS("insert-many*after*"), IA(object, after))

#define MLReplaceWith(self, object, replacement) MLSend(self, IS("replace*with*"), IA(object, replacement))
#define MLReplaceWithMany(self, object, replacements) MLSend(self, IS("replace*with-many*"), IA(object, replacements))

#define MLReplaceAtWith(self, index, replacement) MLSend(self, IS("replace-at*with*"), IA(index, replacement))
#define MLReplaceAtWithMany(self, index, replacements) MLSend(self, IS("replace-at*with-many*"), IA(index, replacements))

#define MLReplaceAtCountWith(self, index, count, replacement) MLSend(self, IS("replace-at*count*with*"), IA(index, count, replacement))
#define MLReplaceAtCountWithMany(self, index, count, replacements) MLSend(self, IS("replace-at*count*with-many*"), IA(index, count, replacements))

#define MLRemove(self, object) MLSend(self, IS("remove*"), IA(object))
#define MLRemoveMany(self, objects) MLSend(self, IS("remove-many*"), IA(objects))

#define MLRemoveAt(self, index) MLSend(self, IS("remove-at*"), IA(index))
#define MLRemoveAtMany(self, indexes) MLSend(self, IS("remove-at-many*"), IA(indexes))
#define MLRemoveAtCount(self, index, count) MLSend(self, IS("remove-at*count*"), IA(index, count))
#define MLRemoveAll(self) MLSend(self, IS("remove-all"), null)

#define MLRemoveFirst(self, object) MLSend(self, IS("remove-first"), IA(object))
#define MLRemoveFirstCount(self, objects) MLSend(self, IS("remove-first*"), IA(objects))

#define MLRemoveLast(self, object) MLSend(self, IS("remove-last"), IA(object))
#define MLRemoveLastCount(self, objects) MLSend(self, IS("remove-last*"), IA(objects))

#define MLReverse(self) MLSend(self, IS("reverse"), null)
#define MLSort(self) MLSend(self, IS("sort"), null)

#define MLKeys(self) MLSend(self, IS("keys"), null)
#define MLValues(self) MLSend(self, IS("values"), null)

#define MLGet(self, key) MLSend(self, IS("get*"), IA(key))
#define MLGetMany(self, keys) MLSend(self, IS("get-many*"), IA(keys))

#define MLWithValue(self, key, value) MLSend(self, IS("with*value*"), IA(key, value))

#define MLPutValue(self, key, value) MLSend(self, IS("put*value*"), IA(key, value))
#define MLPutMany(self, valuesByKeys) MLSend(self, IS("put-many*"), IA(valuesByKeys))

#define MLIsYes(self) MLSend(self, IS("is-yes?"), null)
#define MLIsNo(self) MLSend(self, IS("is-no?"), null)

#define MLIsNaN(self) MLSend(self, IS("is-nan?"), null)
#define MLIsFinite(self) MLSend(self, IS("is-finite?"), null)
#define MLIsInfinite(self) MLSend(self, IS("is-infinite?"), null)

#define MLCodeAt(self, index) MLSend(self, IS("code-at*"), IA(index))
#define MLCodesAt(self, indexes) MLSend(self, IS("codes-at*"), IA(indexes))

#define MLCharacters(self) MLSend(self, IS("characters"), null)
#define MLCodes(self) MLSend(self, IS("codes"), null)

#define MLLines(self) MLSend(self, IS("lines"), null)
#define MLParagraphs(self) MLSend(self, IS("paragraphs"), null)

#define MLBeginsWith(self, prefix) MLSend(self, IS("begins-with*"), IA(prefix))
#define MLEndsWith(self, suffix) MLSend(self, IS("ends-with*"), IA(suffix))

#define MLUppercased(self) MLSend(self, IS("uppercased"), null)
#define MLLowercased(self) MLSend(self, IS("lowercased"), null)
#define MLCapitalized(self) MLSend(self, IS("capitalized"), null)

#define MLPrepend(self, prefix) MLSend(self, IS("prepend*"), IA(prefix))
#define MLAppend(self, suffix) MLSend(self, IS("append*"), IA(suffix))

#define MLDrain(self) MLSend(self, IS("drain"), null)

#endif
