#pragma once

namespace fmt {

#if __cplusplus > 201703l && __has_builtin(__builtin_bit_cast)

template<typename _To, typename _From>
[[nodiscard]] constexpr _To bit_cast(const _From& __from) noexcept
#ifdef __cpp_concepts
    requires (sizeof(_To) == sizeof(_From))
      && __is_trivially_copyable(_To) && __is_trivially_copyable(_From)
#endif
{
    return __builtin_bit_cast(_To, __from);
}

#elif __cplusplus >= 201703L
#include <cstring>

template <typename To, typename From, typename = std::enable_if_t<sizeof(To) == sizeof(From)>>
[[nodiscard]] constexpr To bit_cast(const From& from) noexcept {
    auto to = To();
    memcpy(&to, &from, sizeof(to));
    return to;
}

#else

#endif

} // namespace fmt