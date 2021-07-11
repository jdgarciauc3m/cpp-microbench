#ifndef CPP_MICROBENCH_ALLOC_ARRAY_HPP
#define CPP_MICROBENCH_ALLOC_ARRAY_HPP

#include <boost/align/aligned_allocator.hpp>

template <typename T>
class alloc_array {
  static_assert(std::is_trivial_v<T>);

public:
  alloc_array() = default;

  explicit alloc_array(std::size_t sz);

  alloc_array(const alloc_array &) = delete;
  alloc_array & operator=(const alloc_array &) = delete;

  alloc_array(alloc_array && da) noexcept : size_ {da.size_},
                                                vec_ {da.vec_} {
    da.size_ = 0;
    da.vec_ = nullptr;
  }

  alloc_array & operator=(alloc_array && da) noexcept {
    alloc_.deallocate(vec_, size_);
    size_ = da.size_;
    vec_ = da.vec_;
    da.size_ = 0;
    da.vec_ = nullptr;
    return *this;
  }

  ~alloc_array() { alloc_.deallocate(vec_, size_); }

  T & operator[](std::size_t i) { return vec_[i]; }
  const T & operator[](std::size_t i) const { return vec_[i]; }
  [[nodiscard]] std::size_t size() const { return size_; }
  [[nodiscard]] auto begin() const { return vec_; }
  [[nodiscard]] auto end() const { return vec_ + size_; }

private:
  static constexpr std::size_t alignment = ::alignment;
  using allocator_type = boost::alignment::aligned_allocator<T, alignment>;
  static allocator_type alloc_;// NOLINT
  std::size_t size_ = 0;
  T * vec_ = nullptr;/// NOLINT
};

template <typename T>
typename alloc_array<T>::allocator_type alloc_array<T>::alloc_;// NOLINT

template <typename T>
// NOLINTNEXTLINE
alloc_array<T>::alloc_array(std::size_t sz) : size_ {sz}, vec_ {alloc_.allocate(size_)} {}


#endif//CPP_MICROBENCH_ALLOC_ARRAY_HPP
