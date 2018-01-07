#include "example_config.hpp" // for setting up which string_view implementation is used
#include "svbb/tokenize.hpp"
#include "svbb/util.hpp" // svbb::make_view

namespace ctx {
template<typename Rng, typename T>
constexpr int count(const Rng& rng, const T& value)
{
    int ret = 0;
    auto first = rng.begin();
    auto last = rng.end();
    for(; first != last; ++first) {
        if(*first == value) {
            ret++;
        }
    }
    return ret;
}
} // namespace ctx

int main()
{
    using svbb::tokenize;
    using namespace svbb::literals;

    constexpr auto view = "cat,shark  ,dog, cat, platypus ,cat"_svc;
    constexpr auto delimeter = ',';
    constexpr auto whitespace = " \t"_svc;

    static_assert(ctx::count(tokenize(view, delimeter, whitespace), "cat"_svc) == 3);
}
