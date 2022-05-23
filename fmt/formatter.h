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

} // namespace fmt