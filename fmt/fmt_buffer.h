#pragma once

#include <iterator>

template <class _Tp, template <class...> class _Template>
inline constexpr bool is_specialization_v = false;

template <template <class...> class _Template, class... _Args>
inline constexpr bool is_specialization_v<_Template<_Args...>, _Template> = true;

namespace fmt {

template <class _Ty>
class fmt_buffer {
private:
    _Ty* ptr_          = nullptr;
    size_t size_       = 0;
    size_t capacity_   = 0;
protected:
    explicit fmt_buffer(const size_t _Size) noexcept : size_(_Size), capacity_(_Size) { }

    ~fmt_buffer() = default;

    fmt_buffer(_Ty* _Data, const size_t _Size, const size_t _Capacity) noexcept
        : ptr_(_Data), size_(_Size), capacity_(_Capacity) {}
    
    void _Set(_Ty* _Buf_data, const size_t _Buf_capacity) noexcept {
        ptr_      = _Buf_data;
        capacity_ = _Buf_capacity;
    }

    virtual void _Grow(size_t capacity) = 0;
public:
    using value_type = _Ty;

    fmt_buffer(const fmt_buffer&) = delete;
    fmt_buffer& operator=(const fmt_buffer&) = delete;

    [[nodiscard]] _Ty* begin() noexcept {
        return ptr_;
    }

    [[nodiscard]] _Ty* end() noexcept {
        return ptr_ + size_;
    }

    [[nodiscard]] size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] size_t capacity() const noexcept {
        return capacity_;
    }

    void clear() noexcept {
        size_ = 0;
    }

    void try_resize(const size_t count) {
        try_reserve(count);
        size_ = count <= capacity_ ? count : capacity_;
    }

    void try_reserve(const size_t new_capacity) {
        if (new_capacity > capacity_) {
            _Grow(new_capacity);
        }
    }

    void push_back(const _Ty _Value) {
        try_reserve(size_ + 1);
        ptr_[size_++] = _Value;
    }
};

using fmt_it = std::back_insert_iterator<fmt_buffer<char>>;
using fmt_wit = std::back_insert_iterator<fmt_buffer<wchar_t>>;

}