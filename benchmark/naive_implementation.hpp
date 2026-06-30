#pragma once

namespace naive
{

class vec4f {
public:
  constexpr vec4f() = default;
  constexpr vec4f(float a, float b, float c, float d)
      : data{a, b, c, d} {}

  constexpr auto operator*(const vec4f &other) -> vec4f {
    return {data[0] * other.data[0], data[1] * other.data[1],
            data[2] * other.data[2], data[3] * other.data[3]};
  }

private:
  float data[4];
};

class matrix4x4f {
public:
  constexpr matrix4x4f() = default;
  static constexpr auto identity() -> matrix4x4f {
    matrix4x4f result{};
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

  constexpr auto operator*(const matrix4x4f &other) -> matrix4x4f {
    matrix4x4f result{};
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
