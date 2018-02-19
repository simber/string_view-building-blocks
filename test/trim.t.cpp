#include "catch.hpp"
#include "test_config.hpp"
#include "svbb/trim.hpp"
#include "svbb/util.hpp"
#include "svbb/literals.hpp"

namespace {
using namespace SVBB_NAMESPACE;
using namespace SVBB_NAMESPACE::literals;

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
} // namespace
