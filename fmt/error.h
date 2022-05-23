#pragma once

#include <stdexcept>

namespace fmt {

class format_error : public std::runtime_error {
    using runtime_error::runtime_error;
};

[[noreturn]] inline void throw_format_error(const char* const message) {
    throw format_error{message};
}

} // namespace fmt