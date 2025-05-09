#ifndef Q1_H
#define Q1_H

#include <cstdlib>

constexpr double eps = 1e-8;
constexpr int MAX_ITER = 10000;

class q1 {
public:
  template <typename T, typename Func>
  static double gradient_descent(T x, T alpha, Func func = Func()) {
    auto gradient = [&](T x) {
      return (func(x + eps) - func(x - eps)) / (2 * eps);
    };

    int iter = 0;
    while (std::abs(gradient(x)) > eps && ++iter <= MAX_ITER) {
      x -= alpha * gradient(x);
    }
    return x;
  }
};

#endif // Q1_H