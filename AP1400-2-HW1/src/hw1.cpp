#include "hw1.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace algebra {

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

Matrix zeros(size_t n, size_t m) {
  Matrix res(n, std::vector<double>(m));
  return res;
}

Matrix ones(size_t n, size_t m) {
  Matrix res(n, std::vector<double>(m, 1));
  return res;
}

Matrix random(size_t n, size_t m, double min, double max) {
  try {
    if (min > max) {
      throw std::logic_error("min must not greater than max.");
    }
    Matrix res(n, std::vector<double>(m));
    auto eng = std::uniform_real_distribution<double>(min, max);
    for (auto &row : res) {
      for (auto &x : row) {
        x = eng(rng);
      }
    }
    return res;
  } catch (...) {
    throw;
  }
}

void show(const Matrix &matrix) {
  if (matrix.empty()) {
    return;
  }

  int m = matrix[0].size();
  auto display_line = [&]() {
    std::cout << "-";
    std::cout << std::string(10 * m - 1, ' ');
    std::cout << "-\n";
  };

  display_line();
  for (auto &row : matrix) {
    std::cout << "|";
    for (int j = 0; j < m; j++) {
      std::cout << std::setw(9) << std::right << std::fixed
                << std::setprecision(3) << row[j];
      if (j == m - 1) {
        std::cout << "|\n";
      } else {
        std::cout << " ";
      }
    }
  }
  display_line();
}

Matrix multiply(const Matrix &matrix, double c) {
  auto res = matrix;
  for (auto &row : res) {
    for (auto &x : row) {
      x *= c;
    }
  }
  return res;
}

Matrix multiply(const Matrix &matrix1, const Matrix &matrix2) {
  try {
    if (matrix1.empty()) {
      return Matrix();
    }

    int n = matrix1.size(), m = matrix1[0].size();

    if (matrix2.size() != m) {
      throw std::logic_error("these two matrices cannot be multiplied.");
    }

    if (m == 0) {
      return Matrix();
    }

    int p = matrix2[0].size();

    Matrix res(n, std::vector<double>(p));
    for (int j = 0; j < p; j++) {
      for (int k = 0; k < m; k++) {
        for (int i = 0; i < n; i++) {
          res[i][j] += matrix2[k][j] * matrix1[i][k];
        }
      }
    }

    return res;
  } catch (...) {
    throw;
  }
}

Matrix sum(const Matrix &matrix, double c) {
  auto res = matrix;
  for (auto &row : res) {
    for (auto &x : row) {
      x += c;
    }
  }
  return res;
}

Matrix sum(const Matrix &matrix1, const Matrix &matrix2) {
  try {
    if (matrix1.empty()) {
      if (!matrix2.empty()) {
        throw std::logic_error("these two matrices cannot be added.");
      }
      return Matrix();
    }

    int n = matrix1.size(), m = matrix1[0].size();

    if (matrix2.size() != n || matrix2[0].size() != m) {
      throw std::logic_error("these two matrices cannot be added.");
    }

    Matrix res(matrix1);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        res[i][j] += matrix2[i][j];
      }
    }
    return res;
  } catch (...) {
    throw;
  }
}

Matrix transpose(const Matrix &matrix) {
  if (matrix.empty()) {
    return matrix;
  }

  int n = matrix.size(), m = matrix[0].size();

  Matrix res = zeros(m, n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      res[j][i] = matrix[i][j];
    }
  }
  return res;
}

Matrix minor(const Matrix &matrix, size_t n, size_t m) {
  try {
    if (n < 0 || m < 0) {
      throw std::logic_error("n or m invalid.");
    }

    int N = matrix.size();
    if (n > N) {
      throw std::logic_error("n or m invalid.");
    }

    int M = matrix[0].size();
    if (m > M) {
      throw std::logic_error("n or m invalid.");
    }

    if (N - 1 == 0 || M - 1 == 0) {
      return zeros(0, 0);
    }

    auto res = zeros(N - 1, M - 1);
    for (int i = 0; i < N; i++) {
      if (i == n) {
        continue;
      }

      int exact_i = i < n ? i : i - 1;

      for (int j = 0; j < M; j++) {
        if (j == m) {
          continue;
        }

        int exact_j = j < m ? j : j - 1;

        res[exact_i][exact_j] = matrix[i][j];
      }
    }

    return res;
  } catch (...) {
    throw;
  }
}

int sgn(int i, int j) { // to compute determinant
  return (i + j) % 2 == 0 ? 1 : -1;
}
double determinant(const Matrix &matrix) {
  try {
    if (matrix.empty()) {
      return 1;
    }

    if (matrix.size() != matrix[0].size()) {
      throw std::logic_error("This matrix has not determinant.");
    }

    int n = matrix.size();

    double res = 0;
    for (int j = 0; j < n; j++) {
      res += matrix[0][j] * sgn(0, j) * determinant(minor(matrix, 0, j));
    }
    return res;
  } catch (...) {
    throw;
  }
}

