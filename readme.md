# Metal [![Build Status](https://travis-ci.org/konstantinbe/metal.png)](https://travis-ci.org/konstantinbe/metal)

Experimental prototype-based object model runtime in C99. Inspired by
Objective-C, Lua, and \[[Piumarta-and-Warth-2007](http://piumarta.com/software/cola/objmodel2.pdf)\].

## Getting Started

* Install [GCC](http://gcc.gnu.org) or [Clang](http://clang.llvm.org)
* Install [Ruby](http://www.ruby-lang.org/)
* Install [Rake](http://rake.rubyforge.org)
* Install [Git](http://git-scm.com)
* Clone repo: `git clone git@github.com:konstantinbe/metal.git`
* Build & test: `rake test`

For debugging (optional):

* Install [GDB](http://www.gnu.org/software/gdb/) or [LLDB](http://lldb.llvm.org)
* Build & debug: `rake debug`

For profiling (optional):

* Install [Gcov](http://gcc.gnu.org/onlinedocs/gcc/Gcov.html)
* Build & profile: `rake profile`

For memory analysis (optional):

* Install [Valgrind](http://valgrind.org)
* Build & run memory analysis: `rake valgrind`

## Conventions

* 4 spaces, no tabs
* Use [compact control readability style](http://en.wikipedia.org/wiki/Indent_style#Compact_control_readability_style)
* Use dashed notation for methods:
  `replace-at*with-many*`, `is-mutable?`
* Use UPPERCASE notation for global constants:
  `GREATER_THAN`
* Use CamelCase for everything else in code (types, functions, variables, ...):
  `IndexOf`, `instanceVariableName`
* Use dashed notation for directories and files:
  `array.c`, `test-array.c`
* Use dashed notation for URLs:
  `http://konstantinbender.com/introducing-the-metal-runtime`
* Use dashed notation for XML, HTML and CSS: `person-name`, `table-of-people`,
  `regular-xml-parser`
* No arbitrary abbreviations, such as `src`, `mod`, `obj`, ...
* Source must be "white-space-clean", the [white](https://github.com/konstantinbe/white)
  command line utility will cleanup whitespace for you

## License

Released under the MIT license.

Copyright (c) 2012 [Konstantin Bender](http://konstantinbender.com).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
