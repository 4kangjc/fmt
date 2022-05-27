#pragma once

#include "format_arg_traits.h"
#include "format_arg_type.h"
#include <cstring>

namespace fmt {

template <class Context>
class basic_format_args;

struct format_arg_index {
    constexpr format_arg_index() = default;
    constexpr explicit format_arg_index(const size_t index) noexcept : index_(index) {
        type(basic_format_arg_type::none);
    }

    [[nodiscard]] constexpr basic_format_arg_type type() const noexcept {
        return static_cast<basic_format_arg_type>(type_);
    }

    constexpr void type(basic_format_arg_type val) noexcept {
        type_ = static_cast<size_t>(val);
    }

    size_t index_ : (sizeof(size_t) * 8 - 4);
    size_t type_  : 4; 
};

template <class _Context, class... _Args>
class format_arg_store {
private:
    using char_type = typename _Context::char_type;
    using traits = format_arg_traits<_Context>;

    friend basic_format_args<_Context>;

    static constexpr size_t nums_args = sizeof...(_Args);
    static constexpr size_t storage_length = (traits::template storage_size<_Args> + ...);

    format_arg_index index_array_[nums_args];
    unsigned char storage_[storage_length];

    template <class T>
    void stroe_impl(const size_t arg_index, const basic_format_arg_type arg_type, const std::type_identity_t<T>& val) {
        //TODO check arg_index < num_args

        const auto store_index = index_array_[arg_index].index_;

        memcpy(storage_ + store_index, std::addressof(val), sizeof(T));
        index_array_[arg_index].type(arg_type);
        if (arg_index + 1 < nums_args) {
// #pragma warning(suppress : 6385)
            index_array_[arg_index + 1] = format_arg_index{store_index + sizeof(T)};
        }
    }

    template <class T>
    void store(const size_t arg_index, T&& val) noexcept {
        using erased_type = typename traits::template storage_type<T>;

        basic_format_arg_type arg_type = basic_format_arg_type::none;
        if constexpr (std::is_same_v<erased_type, bool>) {
            arg_type = basic_format_arg_type::bool_type;
        } else if constexpr (std::is_same_v<erased_type, char_type>) {
            arg_type = basic_format_arg_type::char_type;
        } else if constexpr (std::is_same_v<erased_type, int>) {
            arg_type = basic_format_arg_type::int_type;
        } else if constexpr (std::is_same_v<erased_type, unsigned int>) {
            arg_type = basic_format_arg_type::uint_type;
        } else if constexpr (std::is_same_v<erased_type, long long>) {
            arg_type = basic_format_arg_type::long_long_type;
        } else if constexpr (std::is_same_v<erased_type, unsigned long long>) {
            arg_type = basic_format_arg_type::ulong_long_type;
        } else if constexpr (std::is_same_v<erased_type, float>) {
            arg_type = basic_format_arg_type::float_type;
        } else if constexpr (std::is_same_v<erased_type, double>) {
            arg_type = basic_format_arg_type::double_type;
        } else if constexpr (std::is_same_v<erased_type, long double>) {
            arg_type = basic_format_arg_type::long_double_type;
        } else if constexpr (std::is_same_v<erased_type, const void*>) {
            arg_type = basic_format_arg_type::pointer_type;
        } else if constexpr (std::is_same_v<erased_type, const char_type*>) {
            arg_type = basic_format_arg_type::cstring_type;
        } else if constexpr (std::is_same_v<erased_type, std::basic_string_view<char_type>>) {
            arg_type = basic_format_arg_type::string_type;
        } else {
            static_assert(std::is_same_v<erased_type, typename basic_format_arg<_Context>::handle>);
            arg_type = basic_format_arg_type::custom_type;
        }

        stroe_impl<erased_type>(arg_index, arg_type, static_cast<erased_type>(val));
    }
public:
    format_arg_store(_Args&... vals) noexcept {
        index_array_[0] = {};
        size_t arg_index = 0;
        (store(arg_index++, vals), ...);
    }
};

template <class _Context>
class format_arg_store<_Context> {};

}