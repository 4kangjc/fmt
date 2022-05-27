#pragma once
#include <stdint.h>

namespace fmt {

enum class basic_format_arg_type : uint8_t {
    none,
    int_type,
    uint_type,
    long_long_type,
    ulong_long_type,
    bool_type,
    char_type,
    float_type,
    double_type,
    long_double_type,
    pointer_type,
    cstring_type,
    string_type,
    custom_type,
};

} // namespace fmt