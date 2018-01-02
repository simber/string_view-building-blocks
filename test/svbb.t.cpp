#define CATCH_CONFIG_MAIN
#include "config.hpp"
#include "svbb/svbb.hpp"
#include <algorithm>

namespace {

using namespace svbb;
using namespace svbb::literals;

template<typename TokenRng>
void require_range_equal(TokenRng&& rng, const std::vector<string_view>& expected)
{
    using Catch::Matchers::Equals;
    std::vector<string_view> result(rng.begin(), rng.end());
    REQUIRE_THAT(result, Equals(expected));
}

TEST_CASE("trim_left single character")
{
    REQUIRE(trim_left(""_sv, ' ') == "");
    REQUIRE(trim_left(" "_sv, ' ') == "");
    REQUIRE(trim_left(" a"_sv, ' ') == "a");
    REQUIRE(trim_left("  a"_sv, ' ') == "a");
    REQUIRE(trim_left("  a "_sv, ' ') == "a ");

    REQUIRE(trim_left(" "_sv, 'a') == " ");
    REQUIRE(trim_left("a"_sv, 'a') == "");

    REQUIRE(trim_left(" ab c "_sv, ' ') == "ab c ");
    REQUIRE(trim_left("  ab c "_sv, ' ') == "ab c ");

    REQUIRE(trim_left(" ab c "_sv, 'a') == " ab c ");
    REQUIRE(trim_left("ab c "_sv, 'a') == "b c ");
}

TEST_CASE("trim_left multiple characters")
{
    REQUIRE(trim_left(""_sv, ""_sv) == "");
    REQUIRE(trim_left(" \t abcd "_sv, ""_sv) == " \t abcd ");
    REQUIRE(trim_left(" \t abcd "_sv, " \t"_sv) == "abcd ");
    REQUIRE(trim_left(" \t abcd "_sv, " \tabcd"_sv) == "");
}

TEST_CASE("trim_right single character")
{
    REQUIRE(trim_right(""_sv, ' ') == "");
    REQUIRE(trim_right(" "_sv, ' ') == "");
    REQUIRE(trim_right("a "_sv, ' ') == "a");
    REQUIRE(trim_right("a  "_sv, ' ') == "a");
    REQUIRE(trim_right(" a  "_sv, ' ') == " a");

    REQUIRE(trim_right(" "_sv, 'a') == " ");
    REQUIRE(trim_right("a"_sv, 'a') == "");

    REQUIRE(trim_right(" ab c "_sv, ' ') == " ab c");
    REQUIRE(trim_right(" ab c  "_sv, ' ') == " ab c");

    REQUIRE(trim_right(" ab c "_sv, 'c') == " ab c ");
    REQUIRE(trim_right(" ab c"_sv, 'c') == " ab ");
}

TEST_CASE("trim_right multiple characters")
{
    REQUIRE(trim_right(""_sv, ""_sv) == "");
    REQUIRE(trim_right(" \t abcd "_sv, ""_sv) == " \t abcd ");
    REQUIRE(trim_right(" \t abcd \t"_sv, " \t"_sv) == " \t abcd");
    REQUIRE(trim_right(" \t abcd "_sv, " \tabcd"_sv) == "");
}

TEST_CASE("trim single character")
{
    REQUIRE(trim(""_sv, ' ') == "");
    REQUIRE(trim(" "_sv, ' ') == "");
    REQUIRE(trim("a "_sv, ' ') == "a");
    REQUIRE(trim("a  "_sv, ' ') == "a");
    REQUIRE(trim(" a  "_sv, ' ') == "a");

    REQUIRE(trim(" "_sv, 'a') == " ");
    REQUIRE(trim("a"_sv, 'a') == "");

    REQUIRE(trim(" ab c "_sv, ' ') == "ab c");
    REQUIRE(trim(" ab c  "_sv, ' ') == "ab c");

    REQUIRE(trim(" ab c "_sv, 'c') == " ab c ");
    REQUIRE(trim(" ab c"_sv, 'c') == " ab ");
    REQUIRE(trim("cab c"_sv, 'c') == "ab ");
}

TEST_CASE("trim multiple characters")
{
    REQUIRE(trim(""_sv, ""_sv) == "");
    REQUIRE(trim(" \t abcd "_sv, ""_sv) == " \t abcd ");
    REQUIRE(trim(" \t abcd \t"_sv, " \t"_sv) == "abcd");
    REQUIRE(trim(" \t abcd "_sv, " \tabcd"_sv) == "");
}

TEST_CASE("split around position")
{
    REQUIRE(split_around(""_sv, 0) == make_split(""_sv, ""_sv));
    REQUIRE(split_around("abc"_sv, 0) == make_split(""_sv, "bc"_sv));
    REQUIRE(split_around("abc"_sv, 1) == make_split("a"_sv, "c"_sv));
    REQUIRE(split_around("abc"_sv, 2) == make_split("ab"_sv, ""_sv));
    REQUIRE(split_around("abc"_sv, 3) == make_split("abc"_sv, ""_sv));
}

TEST_CASE("split around range")
{
    REQUIRE(split_around(""_sv, 0, 1) == make_split(""_sv, ""_sv));
    REQUIRE(split_around("abc"_sv, 0, 1) == make_split(""_sv, "bc"_sv));
    REQUIRE(split_around("abc"_sv, 0, 2) == make_split(""_sv, "c"_sv));
    REQUIRE(split_around("abc"_sv, 1) == make_split("a"_sv, "c"_sv));
    REQUIRE(split_around("abc"_sv, 2) == make_split("ab"_sv, ""_sv));
    REQUIRE(split_around("abc"_sv, 3) == make_split("abc"_sv, ""_sv));
}

TEST_CASE("cut token")
{
    REQUIRE(cut_token(""_sv, ' ') == make_split(""_sv, ""_sv));
    REQUIRE(cut_token(" ab"_sv, ' ') == make_split(""_sv, "ab"_sv));
    REQUIRE(cut_token("ab"_sv, ' ') == make_split("ab"_sv, ""_sv));
    REQUIRE(cut_token("ab "_sv, ' ') == make_split("ab"_sv, ""_sv));
    REQUIRE(cut_token("ab cd"_sv, ' ') == make_split("ab"_sv, "cd"_sv));
    REQUIRE(cut_token("ab cd ef"_sv, ' ') == make_split("ab"_sv, "cd ef"_sv));
}
TEST_CASE("cut token with quotes")
{
    constexpr char delimeter = ' ';
    constexpr char quoter = '"';
    constexpr auto whitespace = ""_sv;

    REQUIRE(cut_token(""_sv, delimeter, quoter, whitespace) == make_split(""_sv, ""_sv));
    REQUIRE(cut_token(" ab"_sv, delimeter, quoter, whitespace) == make_split(""_sv, "ab"_sv));
    REQUIRE(cut_token("ab"_sv, delimeter, quoter, whitespace) == make_split("ab"_sv, ""_sv));
    REQUIRE(cut_token("ab "_sv, delimeter, quoter, whitespace) == make_split("ab"_sv, ""_sv));
    REQUIRE(cut_token("ab cd"_sv, delimeter, quoter, whitespace) == make_split("ab"_sv, "cd"_sv));
    REQUIRE(cut_token("ab cd ef"_sv, delimeter, quoter, whitespace) ==
            make_split("ab"_sv, "cd ef"_sv));

    REQUIRE(cut_token("\"ab cd\" ef"_sv, delimeter, quoter, whitespace) ==
            make_split("ab cd"_sv, "ef"_sv));
    REQUIRE(cut_token("\"ab \"\"cd\" ef"_sv, delimeter, quoter, whitespace) ==
            make_split("ab \"\"cd"_sv, "ef"_sv));
}

TEST_CASE("tokenize")
{
    require_range_equal(tokenize(""_sv, ','), {});
    require_range_equal(tokenize("abc"_sv, ','), {"abc"});
    require_range_equal(tokenize(",abc"_sv, ','), {"", "abc"});
    require_range_equal(tokenize("abc,"_sv, ','), {"abc"});
    require_range_equal(tokenize("a,bc, def"_sv, ','), {"a", "bc", " def"});
}

TEST_CASE("tokenize trimmed")
{
    constexpr auto whitespace = " \t"_sv;
    constexpr char delimeter = ',';
    require_range_equal(tokenize(""_sv, delimeter, whitespace), {});
    require_range_equal(tokenize(" "_sv, delimeter, whitespace), {});
    require_range_equal(tokenize("abc"_sv, delimeter, whitespace), {"abc"});
    require_range_equal(tokenize(",abc"_sv, delimeter, whitespace), {"", "abc"});
    require_range_equal(tokenize("  abc  ,"_sv, delimeter, whitespace), {"abc"});
    require_range_equal(tokenize("a,bc, def"_sv, delimeter, whitespace), {"a", "bc", "def"});
}

} // namespace
