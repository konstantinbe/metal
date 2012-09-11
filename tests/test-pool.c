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

#include "test.h"


void MLTestPool() {
    var pool = MLNew(MLPool);
    var object = MLNew(MLNumber);
    MLAssertEquals(MLRetainCount(object), N(1), "A new object should have its retain count be 1");

    MLRetain(object);
    MLAssertEquals(MLRetainCount(object), N(2), "After retaining an object its retain count should increase by 1");

    MLAutorelease(object);
    MLAssertEquals(MLRetainCount(object), N(2), "After autoreleasing an object its retain count should remain the same");

    MLDrain(pool);
    MLAssertEquals(MLRetainCount(object), N(1), "After draining the pool the retain count of an autoreleased object should be decreased by 1");
}
