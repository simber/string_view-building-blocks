#pragma once
#include "config.hpp"
#include "split.hpp"

namespace SVBB_NAMESPACE {

template<typename CharT, typename Traits, typename Splitter>
class token_iterator_base
{
public:
    class state
    {
    public:
        using view_type = basic_string_view<CharT, Traits>;

        SVBB_CONSTEXPR state() SVBB_NOEXCEPT {}
        SVBB_CONSTEXPR state(view_type input, Splitter splitter)
            : data_(make_split(view_type(), input)), splitter_(splitter)
        {
        }

        SVBB_CONSTEXPR view_type token() const SVBB_NOEXCEPT { return data_.left; }
        SVBB_CONSTEXPR view_type remainder() const SVBB_NOEXCEPT { return data_.right; }
        SVBB_CONSTEXPR bool empty() const SVBB_NOEXCEPT
        {
            return remainder().empty() && token().empty();
        }
        SVBB_CXX14_CONSTEXPR void split() { data_ = splitter_(remainder()); }

    private:
        split_result<CharT, Traits> data_;
        Splitter splitter_;
    };

    using view_type = basic_string_view<CharT, Traits>;
    using value_type = view_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const view_type*;
    using reference = view_type;
    using iterator_category = std::input_iterator_tag;

    SVBB_CONSTEXPR token_iterator_base() SVBB_NOEXCEPT : state_{nullptr} {};
    SVBB_CXX14_CONSTEXPR token_iterator_base(state& data) : state_(std::addressof(data))
    {
        advance();
    }

    SVBB_CONSTEXPR reference operator*() const SVBB_NOEXCEPT { return state_->token(); }
    SVBB_CXX14_CONSTEXPR token_iterator_base& operator++()
    {
        advance();
        return *this;
    }

    SVBB_CXX14_CONSTEXPR token_iterator_base operator++(int)
    {
        token_iterator_base tmp = *this;
        advance();
        return tmp;
    }

    SVBB_CONSTEXPR bool operator==(const token_iterator_base& rhs) const SVBB_NOEXCEPT
    {
        return state_ == nullptr && rhs.state_ == nullptr;
    }
    SVBB_CONSTEXPR bool operator!=(const token_iterator_base& rhs) const SVBB_NOEXCEPT
    {
        return !(*this == rhs);
    }

    SVBB_CONSTEXPR bool valid() const SVBB_NOEXCEPT { return state_ != nullptr; }

private:
    SVBB_CXX14_CONSTEXPR void advance()
    {
        state_->split();
        if(state_->empty()) state_ = nullptr;
    }

    state* state_;
};

template<typename CharT, typename Traits, typename Splitter>
class token_iterator
{
public:
    using view_type = basic_string_view<CharT, Traits>;
    using value_type = view_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const view_type*;
    using reference = view_type;
    using iterator_category = std::forward_iterator_tag;

    SVBB_CONSTEXPR token_iterator() SVBB_NOEXCEPT = default;
    SVBB_CXX14_CONSTEXPR explicit token_iterator(view_type input, Splitter splitter = Splitter())
        : state_(input, splitter), base_(state_)
    {
    }

    SVBB_CONSTEXPR reference operator*() const SVBB_NOEXCEPT { return *base_; }
    SVBB_CXX14_CONSTEXPR token_iterator& operator++()
    {
        ++base_;
        return *this;
    }

    SVBB_CXX14_CONSTEXPR token_iterator operator++(int)
    {
        token_iterator_base tmp = *this;
        ++base_;
        return tmp;
    }

    SVBB_CONSTEXPR bool operator==(const token_iterator& rhs) const SVBB_NOEXCEPT
    {
        return (base_.valid() && rhs.base_.valid()) ?
                   (state_.remainder().data() == rhs.state_.remainder().data() &&
                    state_.remainder().size() == rhs.state_.remainder().size()) :
                   (base_.valid() == rhs.base_.valid());
    }
    SVBB_CONSTEXPR bool operator!=(const token_iterator& rhs) const SVBB_NOEXCEPT
    {
        return !(*this == rhs);
    }

private:
    using state_type = typename token_iterator_base<CharT, Traits, Splitter>::state;

    state_type state_;
    token_iterator_base<CharT, Traits, Splitter> base_;
};

template<typename CharT, typename Traits, typename Splitter>
auto begin_tokens(basic_string_view<CharT, Traits> input, Splitter splitter)
    -> token_iterator<CharT, Traits, Splitter>
{
    return token_iterator<CharT, Traits, Splitter>(input, splitter);
}

template<typename CharT, typename Traits, typename Splitter>
auto end_tokens(basic_string_view<CharT, Traits>, Splitter)
    -> token_iterator<CharT, Traits, Splitter>
{
    return token_iterator<CharT, Traits, Splitter>();
}

} // namespace SVBB_NAMESPACE
