#pragma once

#include <string_view>
#include <string>
#include "format_arg.h"
#include "fmt_buffer.h"
#include "format_context.h"
#include "format_arg_traits.h"
#include "format_arg_store.h"

namespace fmt {

// using format_context = basci_format_context<

template <class _CharT, class... _Args>
struct basic_format_string {
    std::basic_string_view<_CharT> str_;

    template <class _Ty>
        requires std::convertible_to<const _Ty&, std::basic_string_view<_CharT>>
    // consteval
    constexpr basic_format_string(const _Ty& _Str_val) : str_(_Str_val) {

    }
};

template <class... _Args>
using fmt_string = basic_format_string<char, std::type_identity_t<_Args>...>;

template <class... _Args>
using fmt_wstring = basic_format_string<wchar_t, std::type_identity_t<_Args>...>;


using format_args = basic_format_args<format_context>;
using wformat_args = basic_format_args<wformat_context>;

template <class _Context = format_context, class... _Args>
[[nodiscard]] auto make_format_args(_Args&&... args) {
    return format_arg_store<_Context, _Args...>{args...};
}

template <class... _Args>
[[nodiscard]] auto make_wformat_args(_Args&&... args) {
    return format_arg_store<wformat_context, _Args...>{args...};
}

template <std::output_iterator<const char&> OutputIt>
OutputIt vformat_to(OutputIt out, const std::string_view fmt, const format_args args) {
    if constexpr (std::is_same_v<OutputIt, fmt_it>) {

    } else {
        
    }
    return out;
}

template <typename... _Args>
[[nodiscard]] std::string vformat(const std::string_view fmt, const format_args args) {
    std::string str;
    str.reserve(fmt.size() + args.estimate_required_capacity());
    vformat_to(std::back_insert_iterator{str}, fmt, args);
    return str;
}

template <typename... _Args>
[[nodiscard]] inline std::string format(const fmt_string<_Args...> fmt, _Args&&... args) {
    return vformat(fmt.str_, make_format_args(args...));
}

} // namespace fmt