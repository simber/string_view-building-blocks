# string_view-building-blocks [![Build status](https://ci.appveyor.com/api/projects/status/a17s02nvx5xy5tdd?svg=true)](https://ci.appveyor.com/project/simber/string-view-building-blocks)
C++ string_view extension library.
## Summary
This library provides basic functions for splitting and trimming a `std::string_view` which comes with `C++17`. It can also be used with older compilers (at least `C++11`) and a separate but standard conforming `string_view`  implementation like `boost::string_view`.

## Motivation

#### How to split a string in C++:
- [How to split a string in C++](https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/) 
- [The most elegant way to iterate the words of a string](https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string)

#### How to split a `string_view` with svbb:
```c++
using svbb::make_view;
using svbb::string_view;
using svbb::tokenize;

const string_view view = make_view("cat,shark  ,dog, platypus ,cat");
const char delimeter = ',';
const string_view whitespace = make_view(" \t");

// svbb::tokenize creates a lazy input range
for(string_view token : tokenize(view, delimeter, whitespace)) {
    std::cout << "Substring: \"" << token << "\"\n";
}
```

#### Bonus: How to `constexpr` split a `string_view` with svbb:
```c++
using svbb::tokenize;
using namespace svbb::literals;

// svbb::_svc literal creates a string_view with constexpr char_traits
constexpr auto view = "cat,shark  ,dog, cat, platypus ,cat"_svc;
constexpr auto delimeter = ',';
constexpr auto whitespace = " \t"_svc;
constexpr auto token_range = tokenize(view, delimeter, whitespace);

static_assert(ctx::count(token_range, "cat"_svc) == 3);
```
Note:
* This compiles on Visual Studio 15.5 with `"/std:c++latest"`.
* `string_view` is not meant as a compile time string, so this usage is only a by-product of putting `constexpr` everywhere.

## Usage
- Make the contents of the include folder known to your target.
- `#include "svbb/svbb.hpp"`

For compatibility with older compilers you have to use a different `string_view` implementation like [boost >= 1.61 ](http://www.boost.org/doc/libs/1_61_0/boost/utility/string_view.hpp). There is also a [standalone version](https://github.com/tcbrindle/cpp17_headers).

```c++
#include "boost/utility/string_view.hpp"
#define SVBB_STRING_VIEW_IMPL boost // let svbb know which string_view to use
#define SVBB_NO_CXX14_CONSTEXPR // if your compiler doesn't fully support C++14 constexpr
#define SVBB_NO_CONSTEXPR // if your compiler doesn't fully support C++11 constexpr
#define SVBB_NAMESPACE my_namespace // if you want another namespace than svbb
#include "svbb/svbb.hpp"
```

