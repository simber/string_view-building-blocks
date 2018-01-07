#pragma once
#include "config.hpp"
#include "char_traits.hpp"

namespace SVBB_NAMESPACE {

template<typename CharT, typename Traits, typename Allocator>
auto make_view(const std::basic_string<CharT, Traits, Allocator>& str)
    -> basic_string_view<CharT, Traits>
{
    return basic_string_view<CharT, Traits>{str};
}

template<typename CharT, typename Traits, typename Allocator>
auto make_view(std::basic_string<CharT, Traits, Allocator>&& str)
    -> basic_string_view<CharT, Traits>
{
    static_assert(sizeof(CharT) != sizeof(CharT),
                  "You are trying to create a view to an rvalue string.");
}

template<typename CharT, typename Traits = std::char_traits<CharT>>
SVBB_CONSTEXPR auto make_view(const CharT* str) -> basic_string_view<CharT, Traits>
{
    return basic_string_view<CharT, Traits>{str};
}

template<typename CharT, typename Traits = std::char_traits<CharT>>
SVBB_CONSTEXPR auto make_view(const CharT* str, size_t len) -> basic_string_view<CharT, Traits>
{
    return basic_string_view<CharT, Traits>{str, len};
}

namespace literals {

SVBB_CONSTEXPR string_view operator""_sv(const char* str, std::size_t len) noexcept
{
    return string_view{str, len};
}

constexpr auto operator""_svc(const char* str, std::size_t len) noexcept
    -> basic_string_view<char, constexpr_char_traits<char>>
{
    return basic_string_view<char, constexpr_char_traits<char>>{str, len};
}
} // namespace literals
} // namespace SVBB_NAMESPACE
