#pragma once

#include "format_arg_store.h"
#include "format_arg_type.h"
#include "parse_context.h"
#include "meta.h"
#include "bit.h"

namespace fmt {

struct monostate {};

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
        void /*__attribute__((cdecl))*/ (*format_)
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

        void format(basic_format_parse_context<CharType>& parse_ctx, Context& format_ctx) const {
            format_(parse_ctx, format_ctx);
        }
    };

    basic_format_arg() noexcept : active_state(basic_format_arg_type::none), no_state() { }

    explicit basic_format_arg(const int val) noexcept
        : active_state(basic_format_arg_type::int_type), int_state(val) { }
    explicit basic_format_arg(const unsigned int val) noexcept 
        : active_state(basic_format_arg_type::uint_type), uint_state(val) { }
    explicit basic_format_arg(const long long val) noexcept 
        : active_state(basic_format_arg_type::long_long_type), long_long_state(val) { }
    explicit basic_format_arg(const unsigned long long val) noexcept 
        : active_state(basic_format_arg_type::ulong_long_type), ulong_long_state(val) { }
    explicit basic_format_arg(const bool val) noexcept 
        : active_state(basic_format_arg_type::bool_type), bool_state(val) { }
    explicit basic_format_arg(const CharType val) noexcept 
        : active_state(basic_format_arg_type::char_type), char_state(val) { }
    explicit basic_format_arg(const float val) noexcept 
        : active_state(basic_format_arg_type::float_type), float_state(val) { }
    explicit basic_format_arg(const double val) noexcept 
        : active_state(basic_format_arg_type::double_type), double_state(val) { }
    explicit basic_format_arg(const long double val) noexcept 
        : active_state(basic_format_arg_type::long_double_type), long_double_state(val) { }
    explicit basic_format_arg(const void* val) noexcept 
        : active_state(basic_format_arg_type::pointer_type), pointer_state(val) { }
    explicit basic_format_arg(const CharType* val) noexcept
        : active_state(basic_format_arg_type::cstring_type), cstring_state(val) { }
    explicit basic_format_arg(const std::basic_string_view<CharType> val) noexcept 
        : active_state(basic_format_arg_type::string_type), string_state(val) { }
    explicit basic_format_arg(const handle val) noexcept 
        : active_state(basic_format_arg_type::custom_type), custom_state(val) { }

    explicit operator bool() const noexcept {
        return active_state != basic_format_arg_type::none;
    }


    basic_format_arg_type active_state = basic_format_arg_type::none;
    union {
        monostate no_state = monostate{};
        int int_state;
        unsigned int uint_state;
        long long long_long_state;
        unsigned long long ulong_long_state;
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

template <class _Visitor, class _Context>
decltype(auto) visit_format_arg(_Visitor&& vis, basic_format_arg<_Context> arg) {
    switch (arg.active_state) {
        case basic_format_arg_type::none:
            return std::forward<_Visitor>(vis)(arg.no_state);
        case basic_format_arg_type::int_type: 
            return std::forward<_Visitor>(vis)(arg.int_state);
        case basic_format_arg_type::uint_type: 
            return std::forward<_Visitor>(vis)(arg.uint_state);
        case basic_format_arg_type::long_long_type:
            return std::forward<_Visitor>(vis)(arg.long_long_state);
        case basic_format_arg_type::ulong_long_type:
            return std::forward<_Visitor>(vis)(arg.ulong_long_state);
        case basic_format_arg_type::bool_type:
            return std::forward<_Visitor>(vis)(arg.bool_state);
        case basic_format_arg_type::char_type:
            return std::forward<_Visitor>(vis)(arg.char_state);
        case basic_format_arg_type::float_type:
            return std::forward<_Visitor>(vis)(arg.float_state);
        case basic_format_arg_type::double_type:
            return std::forward<_Visitor>(vis)(arg.double_state);
        case basic_format_arg_type::long_double_type:
            return std::forward<_Visitor>(vis)(arg.long_double_state);
        case basic_format_arg_type::pointer_type:
            return std::forward<_Visitor>(vis)(arg.pointer_state);
        case basic_format_arg_type::cstring_type:
            return std::forward<_Visitor>(vis)(arg.cstring_state);
        case basic_format_arg_type::string_type:
            return std::forward<_Visitor>(vis)(arg.string_state);
        case basic_format_arg_type::custom_type:
            return std::forward<_Visitor>(vis)(arg.custom_state);
        default:
            // TODO log or assert or throw exception
            return std::forward<_Visitor>(vis)(0);
    }
}

template <class _Context>
class basic_format_args {
public:
    basic_format_args() noexcept = default;
    basic_format_args(const format_arg_store<_Context>& ) noexcept {}

    template <class... _Args>
    basic_format_args(const format_arg_store<_Context, _Args...>& store) noexcept 
        : num_args_(sizeof...(_Args)), index_array_(store.index_array_) { }
    
    [[nodiscard]] basic_format_arg<_Context> get(const size_t index) const noexcept {
        if (index >= num_args_) {
            return basic_format_arg<_Context>{};
        }

        using char_type = typename _Context::char_type;

        const auto packed_index = index_array_[index];
        const auto arg_storage = 
            reinterpret_cast<const unsigned char*>(index_array_ + num_args_) + packed_index.index_;
            // index_array_ + num_args_ == storage_

        switch (packed_index.type()) {
            case basic_format_arg_type::none: 
            default:
                _PSTL_ASSERT_MSG(false, "invalid basic_format_arg_type");
                return basic_format_arg<_Context>{};
            case basic_format_arg_type::int_type:
                return basic_format_arg<_Context>{get_value_from_memory<int>(arg_storage)};
            case basic_format_arg_type::uint_type:
                return basic_format_arg<_Context>{get_value_from_memory<unsigned int>(arg_storage)};
            case basic_format_arg_type::long_long_type:
                return basic_format_arg<_Context>{get_value_from_memory<long long>(arg_storage)};
            case basic_format_arg_type::ulong_long_type:
                return basic_format_arg<_Context>{get_value_from_memory<unsigned long long>(arg_storage)};
            case basic_format_arg_type::bool_type:
                return basic_format_arg<_Context>{get_value_from_memory<bool>(arg_storage)};
            case basic_format_arg_type::char_type:
                return basic_format_arg<_Context>{get_value_from_memory<char_type>(arg_storage)};
            case basic_format_arg_type::float_type:
                return basic_format_arg<_Context>{get_value_from_memory<float>(arg_storage)};
            case basic_format_arg_type::double_type:
                return basic_format_arg<_Context>{get_value_from_memory<double>(arg_storage)};
            case basic_format_arg_type::long_double_type:
                return basic_format_arg<_Context>{get_value_from_memory<long double>(arg_storage)};
            case basic_format_arg_type::pointer_type:
                return basic_format_arg<_Context>{get_value_from_memory<const void*>(arg_storage)};
            case basic_format_arg_type::cstring_type:
                return basic_format_arg<_Context>{get_value_from_memory<const char_type*>(arg_storage)};
            case basic_format_arg_type::string_type:
                return basic_format_arg<_Context>{get_value_from_memory<std::basic_string_view<char_type>>(arg_storage)};
            case basic_format_arg_type::custom_type:
                return basic_format_arg<_Context>{get_value_from_memory<typename basic_format_arg<_Context>::handle>(arg_storage)};
        }
    }
    
    [[nodiscard]] size_t estimate_required_capacity() const noexcept {
        using char_type = typename _Context::char_type;
        size_t result = 0;
        const auto visitor = [&result]<class ArgTy>(const ArgTy arg) noexcept {
            if constexpr (std::is_same_v<ArgTy, std::basic_string_view<char_type>>) {
                result += arg.size();
            } else if constexpr (std::is_same_v<ArgTy, const char_type*>) {
                result += 32;   // estimate for length of null-terminated strings
            } else {
                result += 8;    // estimate for length of all other argument
            }
        };

        for (size_t idx = 0; idx < num_args_; ++idx) {
            visit_format_arg(visitor, get(idx));
        }

        return result;
    }
private:
    template <class T>
    [[nodiscard]] static auto get_value_from_memory(const unsigned char* const val) noexcept {
        auto& temp = *reinterpret_cast<const unsigned char(*)[sizeof(T)]>(val);     // -Wno-array-bounds 实例化代码引起编译警告
        return bit_cast<T>(temp);
    }

    size_t num_args_                     = 0;
    const format_arg_index* index_array_ = nullptr;
};

} // namespace fmt