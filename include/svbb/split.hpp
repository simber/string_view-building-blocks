#pragma once
#include "config.hpp"
#include <algorithm>

namespace SVBB_NAMESPACE {

template<typename CharT, typename Traits = std::char_traits<CharT>>
struct split_result
{
    basic_string_view<CharT, Traits> left;
    basic_string_view<CharT, Traits> right;
};

template<typename CharT, typename Traits>
SVBB_CONSTEXPR auto make_split(basic_string_view<CharT, Traits> left,
                               basic_string_view<CharT, Traits> right)
    -> split_result<CharT, Traits>
{
    return {left, right};
}

template<typename CharT, typename Traits>
SVBB_CONSTEXPR bool operator==(const split_result<CharT, Traits>& lhs,
                               const split_result<CharT, Traits>& rhs)
{
    return lhs.left == rhs.left && lhs.right == rhs.right;
}

template<typename CharT, typename Traits>
inline std::ostream& operator<<(std::ostream& os, const split_result<CharT, Traits>& splitted)
{
    return os << splitted.left << " " << splitted.right;
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto split_around(basic_string_view<CharT, Traits> input, size_t start,
                                       size_t length) -> split_result<CharT, Traits>
{
    return {input.substr(0, start), input.substr(std::min(start + length, input.size()))};
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto split_around(basic_string_view<CharT, Traits> input, size_t pos)
    -> split_result<CharT, Traits>
{
    return split_around(input, pos, 1);
}
} // namespace SVBB_NAMESPACE
