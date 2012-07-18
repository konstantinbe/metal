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
//
// Many thanks to Mike Ash for his excellent article: C Macro Tips and Tricks
// http://www.mikeash.com/pyblog/friday-qa-2010-12-31-c-macro-tips-and-tricks.html

#ifndef CR_MACROS_H
#define CR_MACROS_H

#include <stdlib.h>
#include <stdio.h>

#define CRDebug(message, ...) CRLog("DEBUG", message, ## __VA_ARGS__)
#define CRInfo(message, ...) CRLog("INFO", message, ## __VA_ARGS__)
#define CRWarning(message, ...) { CRLog("WARNING", message, ## __VA_ARGS__); CRDereferenceNullPointer(); }
#define CRError(message, ...) { CRLog("ERROR", message, ## __VA_ARGS__); CRDereferenceNullPointer(); }

#define CRFail(message, ...) { CRLog("FAIL", message, ## __VA_ARGS__); CRDereferenceNullPointer(); };
#define CRAssert(condition, message, ...) if (!(condition)) { CRLog("ASSERTION FAILURE", message, ## __VA_ARGS__); CRDereferenceNullPointer(); };

#pragma mark - Private

#define CRLog(level, message, ...) printf("[%s] %s:%d %s --- " #message "\n", level, __FILE__, __LINE__, __FUNCTION__, ## __VA_ARGS__);

#if DEBUG && !(defined __clang_analyzer__)
    #define CRDereferenceNullPointer() { CRInteger* pointer = NULL; *pointer = 0; }
#else
    #define CRDereferenceNullPointer() { /* Do nothing. */ }
#endif

#define CRPointerSize(x) ((__typeof__(x) []){ x })

#endif
