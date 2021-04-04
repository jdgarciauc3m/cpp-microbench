#ifndef CPP_MICROBENCH_ALIGNED_CONT_H
#define CPP_MICROBENCH_ALIGNED_CONT_H

#include <vector>
#include <chrono>

#include <gsl/util>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <boost/align/aligned_allocator.hpp>

constexpr std::size_t alignment = 4096;

template <typename T>
class trivial_dynamic_array {
  static_assert(std::is_trivial_v<T>);

public:
  trivial_dynamic_array() = default;

  explicit trivial_dynamic_array(std::size_t sz);

  trivial_dynamic_array(const trivial_dynamic_array &) = delete;
  trivial_dynamic_array & operator=(const trivial_dynamic_array &) = delete;

  trivial_dynamic_array(trivial_dynamic_array && da) noexcept
      : size_ {da.size_},
        buf_ {da.buf_},
        vec_ {da.vec_}
  {
    da.size_ = 0;
    da.buf_ = nullptr;
    da.vec_ = nullptr;
  }

  trivial_dynamic_array & operator=(trivial_dynamic_array && da) noexcept {
    delete []buf_;
    size_ = da.size_;
    buf_ = da.buf_;
    vec_ = da.vec_;
    da.size_ = 0;
    da.buf_ = nullptr;
    da.vec_ = nullptr;
    return *this;
  }

  ~trivial_dynamic_array() noexcept { delete []buf_; }

  T & operator[](std::size_t i) noexcept { return vec_[i]; }
  const T & operator[](std::size_t i) const noexcept { return vec_[i]; }
  [[nodiscard]] std::size_t size() const { return size_; }
  [[nodiscard]] auto begin() const { return vec_; }
  [[nodiscard]] auto end() const { return vec_ + size_; }

private:
  static constexpr std::size_t pad = 256 / sizeof(T);
  static constexpr std::size_t linesize = 64;
  std::size_t size_ = 0;
  T * buf_ = nullptr; // NOLINT
  T * vec_ = nullptr;
};
template <typename T>
trivial_dynamic_array<T>::trivial_dynamic_array(std::size_t sz)
    : size_{sz},
      buf_{new T[sz + pad]},
      vec_{reinterpret_cast<T*>((reinterpret_cast<intptr_t>(buf_) + pad) & ~(linesize-1))}
{
}

template <typename T>
class dynamic_array {
    static_assert(std::is_trivial_v<T>);

    public:
    dynamic_array() = default;

    explicit dynamic_array(std::size_t sz);

    dynamic_array(const dynamic_array &) = delete;
    dynamic_array & operator=(const dynamic_array &) = delete;

    dynamic_array(dynamic_array && da) noexcept : size_ {da.size_},
    vec_ {da.vec_} {
      da.size_ = 0;
      da.vec_ = nullptr;
    }

    dynamic_array & operator=(dynamic_array && da) noexcept {
      alloc_.deallocate(vec_, size_);
      size_ = da.size_;
      vec_ = da.vec_;
      da.size_ = 0;
      da.vec_ = nullptr;
      return *this;
    }

    ~dynamic_array() { alloc_.deallocate(vec_, size_); }

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
typename dynamic_array<T>::allocator_type dynamic_array<T>::alloc_;// NOLINT

template <typename T>
// NOLINTNEXTLINE
dynamic_array<T>::dynamic_array(std::size_t sz) : size_ {sz}, vec_ {alloc_.allocate(size_)} {}

template <typename T>
using dynamic_vector =
    std::vector<T,
    boost::alignment::aligned_allocator<T, alignment>>;

template <typename T>
auto generate_trivial_array(std::size_t max) {
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  trivial_dynamic_array<T> v(max);
  auto t2 = high_resolution_clock::now();
  for (std::size_t i = 0; i < max; ++i) {
    v[i] = 1.0F / gsl::narrow_cast<float>(i);
  }
  auto t3 = high_resolution_clock::now();
  fmt::print("Create: {}\n", duration_cast<microseconds>(t2-t1));
  fmt::print("Fill: {}\n", duration_cast<microseconds>(t3-t2));
  return v;
}

template <typename T>
auto generate_array(std::size_t max) {
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  dynamic_array<T> v(max);
  auto t2 = high_resolution_clock::now();
  for (std::size_t i = 0; i < max; ++i) {
    v[i] = 1.0F / gsl::narrow_cast<float>(i);
  }
  auto t3 = high_resolution_clock::now();
  fmt::print("Create: {}\n", duration_cast<microseconds>(t2-t1));
  fmt::print("Fill: {}\n", duration_cast<microseconds>(t3-t2));
  return v;
}

template <typename T>
auto generate_vector(std::size_t max) {
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  dynamic_vector<T> v;
  v.reserve(max);
  auto t2 = high_resolution_clock::now();
  for (std::size_t i = 0; i < max; ++i) {
    v.push_back(1.0F / gsl::narrow_cast<float>(i));
  }
  auto t3 = high_resolution_clock::now();
  fmt::print("Create: {}\n", duration_cast<microseconds>(t2-t1));
  fmt::print("Fill: {}\n", duration_cast<microseconds>(t3-t2));
  return v;
}



#endif//CPP_MICROBENCH_ALIGNED_CONT_H
