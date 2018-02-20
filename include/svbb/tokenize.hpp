#pragma once
#include "config.hpp"
#include "token_iterator.hpp"
#include "trim.hpp"

namespace SVBB_NAMESPACE {

template<typename CharT, typename Traits, typename Splitter>
class token_range
{
public:
    using iterator = token_iterator_base<CharT, Traits, Splitter>;
    using const_iterator = iterator;
    using view_type = typename iterator::view_type;

    SVBB_CONSTEXPR token_range(view_type view, Splitter splitter)
        : state_(view, splitter), begin_(state_)
    {
    }

    SVBB_CONSTEXPR auto begin() const SVBB_NOEXCEPT -> iterator { return begin_; }
    SVBB_CONSTEXPR auto end() const SVBB_NOEXCEPT -> iterator { return iterator(); }

private:
    using state_type = typename iterator::state;

    state_type state_;
    iterator begin_;
};

template<typename CharT>
class split_by_char
{
public:
    SVBB_CONSTEXPR split_by_char() : delimeter_() {}
    SVBB_CONSTEXPR explicit split_by_char(CharT delimeter) : delimeter_(delimeter) {}

    template<typename Traits>
    SVBB_CXX14_CONSTEXPR auto operator()(basic_string_view<CharT, Traits> input) const
        -> split_result<CharT, Traits>
    {
        return split_around(input, std::min(input.find_first_of(delimeter_), input.size()));
    }

private:
    CharT delimeter_;
};

template<typename CharT, typename Traits>
class split_by_char_and_trim
{
public:
    using view_type = basic_string_view<CharT, Traits>;
    SVBB_CONSTEXPR split_by_char_and_trim() : whitespace_(), delimeter_() {}
    SVBB_CONSTEXPR split_by_char_and_trim(CharT delimeter, view_type whitespace)
        : whitespace_(whitespace), delimeter_(delimeter)
    {
    }
    SVBB_CXX14_CONSTEXPR auto operator()(view_type input) const -> split_result<CharT, Traits>
    {
        input = trim_left(input, whitespace_);
        auto splitted =
            split_around(input, std::min(input.find_first_of(delimeter_), input.size()));
        splitted.left = trim_right(splitted.left, whitespace_);
        return splitted;
    }

private:
    view_type whitespace_;
    CharT delimeter_;
};

template<typename CharT, typename Traits, typename Splitter>
SVBB_CXX14_CONSTEXPR auto tokenize(basic_string_view<CharT, Traits> view, Splitter splitter)
    -> token_range<CharT, Traits, Splitter>
{
    return token_range<CharT, Traits, Splitter>(view, splitter);
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto tokenize(basic_string_view<CharT, Traits> view, CharT delimeter)
    -> token_range<CharT, Traits, split_by_char<CharT>>
{
    return tokenize(view, split_by_char<CharT>(delimeter));
}

template<typename CharT, typename Traits>
SVBB_CXX14_CONSTEXPR auto tokenize(basic_string_view<CharT, Traits> view, CharT delimeter,
                                   basic_string_view<CharT, Traits> whitespace)
    -> token_range<CharT, Traits, split_by_char_and_trim<CharT, Traits>>
{
    return tokenize(view, split_by_char_and_trim<CharT, Traits>(delimeter, whitespace));
}
} // namespace SVBB_NAMESPACE
