#ifndef UNIQUE_PTR
#define UNIQUE_PTR

#include <utility>
template <typename T> class UniquePtr {
public:
  explicit UniquePtr() noexcept : UniquePtr<T>(nullptr) {}

  explicit UniquePtr(T *q) noexcept : _p(q) {}

  UniquePtr(const UniquePtr &) = delete;

  UniquePtr &operator=(const UniquePtr &) = delete;

  explicit operator bool() const noexcept { return _p != nullptr; };

  ~UniquePtr() noexcept {
    delete _p;
  }

  T *get() const { return _p; }

  T &operator*() const { return *_p; };

  T *operator->() const { return get(); }

  void reset() {
    delete _p;
    _p = nullptr;
  }

  void reset(T *q) {
    delete _p;
    _p = q;
  }

  T *release() {
    T *tmp = _p;
    _p = nullptr;
    return tmp;
  }

private:
  T *_p;
};

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args &&...args) {
  return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

#endif // UNIQUE_PTR