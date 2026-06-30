#pragma once

#include <cstdint>

namespace naive
{

class mat4f {
public:
  constexpr mat4f() = default;
  static constexpr auto identity() -> mat4f {
    mat4f result{};
    for (std::size_t i = 0; i < result.rows(); i++) {
      for (std::size_t j = 0; j < result.cols(); j++) {
        result.at(i, j) = (i == j) ? 1.0f : 0.0f;
      }
    }

    return result;
  }

  constexpr auto rows() const -> std::size_t { return 4; };
  constexpr auto cols() const -> std::size_t { return 4; };
  constexpr auto get(std::size_t row, std::size_t col) const -> float {
    return data[row][col];
  };
  constexpr auto at(std::size_t row, std::size_t col) -> float & {
    return data[row][col];
  };

  constexpr auto operator*(const mat4f &other) -> mat4f {
    mat4f result{};
    for (std::size_t i = 0; i < rows(); i++) {
      for (std::size_t j = 0; j < other.cols(); j++) {
        at(i, j) = 0;
        for (std::size_t k = 0; k < rows(); k++) {
          result.at(i, j) += get(i, k) * other.get(k, j);
        }
      }
    }

    return result;
  }

private:
  float data[4][4];
};

}
