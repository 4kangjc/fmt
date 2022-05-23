#pragma once

#include "formatter.h"
// #include 

namespace fmt {

template <class _Context>
class basic_format_args;

class lazy_locale {
private:
    const std::locale* loc_ = nullptr;
public:
    lazy_locale() = default;
    explicit lazy_locale(const std::locale& loc) : loc_(&loc) {}
    explicit lazy_locale(const std::locale&&) = delete;

    [[nodiscard]] std::locale get() const {
        if (loc_) {
            return *loc_;
        } else {
            return std::locale{};
        }
    }
};

template <class _Out, class _CharT>
    requires std::output_iterator<_Out, const _CharT&>
class basic_format_context {
private:
    _Out outputIt_;
    basic_format_args<basic_format_context> args_;
    lazy_locale loc_;
public:
    using iterator = _Out;
    using char_type = _CharT;

    template <class T>
    using formatter_type = formatter<T, _CharT>;

    constexpr basic_format_context(_Out OutputIt_, basic_format_args<basic_format_context> ctx_args)
        : outputIt_(std::move(OutputIt_)), args_(ctx_args) { }
    
    [[nodiscard]] basic_format_arg<basic_format_context> arg(size_t id) const noexcept {
        return args_.get(id);
    }

    [[nodiscard]] std::locale locale() {
        return loc_.get();
    }

    [[nodiscard]] iterator out() {
        return std::move(outputIt_);
    }

    void advance_to(iterator it) {
        outputIt_ = std::move(it);
    }

    [[nodiscard]] const basic_format_args<basic_format_context>& get_args() const noexcept {
        return args_;
    }

    [[nodiscard]] lazy_locale get_lazy_locale() const {
        return loc_;
    }
};

using format_context = basic_format_context<fmt_it, char>;
using wformat_context = basic_format_context<fmt_wit, wchar_t>;

} // namespace fmt