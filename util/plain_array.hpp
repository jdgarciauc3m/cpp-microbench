#ifndef CPP_MICROBENCH_PLAIN_ARRAY_HPP
#define CPP_MICROBENCH_PLAIN_ARRAY_HPP

template <typename T>
class plain_array {
  static_assert(std::is_trivial_v<T>);

public:
  plain_array() = default;

  explicit plain_array(std::size_t sz);

  plain_array(const plain_array &) = delete;
  plain_array & operator=(const plain_array &) = delete;

  plain_array(plain_array && da) noexcept : size_ {da.size_},
                                            vec_ {std::move(da.vec_)} {
    da.size_ = 0;
  }

  plain_array & operator=(plain_array && da) noexcept {
    size_ = da.size_;
    da.size_ = 0;
    vec_ = std::move(da.vec_);
    return *this;
  }

  ~plain_array() noexcept = default;

  T & operator[](std::size_t i) { return vec_[i]; }
  const T & operator[](std::size_t i) const { return vec_[i]; }
  [[nodiscard]] std::size_t size() const { return size_; }
  [[nodiscard]] auto begin() const { return vec_.get(); }
  [[nodiscard]] auto end() const { return vec_.get() + size_; }

private:
  std::size_t size_ = 0;
  std::unique_ptr<T[]> vec_ = nullptr;/// NOLINT
};

template <typename T>
// NOLINTNEXTLINE
plain_array<T>::plain_array(std::size_t sz)
    : size_ {sz},
      vec_ {std::make_unique<T[]>(size_)} {}

#endif//CPP_MICROBENCH_PLAIN_ARRAY_HPP
