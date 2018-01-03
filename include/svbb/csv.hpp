#pragma once

#include "svbb.hpp"
#include <sstream>
#include <algorithm>
#include <type_traits>

#ifndef SVBB_OPTIONAL_IMPL
#include <optional>
#define SVBB_OPTIONAL_IMPL std::optional
#endif // !SVBB_OPTIONAL_IMPL

namespace svbb {
template<typename T>
using optional = SVBB_OPTIONAL_IMPL<T>;

template<typename InputIterator, typename T>
SVBB_CXX14_CONSTEXPR optional<int> count_until(InputIterator first, InputIterator last,
                                               const T& val)
{
    int count = 0;
    const auto it = std::find_if(first, last, [&](const T& other) {
        ++count;
        return other == val;
    });
    return it != last ? optional<int>{count - 1} : optional<int>{};
}

template<typename TokenRange>
SVBB_CXX14_CONSTEXPR optional<int> find_column_index(const TokenRange& line,
                                                     string_view column_name)
{
    using std::begin;
    using std::end;
    return count_until(begin(line), end(line), column_name);
}

template<typename TokenRange, typename... StringViewable>
SVBB_CXX14_CONSTEXPR auto find_column_indices(const TokenRange& line,
                                              StringViewable&&... column_names)
    -> std::array<optional<int>, sizeof...(StringViewable)>
{
    const auto name_array = concat(std::forward<StringViewable>(column_names)...);
    std::array<optional<int>, sizeof...(StringViewable)> out;
    int column_index = 0;
    for(const auto&& token : line) {

        const auto name_index = find_column_index(name_array, token);
        if(name_index) {
            if(out[*name_index]) {
                // TODO error double found
            }

            out[*name_index] = column_index;
        }
        ++column_index;
    }
    return out;
}

template<typename TokenRange, typename... StringViewable>
SVBB_CXX14_CONSTEXPR auto parse_csv_header(const TokenRange& line, StringViewable&&... column_names)
    -> std::vector<optional<int>>
{
    const auto indices = find_column_indices(line, std::forward<StringViewable>(column_names)...);
    const auto max_it = std::max_element(indices.begin(), indices.end());
    std::vector<optional<int>> out(max_it != indices.end() && *max_it ? *(*max_it) + 1 : 0);

    int name_index = 0;
    for(const auto& idx : indices) {
        if(idx) {
            out[*idx] = name_index;
        }
        else {
            // TODO not found error
        }
        ++name_index;
    }

    return out;
}

template<typename TokenRange, size_t N>
SVBB_CXX14_CONSTEXPR void parse_csv_line(const TokenRange& line, string_view_concatination<N>& out,
                                         const std::vector<optional<int>>& header)
{
    int column_index = 0;
    for(const auto&& token : line) {
        if(header[column_index]) {
            if(*header[column_index] >= static_cast<int>(N)) {
                // TODO blow;
            }

            out[*header[column_index]] = token;
        }

        ++column_index;
    }
}

template<size_t N>
void parse_helper(const string_view_concatination<N>&, size_t)
{
}

// specialize for conversion of builtin types or types from unowned namespaces
template<typename T>
struct str_to_value_converter
{
    template<typename StringType>
    static bool convert(const StringType& str, T& val)
    {
        static_assert(std::is_assignable<T, StringType>::value,
                      "You need to overload your conversion");
        val = str;
        return true;
    }
};

// overload for adl types
template<typename StringType, typename T>
bool str_to_value(const StringType& str, T& val)
{
    return str_to_value_converter<T>::convert(str, val);
}

template<size_t N, class T, class... Args>
void parse_helper(const string_view_concatination<N>& tokens, size_t idx, T& t, Args&... args)
{
    str_to_value(tokens[idx], t);
    parse_helper(tokens, idx + 1, args...);
}

template<typename TokenRange, typename... Args>
void parse_csv_line(const TokenRange& line, const std::vector<optional<int>>& header, Args&... args)
{
    string_view_concatination<sizeof...(Args)> columns;
    parse_csv_line(line, columns, header);
    parse_helper(columns, 0, args...);
}

} // namespace svbb
