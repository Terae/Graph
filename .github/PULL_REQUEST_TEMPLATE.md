[Describe your pull request here. Please read the text below the line, and make sure you follow the checklist.]

* * *

## Pull request checklist
- [ ] Changes are described in the pull request, or an [existing issue is referenced](https://github.com/terae/graoh/issues).
- [ ] The test suite [compiles and runs](https://github.com/terae/graph#execute-unit-tests) without error.
- [ ] [Code coverage](https://codecov.io/gh/terae/graph) is 100%. Test cases can be added by editing the [test suite](https://github.com/terae/graph/tree/dev/test/src).

## Please don't
* The C++ support varies between different **compilers** and versions. Please note the [list of supported compilers](https://github.com/terae/graph#supported-compilers). Some compilers like GCC 4.9 (and earlier), Clang 3.7 (and earlier) or Microsoft Visual Studio not work on my library. Please refrain proposing changes that work around these compiler's limitations with `#ifdef` or other means.
* Please do not open pull requests that address **multiple issues**.
