#pragma once

#include <string_view>
#include <string>
#include "format_arg.h"
#include "fmt_buffer.h"
#include "format_context.h"

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

template <class _Context>
struct format_arg_traits {
    using _Char_type = typename _Context::char_type;


};


template <class _Context, class... _Args>
class format_arg_store {
private:
    using _CharType = typename _Context::char_type;
    using _Traits = format_arg_store<_Context>;

};

template <class _Context>
class basic_format_args {
public:
    basic_format_args() noexcept = default;
    // basic_format_args(const );

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

template <typename... _Args>
[[nodiscard]] std::string vformat(const std::string_view fmt, const format_args args) {
    std::string str;
    // str.reserve(fmt.size() + args.);

    return str;
}

template <typename... _Args>
[[nodiscard]] inline std::string format(const fmt_string<_Args...> fmt, _Args&&... args) {

    return "";
}

} // namespace fmt