#pragma once
#include "meta.h"
#include <string_view>

namespace fmt {

template <class Context>
class basic_format_arg;

template <class Context>
struct format_arg_traits {
    using char_type = typename Context::char_type;

    template <class Ty>
    static auto Phony_basic_format_arg_constructor(Ty&& ) {
        using Td = remove_cvref_t<Ty>;

        if constexpr (std::is_same_v<Td, bool>) {
            return bool{};
        } else if constexpr (std::is_same_v<Td, char_type>) {
            return char_type{};
        } else if constexpr (std::is_same_v<Td, char> && std::is_same_v<char_type, wchar_t>) {
            return char_type{};
        } else if constexpr (std::signed_integral<Td> && sizeof(Td) <= sizeof(int)) {
            return int{};
        } else if constexpr (std::unsigned_integral<Td> && sizeof(Td) <= sizeof(unsigned int)) {
            return 42u;
        } else if constexpr (std::signed_integral<Td> && sizeof(Td) <= sizeof(long long)) {
            return 42ll;
        } else if constexpr (std::unsigned_integral<Td> && sizeof(Td) <= sizeof(unsigned long long)) {
            return 42ull;
        } else {
            return typename basic_format_arg<Context>::handle{42};
        }
    }


    static auto Phony_basic_format_arg_constructor(float) -> float;
    static auto Phony_basic_format_arg_constructor(double) -> double;
    static auto Phony_basic_format_arg_constructor(long double) -> long double;
    static auto Phony_basic_format_arg_constructor(const char_type*) -> const char_type*;

    template <class _Traits>
    static auto Phony_basic_format_arg_constructor(std::basic_string_view<char_type, _Traits>) 
        -> std::basic_string_view<char_type>;
    
    template <class _Traits>
    static auto Phony_basic_format_arg_constructor(const std::basic_string<char_type, _Traits>&)
        -> std::basic_string_view<char_type>;

    static auto Phony_basic_format_arg_constructor(std::nullptr_t) -> const void*;

    template <class T>
        requires std::is_void_v<T>
    static auto Phony_basic_format_arg_constructor(T*) -> const void*;

    template <class T>
    using storage_type = decltype(Phony_basic_format_arg_constructor(std::declval<T&>()));

    template <class T>
    static constexpr size_t storage_size = sizeof(storage_type<T>);
};

} // namespace fmt