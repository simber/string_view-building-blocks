#include <iostream>
#include "config.hpp" // for setting up which string_view implementation is used
#include "svbb/tokenize.hpp"
#include "svbb/util.hpp" // svbb::make_view

int main()
{
    using svbb::make_view;
    using svbb::tokenize;

    const auto view = make_view("cat,shark,dog,platypus,cat");

    for(const auto token : tokenize(view, ',')) {
        if(token.size() > 3) std::cout << token << '\n';
    }
}
