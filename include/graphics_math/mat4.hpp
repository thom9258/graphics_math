#pragma once

#include "vec4.hpp"

#include <emmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>

#include <cmath>
#include <numeric>

namespace graphics_math {

struct alignas(detail::simd_aligned) mat4f {
public:
  constexpr mat4f() = default;
  constexpr ~mat4f() = default;
  constexpr mat4f(const mat4f &) = default;
  constexpr mat4f(mat4f &&) = default;
  constexpr mat4f &operator=(const mat4f &) = default;
  constexpr mat4f &operator=(mat4f &&) = default;

  constexpr mat4f(vec4f r0, vec4f r1, vec4f r2, vec4f r3);

  static constexpr auto identity() -> mat4f;
  constexpr auto rows() const -> std::size_t;
  constexpr auto cols() const -> std::size_t;

  constexpr auto operator[](std::size_t row) const -> const vec4f &;

  constexpr auto set(std::size_t row, std::size_t col, float value) -> void;

  constexpr auto operator*(const mat4f &other) -> mat4f;

private:
  vec4f _data[4];
};

constexpr mat4f::mat4f(vec4f r0, vec4f r1, vec4f r2, vec4f r3)
    : _data{r0, r1, r2, r3} {}

constexpr auto mat4f::identity() -> mat4f {
  return mat4f(vec4f(1.0f, 0.0f, 0.0f, 0.0f), vec4f(0.0f, 1.0f, 0.0f, 0.0f),
               vec4f(0.0f, 0.0f, 1.0f, 0.0f), vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

constexpr auto mat4f::rows() const -> std::size_t { return 4; };
constexpr auto mat4f::cols() const -> std::size_t { return 4; };

constexpr auto mat4f::operator[](std::size_t row) const -> const vec4f & {
  return _data[row];
}

constexpr auto mat4f::set(std::size_t row, std::size_t col, float value)
    -> void {
  _data[row].set(col, value);
}

// linear combination:
// a[0] * B.row[0] + a[1] * B.row[1] + a[2] * B.row[2] + a[3] * B.row[3]
auto lincomb(vec4f v, const mat4f &B) -> vec4f {
  __m128 a = v.data();
  __m128 result = _mm_mul_ps(_mm_shuffle_ps(a, a, 0x00), B[0].data());
  result =
      _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0x55), B[1].data()));
  result =
      _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xaa), B[2].data()));
  result =
      _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xff), B[3].data()));
  return vec4f(result);
}

constexpr auto mat4f::operator*(const mat4f &other) -> mat4f {
  //  out_ij = sum_k a_ik b_kj
  //  => out_0j = a_00 * b_0j + a_01 * b_1j + a_02 * b_2j + a_03 * b_3j
  return mat4f(lincomb(_data[0], other), lincomb(_data[1], other),
               lincomb(_data[2], other), lincomb(_data[3], other));
}

} // namespace graphics_math
