#include "catch.hpp"
#include "test_config.hpp"
#include "svbb/split.hpp"
#include "svbb/util.hpp"

namespace {
using namespace SVBB_NAMESPACE;
using namespace SVBB_NAMESPACE::literals;

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
} // namespace
