#pragma once
#include "config.hpp"
#include "split.hpp"
#include "trim.hpp"

namespace SVBB_NAMESPACE {

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
        return split_around(input, std::min(input.find_first_of(delimeter), input.size()));
    }
    CharT delimeter;
};

template<typename CharT, typename Traits>
struct trim_splitter
{
    using view_type = basic_string_view<CharT, Traits>;
    constexpr trim_splitter(CharT delimeter, view_type whitespace)
        : whitespace(whitespace), delimeter(delimeter)
    {
    }
    SVBB_CXX14_CONSTEXPR auto operator()(view_type input) const -> split_result<CharT, Traits>
    {
        input = trim_left(input, whitespace);
        auto splitted = split_around(input, std::min(input.find_first_of(delimeter), input.size()));
        splitted.left = trim_right(splitted.left, whitespace);
        return splitted;
    }
    view_type whitespace;
    CharT delimeter;
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
} // namespace SVBB_NAMESPACE
