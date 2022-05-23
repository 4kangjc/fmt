#pragma once
#include <string_view>
#include "error.h"

namespace fmt {

inline void you_see_this_error_because_arg_id_is_out_of_range() noexcept {}

template <class _CharT>
class basic_format_parse_context {
public:
    using char_type = _CharT;
    using const_iterator = typename std::basic_string_view<_CharT>::const_iterator;
    using iterator = const_iterator;

    constexpr explicit basic_format_parse_context(
        const std::basic_string_view<_CharT> fmt, const size_t num_args = 0) noexcept
        : format_string_(fmt), num_args_(num_args) { }

    basic_format_parse_context(const basic_format_parse_context&) = delete;
    basic_format_parse_context& operator=(const basic_format_parse_context&) = delete;

    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return format_string_.begin();
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return format_string_.end();
    }

    constexpr void addvance_to(const const_iterator it) {
        format_string_.remove_prefix(it - begin());
    }

    [[nodiscard]] constexpr size_t next_arg_id() {
        if (next_arg_id_ < 0) {
            throw_format_error("Can not switch from manual to automatic indexing");
        }

        if (std::is_constant_evaluated()) {
            if (static_cast<size_t>(next_arg_id_) >= num_args_) {
                you_see_this_error_because_arg_id_is_out_of_range();
            }
        }

        return static_cast<size_t>(next_arg_id_++);
    }

    constexpr void check_arg_id(const size_t id) {
        if (std::is_constant_evaluated()) {
            if (id >= num_args_) {
                you_see_this_error_because_arg_id_is_out_of_range();
            }
        }

        if (next_arg_id_ > 0) {
            throw_format_error("Can not switch from automatic to manual indexing");
        }
        next_arg_id_ = -1;
    }
private:
    std::basic_string_view<_CharT> format_string_;
    size_t num_args_;

    ptrdiff_t next_arg_id_ = 0;
};

using format_parse_context = basic_format_parse_context<char>;
using wformat_parse_context = basic_format_parse_context<wchar_t>;

}