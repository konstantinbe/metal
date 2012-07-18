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

#include "test.h"
#include <time.h>


void CRTestDateMake() {
    CRAssert(CRDate(123).payload.decimal == 123, "A date should store the milliseconds in its payload as decimal");
    CRAssert(CRDate(-123).payload.decimal == -123, "A date should store the negative milliseconds in its payload as decimal");

    CRAssert(CRDate(123.49999).payload.decimal == 123, "Creating a date with fractional milliseconds should round to nearest value");
    CRAssert(CRDate(123.50).payload.decimal == 124, "Creating a date with fractional milliseconds should round to nearest value");
}


void CRTestDateNow() {
    time_t seconds_before = 0;
    time_t seconds_after = 0;

    time(&seconds_before);
    var now = CRDateNow();
    time(&seconds_after);

    const CRDecimal seconds = CRDateSecondsSince1970(now);
    CRAssert(seconds_before <= seconds && seconds_after >= seconds, "CRDateNow() should return the current date and time");
}


void CRTestDateParse() {
    var date = CRDateParse(CRString("Some date format"));
    // TODO: implement.
}


// -----------------------------------------------------------------------------


void CRTestDateHash() {
    var date = CRDate(1330629046);
    CRAssert(CRHash(date) == date.payload.natural, "The hash of a date should be the bitpattern of its decimal milliseconds interpreted as CRNatural64");

    date.payload.natural = 0;
    CRAssert(CRHash(date) == date.payload.natural, "The hash of a date should be the bitpattern of its decimal milliseconds interpreted as CRNatural64");
}


void CRTestDateEquals() {
    var date1 = CRDate(123);
    var date2 = CRDate(123);
    var date3 = CRDate(124);
    CRAssert(CREquals(date1, date2), "Two equal dates should be equal");
    CRAssert(!CREquals(date1, date3), "Two not equal dates should be not equal");
}


void CRTestDateCopy() {
    var date = CRDate(123);
    CRAssert(CREquals(date, CRCopy(date)), "A copy of a date should be equal to its original");
}


void CRTestDateDescription() {
    const var date = CRDate(1330629046);
    const var description = CRDescription(date);
    CRAssert(CREquals(description, CRString("Thu Mar  1 19:10:46 2012\n")), "Description for date created with 1330629046 seconds since 1970 should be 'Thu Mar  1 19:10:46 2012'");
}


void CRTestDate() {
    CRTestDateMake();

    CRTestDateNow();
    CRTestDateParse();

    CRTestDateHash();
    CRTestDateEquals();
    CRTestDateCopy();
    CRTestDateDescription();
}
