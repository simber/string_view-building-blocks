#include "catch.hpp"
#include "config.hpp"
#include "svbb/tokenize.hpp"
#include "svbb/util.hpp"
#include <array>
#include <vector>

namespace {
using namespace SVBB_NAMESPACE;
using namespace SVBB_NAMESPACE::literals;

template<typename TokenRng>
void require_range_equal(TokenRng&& rng, const std::vector<string_view>& expected)
{
    using Catch::Matchers::Equals;
    std::vector<string_view> result(rng.begin(), rng.end());
    REQUIRE_THAT(result, Equals(expected));
}

template<typename TokenRng, size_t N>
SVBB_CXX14_CONSTEXPR bool rng_check(const TokenRng& rng,
                                    const std::array<typename TokenRng::view_type, N>& expected)
{
    auto it = rng.begin();
    for(size_t i = 0; i < N; ++i) {
        if(it == rng.end() || *it != expected[i]) return false;
        ++it;
    }
    return it == rng.end();
}

template<typename... T>
constexpr auto make_array(T&&... values)
    -> std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>
{
    return std::array<typename std::decay<typename std::common_type<T...>::type>::type,
                      sizeof...(T)>{std::forward<T>(values)...};
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
    static constexpr auto whitespace = " \t"_sv;
    static constexpr char delimeter = ',';
    require_range_equal(tokenize(""_sv, delimeter, whitespace), {});
    require_range_equal(tokenize(" "_sv, delimeter, whitespace), {});
    require_range_equal(tokenize("abc"_sv, delimeter, whitespace), {"abc"});
    require_range_equal(tokenize(",abc"_sv, delimeter, whitespace), {"", "abc"});
    require_range_equal(tokenize("  abc  ,"_sv, delimeter, whitespace), {"abc"});
    require_range_equal(tokenize("a,bc, def"_sv, delimeter, whitespace), {"a", "bc", "def"});
}
} // namespace
