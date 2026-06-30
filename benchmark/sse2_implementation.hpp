#pragma once

#include <emmintrin.h>

namespace sse2
{
	
class vec4f {
public:
  constexpr vec4f() = default;
  constexpr vec4f(__m128 data) : data{data} {}
  constexpr vec4f(float a, float b, float c, float d)
      : data{_mm_set_ps(a, b, c, d)} {}

  constexpr auto operator*(const vec4f &other) -> vec4f {
    return {_mm_mul_ps(data, other.data)};
  }

private:
  __m128 data;
};

class matrix4x4f {
public:
  constexpr matrix4x4f() = default;
  static constexpr auto identity() -> matrix4x4f {
    matrix4x4f result{};
    result.set_row(0, _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f));
    result.set_row(1, _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f));
    result.set_row(2, _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f));
    result.set_row(3, _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f));
    return result;
  }

  constexpr auto rows() const -> std::size_t { return 4; };
  constexpr auto cols() const -> std::size_t { return 4; };
  constexpr auto get(std::size_t row, std::size_t col) const -> float {
    return data[row][col];
  };

  constexpr auto get_row(std::size_t row) const -> __m128 { return data[row]; }

  constexpr auto set_row(std::size_t row, __m128 value) -> void {
    data[row] = value;
  };

  constexpr auto set(std::size_t row, std::size_t col, float value) -> void {
    float v[4];
    _mm_store_ps(v, data[row]);
    v[col] = value;
    set_row(row, _mm_set_ps(v[0], v[1], v[2], v[3]));
  };

  constexpr auto operator*(const matrix4x4f &other) -> matrix4x4f {
    matrix4x4f result{};
    // out_ij = sum_k a_ik b_kj
    // => out_0j = a_00 * b_0j + a_01 * b_1j + a_02 * b_2j + a_03 * b_3j
    result.set_row(0, lincomb(get_row(0), other));
    result.set_row(1, lincomb(get_row(1), other));
    result.set_row(2, lincomb(get_row(2), other));
    result.set_row(3, lincomb(get_row(3), other));
    return result;
  }

private:
  // linear combination:
  // a[0] * B.row[0] + a[1] * B.row[1] + a[2] * B.row[2] + a[3] * B.row[3]
  static inline __m128 lincomb(const __m128 &a, const matrix4x4f &B) {
    __m128 result = _mm_mul_ps(_mm_shuffle_ps(a, a, 0x00), B.get_row(0));
    result = _mm_add_ps(result,
                        _mm_mul_ps(_mm_shuffle_ps(a, a, 0x55), B.get_row(1)));
    result = _mm_add_ps(result,
                        _mm_mul_ps(_mm_shuffle_ps(a, a, 0xaa), B.get_row(2)));
    result = _mm_add_ps(result,
                        _mm_mul_ps(_mm_shuffle_ps(a, a, 0xff), B.get_row(3)));
    return result;
  }

  __m128 data[4];
};

}
