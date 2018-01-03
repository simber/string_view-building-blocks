#pragma once
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

#define SVBB_NAMESPACE svbb

namespace SVBB_NAMESPACE {

template<typename CharT, typename Traits>
using basic_string_view = SVBB_STRING_VIEW_IMPL::basic_string_view<CharT, Traits>;
using string_view = SVBB_STRING_VIEW_IMPL::string_view;

} // namespace SVBB_NAMESPACE
