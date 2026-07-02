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

  constexpr mat4f(float v00, float v10, float v20, float v30, //
                  float v01, float v11, float v21, float v31, //
                  float v02, float v12, float v22, float v32, //
                  float v03, float v13, float v23, float v33);

  static constexpr auto identity() -> mat4f;

  constexpr auto operator==(const mat4f &other) const -> bool;
  constexpr auto operator!=(const mat4f &other) const -> bool;

  constexpr auto rows() const -> std::size_t;
  constexpr auto cols() const -> std::size_t;

  constexpr auto operator[](std::size_t row) const -> const vec4f &;

  constexpr auto set(std::size_t row, std::size_t col, float value) -> void;

  constexpr auto get(std::size_t row, std::size_t col) const -> float;

  constexpr auto operator*(const mat4f &other) const -> mat4f;
  constexpr auto operator*(vec4f v) const -> vec4f;

private:
  vec4f _data[4];
};

constexpr mat4f::mat4f(vec4f r0, vec4f r1, vec4f r2, vec4f r3)
    : _data{r0, r1, r2, r3} {}

constexpr mat4f::mat4f(float v00, float v10, float v20, float v30, //
                       float v01, float v11, float v21, float v31, //
                       float v02, float v12, float v22, float v32, //
                       float v03, float v13, float v23, float v33)
    : mat4f(vec4f(v00, v10, v20, v30), //
            vec4f(v01, v11, v21, v31), //
            vec4f(v02, v12, v22, v32), //
            vec4f(v03, v13, v23, v33)) {}

constexpr auto mat4f::identity() -> mat4f {
  return mat4f(vec4f(1.0f, 0.0f, 0.0f, 0.0f), //
               vec4f(0.0f, 1.0f, 0.0f, 0.0f), //
               vec4f(0.0f, 0.0f, 1.0f, 0.0f), //
               vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

constexpr auto mat4f::operator==(const mat4f &other) const -> bool {
  return _data[0] == other._data[0]    //
         && _data[1] == other._data[1] //
         && _data[2] == other._data[2] //
         && _data[3] == other._data[3];
}

constexpr auto mat4f::operator!=(const mat4f &other) const -> bool {
  return !(*this == other);
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

constexpr auto mat4f::get(std::size_t row, std::size_t col) const -> float {
  return _data[row][col];
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

constexpr auto mat4f::operator*(const mat4f &other) const -> mat4f {
  return mat4f(lincomb(_data[0], other), //
               lincomb(_data[1], other), //
               lincomb(_data[2], other), //
               lincomb(_data[3], other));
}

inline auto horizontal_sum(vec4f v) -> float {
  return v[0] + v[1] + v[2] + v[3];
}

constexpr auto mat4f::operator*(vec4f v) const -> vec4f {
  return vec4f(horizontal_sum(_data[0] * v), //
               horizontal_sum(_data[1] * v), //
               horizontal_sum(_data[2] * v), //
               horizontal_sum(_data[3] * v));
}

constexpr auto transpose(const mat4f &m) -> mat4f {
  mat4f out;
  for (std::size_t i = 0; i < m.rows(); i++) {
    for (std::size_t j = 0; j < m.rows(); j++) {
      out.set(j, i, m[i][j]);
    }
  }

  return out;
}

constexpr auto perspective_projection(float fov_y, float aspect, float near_z,
                                      float far_z) -> mat4f {
  // https://johannesugb.github.io/gpu-programming/setting-up-a-proper-vulkan-projection-matrix/
  float const v00 = aspect / std::tan(fov_y / 2);
  float const v11 = 1 / std::tan(fov_y / 2);
  float const v22 = far_z / (far_z - near_z);
  float const v23 = -((near_z * far_z) / (far_z - near_z));
  return mat4f(vec4f(v00, 0, 0, 0),   //
               vec4f(0, v11, 0, 0),   //
               vec4f(0, 0, v22, v23), //
               vec4f(0, 0, 1, 0));
}

} // namespace graphics_math

#include <format>

namespace std {

template <> struct formatter<graphics_math::mat4f> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  constexpr auto format(const graphics_math::mat4f &m,
                        format_context &ctx) const {
    return format_to(ctx.out(), "[{} {} {} {}]", m[0], m[1], m[2], m[3]);
  }
};

} // namespace std

static_assert(sizeof(graphics_math::mat4f) == sizeof(float) * 16);

static_assert(requires(const graphics_math::mat4f &m) {
  std::format("{}", m);
});