Matrix inverse(const Matrix &matrix) {
  try {
    if (std::abs(determinant(matrix)) < 1e-12) {
      throw std::logic_error("this matrix has not inverse.");
    }

    int n = matrix.size();
    auto argu = zeros(n, 2 * n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        argu[i][j] = matrix[i][j];
      }
      argu[i][i + n] = 1;
    }

    for (int i = 0; i < n; i++) { // Gaussian Elimination
      for (int j = i; j < n; j++) {
        if (std::abs(argu[j][i]) > 1e-12) {
          std::swap(argu[i], argu[j]);
          break;
        }
      }
      auto r = argu[i][i];
      for (int j = 0; j < 2 * n; j++) {
        argu[i][j] /= r;
      }
      for (int j = 0; j < n; j++) {
        if (j == i) {
          continue;
        }
        auto r = argu[j][i];
        for (int k = 0; k < 2 * n; k++) {
          argu[j][k] -= r * argu[i][k];
        }
      }
    }

    auto res = zeros(n, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        res[i][j] = argu[i][j + n];
      }
    }

    return res;
  } catch (...) {
    throw;
  }
}

Matrix concatenate(const Matrix &matrix1, const Matrix &matrix2, int axis = 0) {
  try {
    if (axis == 0) {
      if (matrix1.size() == 0) {
        return matrix2;
      }
      int m = matrix1[0].size();
      if (matrix2.size() == 0) {
        return matrix1;
      }
      if (matrix2[0].size() != m) {
        throw std::logic_error("these two matrices can not be concatenated.");
      }

      auto res = matrix1;
      for (auto &row : matrix2) {
        res.emplace_back(row);
      }
      return res;
    } else if (axis == 1) {
      if (matrix1.size() == 0) {
        return matrix2;
      }
      int n = matrix1.size();
      if (matrix2.size() == 0) {
        return matrix1;
      }
      if (matrix2.size() != n) {
        throw std::logic_error("these two matrices can not be concatenated.");
      }

      auto res = matrix1;
      for (int i = 0; i < n; i++) {
        for (auto &x : matrix2[i]) {
          res[i].emplace_back(x);
        }
      }
      return res;
    } else {
      throw std::logic_error("input of axis is invalid.");
    }
  } catch (...) {
    throw;
  }
}

Matrix ero_swap(const Matrix &matrix, size_t r1, size_t r2) {
  try {
    int n = matrix.size();
    if (r1 < 0 || r2 < 0 || r1 >= n || r2 >= n) {
      throw std::logic_error("input of row-number is invalid.");
    }

    auto res = matrix;
    std::swap(res[r1], res[r2]);
    return res;
  } catch (...) {
    throw;
  }
}

Matrix ero_multiply(const Matrix &matrix, size_t r, double c) {
  try {
    int n = matrix.size();
    if (r < 0 || r >= n) {
      throw std::logic_error("input of row-number is invalid.");
    }

    auto res = matrix;
    for (auto &x : res[r]) {
      x *= c;
    }
    return res;
  } catch (...) {
    throw;
  }
}

Matrix ero_sum(const Matrix &matrix, size_t r1, double c, size_t r2) {
  try {
    int n = matrix.size();
    if (r1 < 0 || r2 < 0 || r1 >= n || r2 >= n) {
      throw std::logic_error("input of row-number is invalid.");
    }
    auto res = matrix;
    int m = matrix[0].size();
    for (int j = 0; j < m; j++) {
      res[r2][j] += res[r1][j] * c;
    }
    return res;
  } catch (...) {
    throw;
  }
}

Matrix upper_triangular(const Matrix &matrix) {
  try {
    int n = matrix.size();
    if (n == 0) {
      return matrix;
    }

    int m = matrix[0].size();
    if (m > n) {
      throw std::logic_error(
          "this matrix cannot be converted to upper triangular.");
    }

    auto res = matrix;
    
    for (int i = 0; i < n; i++) {
      if (std::abs(res[i][i]) < 1e-12) {
        for (int j = i + 1; j < n; j++) {
          if (std::abs(res[j][i]) > 1e-12) {
            res = ero_swap(res, i, j);
            break;
          }
        }
      }

      for (int j = i + 1; j < n; j++) {
        double r = -res[j][i] / res[i][i];
        res = ero_sum(res, i, r, j);
      }
    }

    return res;
  } catch (...) {
    throw;
  }
}

} // namespace algebra