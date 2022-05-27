#pragma once

#include "format_arg.h"

namespace fmt {

enum class fmt_align : uint8_t { none, left, right, center };
enum class fmt_sign : uint8_t { none, plus, minus, space };

template <class CharT>
struct basic_format_specs {
    int width           = 0;
    int precision       = -1;
    char type           = '\0';
    fmt_align alignment = fmt_align::none;
    fmt_sign sgn        = fmt::fmt_sign::none;
    bool alt            = false;
    bool localized      = false;
    bool leading_zero   = false;
    uint8_t fill_length = 1;
    CharT fill[4 / sizeof(CharT)] = {CharT{' '}};
};

template <class CharT>
struct dynamic_format_specs : basic_format_specs<CharT> {
    int dynamic_width_index     = -1;
    int dynamic_precision_index = -1;
};

template <class T, class CharT>
struct formatter {
    formatter() = delete;
    formatter(const formatter&) = delete;
    formatter operator=(const formatter&) = delete;
};

template <class T, class CharT, basic_format_arg_type ArgType>
struct formatter_base {

private:
    dynamic_format_specs<CharT> specs_;
};

#define _FORMAT_SPECIALIZE_FOR(Type, ArgType) \
    template <format_supported_charT CharT>   \
    struct formatter<Type, CharT> : formatter_base<Type, CharT, ArgType> { }

_FORMAT_SPECIALIZE_FOR(int, basic_format_arg_type::int_type);
_FORMAT_SPECIALIZE_FOR(unsigned int, basic_format_arg_type::uint_type);
_FORMAT_SPECIALIZE_FOR(long long, basic_format_arg_type::long_long_type);
_FORMAT_SPECIALIZE_FOR(unsigned long long, basic_format_arg_type::ulong_long_type);
_FORMAT_SPECIALIZE_FOR(bool, basic_format_arg_type::bool_type);
_FORMAT_SPECIALIZE_FOR(float, basic_format_arg_type::float_type);
_FORMAT_SPECIALIZE_FOR(double, basic_format_arg_type::double_type);
_FORMAT_SPECIALIZE_FOR(long double, basic_format_arg_type::long_double_type);
_FORMAT_SPECIALIZE_FOR(std::nullptr_t, basic_format_arg_type::pointer_type);
_FORMAT_SPECIALIZE_FOR(void*, basic_format_arg_type::pointer_type);
_FORMAT_SPECIALIZE_FOR(const void*, basic_format_arg_type::pointer_type);
_FORMAT_SPECIALIZE_FOR(short, basic_format_arg_type::int_type);
_FORMAT_SPECIALIZE_FOR(unsigned short, basic_format_arg_type::uint_type);
_FORMAT_SPECIALIZE_FOR(long, basic_format_arg_type::int_type);
_FORMAT_SPECIALIZE_FOR(unsigned long, basic_format_arg_type::uint_type);
_FORMAT_SPECIALIZE_FOR(char, basic_format_arg_type::char_type);
_FORMAT_SPECIALIZE_FOR(signed char, basic_format_arg_type::int_type);
_FORMAT_SPECIALIZE_FOR(unsigned char, basic_format_arg_type::uint_type);

#undef _FORMAT_SPECIALIZE_FOR

} // namespace fmt