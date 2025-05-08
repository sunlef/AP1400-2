#ifndef SHARED_PTR
#define SHARED_PTR

#include <cstddef>
#include <utility>

template <typename T> class SharedPtr {
public:
  explicit SharedPtr(T *q) : _p(q), _cnt(new size_t(1)) {}

  explicit SharedPtr() : _p(nullptr), _cnt(nullptr) {}

  ~SharedPtr() { reset(); }

  T *get() const { return _p; }

  size_t use_count() const { return _cnt ? *_cnt : 0; }

  explicit SharedPtr(const SharedPtr &other) : _p(other._p), _cnt(other._cnt) {
    if (_cnt) {
      ++(*_cnt);
    }
  }

  SharedPtr &operator=(const SharedPtr<T> &other) {
    if (this != &other) {
      if (_cnt && --*_cnt == 0) {
        delete _p;
        delete _cnt;
      }
      _p = other._p;
      _cnt = other._cnt;
      if (_cnt) {
        ++*_cnt;
      }
    }
    return *this;
  }

  T &operator*() const { return *_p; }

  T *operator->() const { return _p; }

  void reset() {
    if (_cnt && --*_cnt == 0) {
      delete _p;
      delete _cnt;
    }
    _p = nullptr;
    _cnt = nullptr;
  }

  void reset(T *q) {
    if (_cnt && --*_cnt == 0) {
      delete _p;
      delete _cnt;
    }
    _p = q;
    _cnt = new size_t(1);
  }

  explicit operator bool() const { return _p != nullptr; }

private:
  T *_p;
  size_t *_cnt;
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args &&...args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
#endif // SHARED_PTR