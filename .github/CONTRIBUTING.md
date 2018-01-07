[![Issue Stats](http://issuestats.com/github/terae/graph/badge/pr?style=flat)](http://issuestats.com/github/terae/graph)

# How to contribute
If you want to improve this library by proposing features, finding bugs or even fixing my mistakes, here are a few guidelines which should help us to make it as easy as possible to help us avoiding all kinds of unnecessary work or disappointment.
Of course, this document is subject to discussion, so please [create an issue](https://github.com/terae/graph/issues/new) or a pull request if you find a way to improve it!

## Private reports
Usually, all issues are tracked publicly on [GitHub](https://github.com/terae/graph/issues). If you want to make a private report (e.g., for a vulnerability or to attach an example that is not meant to be published), please send an email to <bigey@etud.insa-toulouse.fr>.

## Prerequisites
Please [create an issue](https://github.com/nlohmann/terae/graph/new), assuming one does not already exists, and describe your concern. Note you need a [GitHub account](https://github.com/) for this.

## Describe your issue
Clearly describe the issue:
* If it is a bug, please describe how to **reproduce** it. If possible, attach a complete example which demonstrates the error. Please also state what you **expected** to happen instead of the error.
* If you propose a change or addition, try to give an **example** how the improved code could look like or how to use it.
* If you found a compilation error, please tell me which **compiler** (version and operator system) you used and paste the (relevant part of) the error messages to the ticket.

Please stick the [issue template](https://github.com/terae/graph/blob/dev/.github/ISSUE_TEMPLATE.md) if possible.

## Files to change
There are some files which need to be edited:
1. [src/*](https://github.com/terae/graph/blob/dev/src)
2. [test/src/unit-*.cpp](https://github.com/terae/graph/blob/dev/test/src) - These files contain the [Catch](https://github.com/catchorg/Catch2) unit tests which currently cover [100%](https://codecov.io/gh/Terae/Graph) of the library's code.

If you add or change a feature, please also add a unit test to this file. The unit tests can be compiled and executed with

```c++
mkdir build
cd build
cmake ..
make all -j 4
ctest
```

The test cases are also executed with several different compilers on [Travis](https://travis-ci.org/terae/graph) once you open a pull request.

## Note
* If you open a pull request, the code will be automatically tested with [Valgrind](http://valgrind.org/)'s Memcheck tool to detect memory leaks. Please be aware that the execution with Valgrind _may_ in rare cases yield different behaviour than running the code directly. This can result in failing unit tests which run successfully without Valgrind.
<!-- * There is a Makefile target `make pretty` which runs [Artistic Style](http://astyle.sourceforge.net/) to fix indentation. If possible, run it before opening the pull request. Otherwise, I shall run it afterward. -->

## Please don't
* Please do not open pull requests that address **multiple issues**.

## Wanted
The following areas really need contribution:
* Extending the **continuous integration** toward more exotic compilers such as Android NKD, Intel's Compiler, MSVC or the bleeding-edge version of GCC or Clang.
* Creating **benchmarks** to include this library comparing to others. Though efficiency is not everything, speed and memory cosumptions are very important characteristics for C++ developers, so having proper comparisons would be interesting.
