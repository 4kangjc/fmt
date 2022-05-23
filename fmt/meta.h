#pragma once

#include <type_traits>

namespace fmt {

#if __cplusplus >= 202002L
template <class _Tp>
using remove_cvref_t = std::remove_cvref_t<_Tp>;

template <class _Tp, class _Context>
concept has_formatter = requires(_Tp& val, _Context ctx) {
    std::declval<typename _Context::template formatter_type<remove_cvref_t<_Tp>>>().format(val, ctx);
};

template <class _Tp, class _Context>
concept has_const_formatter = has_formatter<const remove_cvref_t<_Tp>, _Context>;

#elif __cplusplus >= 201703L
template <typename T>
using remove_cvref_t = typename std::remove_cv_t<std::remove_reference_t<T>>;


#else
template <typename T>
using remove_cvref_t = typename std::remove_cv<std::remove_reference<_Tp>::value>::value;
#endif

}