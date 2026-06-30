#pragma once

#include "detail.hpp"

#include <emmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>

#include <cmath>
#include <numeric>

namespace graphics_math {

struct alignas(detail::simd_aligned) vec4f {
public:
  static constexpr auto identity() -> vec4f;

  constexpr vec4f() = default;
  constexpr ~vec4f() = default;
  constexpr vec4f(const vec4f &) = default;
  constexpr vec4f(vec4f &&) = default;
  constexpr vec4f &operator=(const vec4f &) = default;
  constexpr vec4f &operator=(vec4f &&) = default;

  explicit constexpr vec4f(__m128 data);
  explicit constexpr vec4f(detail::fill_tag, float value);
  constexpr vec4f(float a, float b, float c, float d);

  constexpr auto size() const -> std::size_t;

  constexpr auto operator==(const vec4f &other) const -> bool;
  constexpr auto operator!=(const vec4f &other) const -> bool;
  constexpr auto operator*(const vec4f &other) const -> vec4f;
  constexpr auto operator+(const vec4f &other) const -> vec4f;
  constexpr auto operator-(const vec4f &other) const -> vec4f;
  constexpr auto operator/(float divisor) const -> vec4f;

  constexpr auto operator[](std::size_t i) const -> float;

  constexpr auto set(std::size_t index, float value) -> void;

  constexpr auto store_aligned(float *values) const -> void;
  constexpr auto store_unaligned(float *values) const -> void;

  constexpr auto sum() const -> float;
  constexpr auto length() const -> float;

  constexpr auto data() const -> __m128;

private:
  __m128 _data;
};

static_assert(sizeof(vec4f) == sizeof(float) * 4);

constexpr auto vec4f::identity() -> vec4f {
  return vec4f(0.0f, 0.0f, 0.0f, 1.0f);
}

constexpr vec4f::vec4f(detail::fill_tag, float value)
    : _data{_mm_set1_ps(value)} {}

constexpr vec4f::vec4f(float a, float b, float c, float d)
    : _data{_mm_setr_ps(a, b, c, d)} {}

constexpr vec4f::vec4f(__m128 data) : _data{data} {}

constexpr auto vec4f::operator==(const vec4f &other) const -> bool {
  return _mm_movemask_ps(_mm_cmpeq_ps(_data, other._data)) == 0xF;
}

constexpr auto vec4f::operator!=(const vec4f &other) const -> bool {
  return !(*this == other);
}

constexpr auto vec4f::operator*(const vec4f &other) const -> vec4f {
  return vec4f(_mm_mul_ps(_data, other._data));
}

constexpr auto vec4f::operator+(const vec4f &other) const -> vec4f {
  return vec4f(_mm_add_ps(_data, other._data));
}

constexpr auto vec4f::operator-(const vec4f &other) const -> vec4f {
  return vec4f(_mm_sub_ps(_data, other._data));
}

constexpr auto vec4f::operator/(float divisor) const -> vec4f {
  return vec4f(_mm_div_ps(_data, _mm_set1_ps(divisor)));
}

constexpr auto vec4f::operator[](std::size_t i) const -> float {
  alignas(detail::simd_aligned) float values[4];
  _mm_store_ps(values, _data);
  return values[i];
}

constexpr auto vec4f::store_aligned(float *values) const -> void {
  if (values != nullptr) {
    _mm_store_ps(values, _data);
  }
}

constexpr auto vec4f::store_unaligned(float *values) const -> void {
  if (values != nullptr) {
    _mm_storeu_ps(values, _data);
  }
}

constexpr auto vec4f::set(std::size_t index, float value) -> void {
  alignas(detail::simd_aligned) float values[4];
  _mm_store_ps(values, _data);
  values[index] = value;
  _data = _mm_load_ps(values);
}

constexpr auto vec4f::sum() const -> float {
  __m128 const sum64 = _mm_add_ps(_data, _mm_movehl_ps(_data, _data));
  __m128 const sum = _mm_add_ss(sum64, _mm_shuffle_ps(sum64, sum64, 1));
  return _mm_cvtss_f32(sum);
}

constexpr auto vec4f::length() const -> float {
  __m128 const dxd = _mm_mul_ps(_data, _data);
  __m128 const sum64 = _mm_add_ps(dxd, _mm_movehl_ps(dxd, dxd));
  __m128 const dot = _mm_add_ss(sum64, _mm_shuffle_ps(sum64, sum64, 1));
  __m128 const sqrt = _mm_sqrt_ss(dot);
  return _mm_cvtss_f32(sqrt);
}

constexpr auto vec4f::size() const -> std::size_t { return 4; }

constexpr auto vec4f::data() const -> __m128 { return _data; }

constexpr auto normalize(vec4f v) -> vec4f {
  float const vxvsum = (v * v).sum();
  float const w = std::sqrt(vxvsum);
  return v / w;
}

} // namespace graphics_math

#include <format>

namespace std {

template <> struct formatter<graphics_math::vec4f> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  constexpr auto format(const graphics_math::vec4f &v, format_context &ctx) const {
    return format_to(ctx.out(), "[{} {} {} {}]", v[0], v[1], v[2], v[3]);
  }
};

static_assert(requires (graphics_math::vec4f v) { std::format("{}", v); } );

} // namespace std
