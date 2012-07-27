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


#include "core.h"

#include "macros.h"
#include "string.h"

#include <string.h>
#include <assert.h>


int placeholder = 0;


// void CRInitClass(var class, var name, var superclass) {
//     struct CRClass* klass = (struct CRClass*)class.pointer;
//     struct CRClass* root = (struct CRClass*)CRObject.pointer;
//     struct CRClass* super = (struct CRClass*)superclass.pointer;
//     struct CRClass* meta = CRAllocateAndClear(sizeof(struct CRClass));
//
//     const char* meta_suffix = "MetaClass";
//     const CRNatural name_length = strlen(name.pointer);
//     const CRNatural meta_suffix_length = strlen(meta_suffix);
//
//     char* class_name = (char *)CRAllocate(name_length + 1);
//     char* meta_name = (char *)CRAllocate(name_length + meta_suffix_length + 1);
//
//     strcpy(class_name, name.pointer);
//     strcpy(meta_name, name.pointer);
//     strcat(meta_name, meta_suffix);
//
//     if (klass->class != NULL) CRError("Can't create class %s, class already exists under the name %s", (char *)class_name, (char *)klass->name.pointer);
//
//     // init meta klass
//     meta->class = meta;
//     meta->retain_count = CRRetainCountMax;
//     meta->name = CRStringCreateWithCharacters(meta_name);
//     meta->superclass = CRReference(root);
//
//     // init klass
//     klass->class = meta;
//     klass->retain_count = CRRetainCountMax;
//     klass->name = CRStringCreateWithCharacters(class_name);
//     klass->superclass = superclass;
// }
//
//
// void CRAddMethod(var class, var command, CRCode code) {
//     // check first character of signature if it is '+' or '-'
//     // if '+', then its a class method
//     // if '-' then its an instance method
//     // throw error if its neither '+' nor '-'
//     // add method to the appropriate method dictionary
// }
//
//
// var CRSend(var self, var command, var arguments) {
//     // if (self.pointer == NULL) self = null;
//     //
//     // // TODO: lookup in cache.
//     //
//     // const struct CRObject* that = self.pointer;
//     // const struct CRBehavior* class = that->class;
//     // const CRCode lookup = class->lookup;
//     // const var method = lookup(self, command, arguments);
//     // const CRCode code = method.payload.code;
//     //
//     // // TODO: send message resolve* if not found.
//     // // TODO: store found method in cache if found or resolved.
//     // // TODO: send method_not_found* if not found and not resolved.
//     // assert(code);
//     //
//     // return code(self, command, arguments);
//
//     return null;
// }
