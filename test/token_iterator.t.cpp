#include "catch.hpp"
#include "test_config.hpp"
#include "svbb/token_iterator.hpp"
#include "svbb/tokenize.hpp"
#include "svbb/util.hpp"
#include <vector>
#include <algorithm>

namespace {
using namespace SVBB_NAMESPACE;

TEST_CASE("equality")
{
    const auto first_input = make_view("a b c");
    const auto splitter = split_by_char<char>(' ');

    REQUIRE(begin_tokens(first_input, splitter) == begin_tokens(first_input, splitter));
    REQUIRE(end_tokens(first_input, splitter) == end_tokens(first_input, splitter));
    REQUIRE(begin_tokens(first_input, splitter) != end_tokens(first_input, splitter));
}

TEST_CASE("copy")
{
    const auto first_input = make_view("a b c");
    const auto splitter = split_by_char<char>(' ');

    std::vector<string_view> tokens;
    std::copy(begin_tokens(first_input, splitter), end_tokens(first_input, splitter),
              std::back_inserter(tokens));

    using Catch::Matchers::Equals;
    REQUIRE_THAT(tokens, Equals(std::vector<string_view>{"a", "b", "c"}));
}
} // namespace
