#ifndef CPP_MICROBENCH_TRIVIAL_ALIGNED_ARRAY_HPP
#define CPP_MICROBENCH_TRIVIAL_ALIGNED_ARRAY_HPP

template <typename T>
class trivial_aligned_array {
  static_assert(std::is_trivial_v<T>);

public:
  trivial_aligned_array() noexcept = default;

  explicit trivial_aligned_array(std::size_t sz);

  trivial_aligned_array(const trivial_aligned_array &) = delete;
  trivial_aligned_array & operator=(const trivial_aligned_array &) = delete;

  trivial_aligned_array(trivial_aligned_array && da) noexcept
      : size_ {da.size_},
        buf_ {da.buf_},
        vec_ {da.vec_} {
    da.size_ = 0;
    da.buf_ = nullptr;
    da.vec_ = nullptr;
  }

  trivial_aligned_array & operator=(trivial_aligned_array && da) noexcept {
    delete[] buf_;
    size_ = da.size_;
    buf_ = da.buf_;
    vec_ = da.vec_;
    da.size_ = 0;
    da.buf_ = nullptr;
    da.vec_ = nullptr;
    return *this;
  }

  ~trivial_aligned_array() noexcept { delete[] buf_; }

  T & operator[](std::size_t i) noexcept { return vec_[i]; }
  const T & operator[](std::size_t i) const noexcept { return vec_[i]; }
  [[nodiscard]] std::size_t size() const noexcept { return size_; }
  [[nodiscard]] auto begin() const noexcept { return vec_; }
  [[nodiscard]] auto end() const noexcept { return vec_ + size_; }

private:
  static constexpr std::size_t pad = 256 / sizeof(T);
  static constexpr std::size_t linesize = 64;
  std::size_t size_ = 0;
  T * buf_ = nullptr;// NOLINT
  T * vec_ = nullptr;
};
template <typename T>
trivial_aligned_array<T>::trivial_aligned_array(std::size_t sz)
    : size_ {sz},
      buf_ {new T[sz + pad]},
      vec_ {reinterpret_cast<T *>(
          (reinterpret_cast<intptr_t>(buf_) + pad) & ~(linesize - 1))} {
}

#endif//CPP_MICROBENCH_TRIVIAL_ALIGNED_ARRAY_HPP
