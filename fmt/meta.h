#pragma once

#include <type_traits>

namespace fmt {

template <class _Tp, class... T>
inline constexpr bool is_any_of_v = std::disjunction_v<std::is_same_v<_Tp, T>...>;

#if __cplusplus >= 202002L
template <class _Tp>
using remove_cvref_t = std::remove_cvref_t<_Tp>;

template <class _Tp, class _Context>
concept has_formatter = requires(_Tp& val, _Context ctx) {
    std::declval<typename _Context::template formatter_type<remove_cvref_t<_Tp>>>().format(val, ctx);
};

template <class _Tp, class _Context>
concept has_const_formatter = has_formatter<const remove_cvref_t<_Tp>, _Context>;

template <class CharT>
concept format_supported_charT = is_any_of_v<CharT, char, wchar_t>;

#elif __cplusplus >= 201703L
template <typename T>
using remove_cvref_t = typename std::remove_cv_t<std::remove_reference_t<T>>;

template <typename _Tp, class _Context>
using has_formatter = std::is_constructible<typename _Context::template formatter<_Tp>>;

template <class Context, class T>
constexpr auto has_const_formatter_impl(T*) 
    -> decltype(typename Context::template formatter_type<T>().format(
                    std::declval<const T&>(), std::declval<Context&>()), true) {
    return true;
}

#else
template <typename T>
using remove_cvref_t = typename std::remove_cv<std::remove_reference<_Tp>::value>::value;
#endif

}