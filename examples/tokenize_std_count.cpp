#include <iostream>
#include <algorithm>
#include "example_config.hpp" // for setting up which string_view implementation is used
#include "svbb/tokenize.hpp"
#include "svbb/util.hpp" // svbb::make_view

int main()
{
    using svbb::make_view;
    using svbb::tokenize;

    const auto view = make_view("cat,shark,dog,platypus,cat");

    auto token_range = tokenize(view, ',');
    const auto cat_count = std::count(token_range.begin(), token_range.end(), make_view("cat"));
    std::cout << "cat_count=" << cat_count << '\n';
}
