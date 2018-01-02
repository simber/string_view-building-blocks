#pragma once

#include <array>
#include <cassert>
#define SVBB_ASSERT(x) assert(x)

#ifdef SVBB_NO_CXX14_CONSTEXPR
#define SVBB_CXX14_CONSTEXPR inline
#else
#define SVBB_CXX14_CONSTEXPR constexpr
#endif

#ifndef SVBB_STRING_VIEW_IMPL
#include <string_view>
#define SVBB_STRING_VIEW_IMPL std
#endif // !SVBB_STRING_VIEW_IMPL

namespace svbb {

template<typename CharT, typename Traits>
using basic_string_view = SVBB_STRING_VIEW_IMPL::basic_string_view<CharT, Traits>;
using string_view = SVBB_STRING_VIEW_IMPL::string_view;
// namespace detail {
// SVBB_CXX14_CONSTEXPR string_view remove_prefix(string_view view, size_t n) noexcept
//{
//    return string_view{view.data() + n, view.size() - n};
//}
// SVBB_CXX14_CONSTEXPR string_view remove_suffix(string_view view, size_t n) noexcept
//{
//    return string_view{view.data(), view.size() - n};
//}
//} // namespace detail

// template<typename CharT, typename Traits, typename Allocator>
// auto make_string_view(const std::basic_string<CharT, Traits, Allocator>& str)
//    -> basic_string_view<CharT, Traits>
//{
//    return basic_string_view<CharT, Traits>{str};
//}
//
// template<typename CharT, typename Traits, typename Allocator>
// auto make_string_view(std::basic_string<CharT, Traits, Allocator>&& str)
//    -> basic_string_view<CharT, Traits>
//{
//    static_assert(sizeof(CharT) != sizeof(CharT),
//                  "If you get an error here. You are trying to pass an rvalue string to a function
//                  " "that returns a string_view.");
//}
//
// template<typename CharT, typename Traits = std::char_traits<CharT>>
// constexpr auto make_string_view(const CharT* str) -> basic_string_view<CharT, Traits>
//{
//    return basic_string_view<CharT, Traits>{str};
//}
//
// template<typename CharT, typename Traits = std::char_traits<CharT>>
// constexpr auto make_string_view(const CharT* str, size_t len) -> basic_string_view<CharT, Traits>
//{
//    return basic_string_view<CharT, Traits>{str, len};
//}

namespace literals {
constexpr string_view operator""_sv(char const* str, std::size_t len) noexcept
{
    return string_view{str, len};
}
} // namespace literals

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto trim_left(basic_string_view<CharT, Traits> to_trim, CharT with)
    -> basic_string_view<CharT, Traits>
{
    to_trim.remove_prefix(std::min(to_trim.find_first_not_of(with), to_trim.size()));
    return to_trim;
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto trim_left(basic_string_view<CharT, Traits> to_trim,
                                    basic_string_view<CharT, Traits> with)
    -> basic_string_view<CharT, Traits>
{
    to_trim.remove_prefix(std::min(to_trim.find_first_not_of(with), to_trim.size()));
    return to_trim;
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto trim_right(basic_string_view<CharT, Traits> to_trim, CharT with)
    -> basic_string_view<CharT, Traits>
{
    to_trim.remove_suffix(
        std::min(to_trim.size() - to_trim.find_last_not_of(with) - 1, to_trim.size()));
    return to_trim;
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto trim_right(basic_string_view<CharT, Traits> to_trim,
                                     basic_string_view<CharT, Traits> with)
    -> basic_string_view<CharT, Traits>
{
    to_trim.remove_suffix(
        std::min(to_trim.size() - to_trim.find_last_not_of(with) - 1, to_trim.size()));
    return to_trim;
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto trim(basic_string_view<CharT, Traits> to_trim, CharT with)
    -> basic_string_view<CharT, Traits>
{
    return trim_left(trim_right(to_trim, with), with);
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto trim(basic_string_view<CharT, Traits> to_trim,
                               basic_string_view<CharT, Traits> with)
    -> basic_string_view<CharT, Traits>
{
    return trim_left(trim_right(to_trim, with), with);
}

template<typename CharT, typename Traits = std::char_traits<CharT>>
struct split_result
{
    basic_string_view<CharT, Traits> left;
    basic_string_view<CharT, Traits> right;
};

template<typename CharT, typename Traits>
constexpr auto make_split(basic_string_view<CharT, Traits> left,
                          basic_string_view<CharT, Traits> right) -> split_result<CharT, Traits>
{
    return {left, right};
}

template<typename CharT, typename Traits>
constexpr bool operator==(const split_result<CharT, Traits>& lhs,
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

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR size_t find_first_of(basic_string_view<CharT, Traits> input, CharT delimeter)
{
    for(auto iter = input.begin(), end = input.end(); iter != end; ++iter) {
        if(*iter == delimeter) return std::distance(input.begin(), iter);
    }
    return basic_string_view<CharT, Traits>::npos;
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto cut_token(basic_string_view<CharT, Traits> input, CharT delimeter)
    -> split_result<CharT, Traits>
{
    return split_around(input, std::min(find_first_of(input, delimeter), input.size()));
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR size_t find_first_of_not_quoted(basic_string_view<CharT, Traits> input,
                                                     CharT delimeter, CharT quoter)
{
    SVBB_ASSERT(quoter != delimeter);
    bool is_quoted = false;
    for(auto iter = input.begin(), end = input.end(); iter != end; ++iter) {
        if(*iter == quoter)
            is_quoted = !is_quoted;
        else if(!is_quoted && *iter == delimeter)
            return std::distance(input.begin(), iter);
    }
    return basic_string_view<CharT, Traits>::npos;
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto cut_token(basic_string_view<CharT, Traits> input, CharT delimeter,
                                    CharT quoter, basic_string_view<CharT, Traits> whitespace)
    -> split_result<CharT, Traits>
{
    SVBB_ASSERT(quoter != delimeter);
    const auto splitted = split_around(
        input, std::min(find_first_of_not_quoted(input, delimeter, quoter), input.size()));

    return {trim(trim(splitted.left, whitespace), quoter), splitted.right};
}

namespace detail {

template<typename CharT, typename Traits, typename Splitter>
class token_range_state
{
public:
    using view_type = basic_string_view<CharT, Traits>;

    constexpr token_range_state(view_type input, Splitter splitter)
        : data_{view_type{}, input}, splitter_(splitter)
    {
    }

    constexpr view_type token() const noexcept { return data_.left; }
    constexpr view_type remainder() const noexcept { return data_.right; }
    constexpr bool empty() const noexcept { return remainder().empty() && token().empty(); }
    SVBB_CXX14_CONSTEXPR void split() { data_ = splitter_(remainder()); }

private:
    split_result<CharT, Traits> data_;
    Splitter splitter_;
};

template<typename CharT, typename Traits, typename Splitter>
class token_iterator
{
public:
    using view_type = basic_string_view<CharT, Traits>;
    using state_type = token_range_state<CharT, Traits, Splitter>;
    using value_type = view_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const view_type*;
    using reference = view_type;
    using iterator_category = std::input_iterator_tag;

    constexpr token_iterator() noexcept : state_{nullptr} {};
    SVBB_CXX14_CONSTEXPR token_iterator(state_type* data) : state_{data} { advance(); }

    constexpr reference operator*() const noexcept { return state_->token(); }
    SVBB_CXX14_CONSTEXPR token_iterator& operator++()
    {
        advance();
        return *this;
    }

    SVBB_CXX14_CONSTEXPR token_iterator operator++(int)
    {
        token_iterator tmp = *this;
        advance();
        return tmp;
    }

    constexpr bool operator==(const token_iterator& rhs) const noexcept
    {
        return state_ == nullptr && rhs.state_ == nullptr;
    }
    constexpr bool operator!=(const token_iterator& rhs) const noexcept { return !(*this == rhs); }

private:
    SVBB_CXX14_CONSTEXPR void advance()
    {
        state_->split();
        if(state_->empty()) state_ = nullptr;
    }

    state_type* state_;
};

template<typename CharT, typename Traits, typename Splitter>
class token_range
{
public:
    using iterator = token_iterator<CharT, Traits, Splitter>;
    using const_iterator = iterator;
    using state_type = typename iterator::state_type;
    using view_type = typename iterator::view_type;

    constexpr token_range(view_type view, Splitter splitter)
        : state_{view, splitter}, begin_{&state_}
    {
    }

    constexpr auto begin() const noexcept -> iterator { return begin_; }
    constexpr auto end() const noexcept -> iterator { return iterator{}; }

private:
    state_type state_;
    iterator begin_;
};

template<typename CharT>
struct basic_splitter
{
    constexpr explicit basic_splitter(CharT delimeter) : delimeter(delimeter) {}

    template<typename Traits>
    SVBB_CXX14_CONSTEXPR auto operator()(basic_string_view<CharT, Traits> input) const
        -> split_result<CharT, Traits>
    {
        return cut_token(input, delimeter);
    }
    CharT delimeter;
};

template<typename CharT, typename Traits>
struct trim_splitter : basic_splitter<CharT>
{
    using view_type = basic_string_view<CharT, Traits>;
    constexpr trim_splitter(CharT delimeter, view_type whitespace)
        : basic_splitter<CharT>(delimeter), whitespace(whitespace)
    {
    }
    SVBB_CXX14_CONSTEXPR auto operator()(view_type input) const -> split_result<CharT, Traits>
    {
        auto splitted = basic_splitter<CharT>::operator()(input);
        splitted.left = trim(splitted.left, whitespace);
        return splitted;
    }
    view_type whitespace;
};
} // namespace detail

template<typename CharT, typename Traits, typename Splitter>
SVBB_CXX14_CONSTEXPR auto tokenize(basic_string_view<CharT, Traits> view, Splitter splitter)
    -> detail::token_range<CharT, Traits, Splitter>
{
    return detail::token_range<CharT, Traits, Splitter>{view, splitter};
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto tokenize(basic_string_view<CharT, Traits> view, CharT delimeter)
    -> detail::token_range<CharT, Traits, detail::basic_splitter<CharT>>
{
    return tokenize(view, detail::basic_splitter<CharT>{delimeter});
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto tokenize(basic_string_view<CharT, Traits> view, CharT delimeter,
                                   basic_string_view<CharT, Traits> whitespace)
    -> detail::token_range<CharT, Traits, detail::trim_splitter<CharT, Traits>>
{
    return tokenize(view, detail::trim_splitter<CharT, Traits>{delimeter, whitespace});
}

} // namespace svbb
