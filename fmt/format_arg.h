#pragma once

#include <string_view>
#include "parse_context.h"
#include "meta.h"

namespace fmt {

struct monostate {};

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
    ctring_type,
    string_type,
    custom_type,
};

static_assert(static_cast<int>(basic_format_arg_type::custom_type) < 16, "must fit in 4-bit bitfield");

[[nodiscard]] constexpr bool is_integral_fmt_type(basic_format_arg_type _Ty) {
    return _Ty > basic_format_arg_type::none && _Ty <= basic_format_arg_type::char_type;
}

[[nodiscard]] constexpr bool is_arithmetic_fmt_type(basic_format_arg_type _Ty) {
    return _Ty > basic_format_arg_type::none && _Ty <= basic_format_arg_type::long_double_type;
}

template <class Context>
class basic_format_arg {
public:
    using CharType = typename Context::char_type;

    class handle {
    private:
        const void* ptr_;
        void __attribute__((cdecl)) (*format_)
        (basic_format_parse_context<CharType>& parse_ctx, Context& format_ctx, const void*);
        // friend basic_format_arg;
    public:
        template <class _Tp>
        explicit handle(_Tp&& val) noexcept 
            : ptr_(std::addressof(val)),
              format_([](basic_format_parse_context<CharType>& parse_ctx, Context& format_ctx, const void* ptr) {
                //   using value_type = remove_cvref_t<_Tp>;
                    // typename Context::template formatter_type<value_type> formatter;     // 不是很懂这种写法
                    // typename Context::formatter_type<value_type> formatter;
              }) {

        }
    };

    basic_format_arg_type active_state = basic_format_arg_type::none;
    union {
        monostate no_state = monostate{};
        int int_state;
        unsigned int uint_state;
        long long long_long_state;
        unsigned long long uLong_long_state;
        bool bool_state;
        CharType char_state;
        float float_state;
        double double_state;
        long double long_double_state;
        const void* pointer_state;
        const CharType* cstring_state;
        std::basic_string_view<CharType> string_state;
        handle custom_state;
    };
    
};

} // namespace fmt