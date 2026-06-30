#pragma once

#include <emmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>

#include <cmath>
#include <numeric>

namespace graphics_math {

struct fill_t {
  explicit constexpr fill_t(int ignore) : _ignore{ignore} {}

private:
  int _ignore{};
};

static constexpr const fill_t fill{42};

class vec4f {
public:
  constexpr vec4f() = default;
  constexpr ~vec4f() = default;
  explicit constexpr vec4f(fill_t, float value) : _data{_mm_set1_ps(value)} {}
  explicit constexpr vec4f(__m128 data) : _data{data} {}
  constexpr vec4f(const vec4f &) = default;
  constexpr vec4f(vec4f &&) = default;
  constexpr vec4f &operator=(const vec4f &) = default;
  constexpr vec4f &operator=(vec4f &&) = default;

  constexpr vec4f(float a, float b, float c, float d)
      : _data{_mm_set_ps(a, b, c, d)} {}

  constexpr auto operator==(const vec4f &other) const -> bool {
    return _mm_movemask_ps(_mm_cmpeq_ps(_data, other._data)) == 0xF;
  }

  constexpr auto operator!=(const vec4f &other) const -> bool {
    return !(*this == other);
  }

  constexpr auto operator*(const vec4f &other) const -> vec4f {
    return vec4f(_mm_mul_ps(_data, other._data));
  }

  constexpr auto operator+(const vec4f &other) const -> vec4f {
    return vec4f(_mm_add_ps(_data, other._data));
  }

  constexpr auto operator-(const vec4f &other) const -> vec4f {
    return vec4f(_mm_sub_ps(_data, other._data));
  }

  constexpr auto operator/(const vec4f &other) const -> vec4f {
    return vec4f(_mm_div_ps(_data, other._data));
  }

  constexpr auto operator/(float divisor) const -> vec4f {
    return *this / vec4f(fill, divisor);
  }

  constexpr auto operator[](std::size_t i) const -> float {
    alignas(128) float tmp[4];
    copy_to(tmp);
    return tmp[i];
  }

  constexpr auto copy_to(float *values) const -> void {
    if (values == nullptr) {
      return;
    }

    alignas(128) float tmp[4];
    _mm_store_ps(tmp, _data);
    values[0] = tmp[3];
    values[1] = tmp[2];
    values[2] = tmp[1];
    values[3] = tmp[0];
  }

  constexpr auto sum() const -> float {
    __m128 const sum64 = _mm_add_ps(_data, _mm_movehl_ps(_data, _data));
    __m128 const sum = _mm_add_ss(sum64, _mm_shuffle_ps(sum64, sum64, 1));
    return _mm_cvtss_f32(sum);
  }

  constexpr auto length() const -> float {
    __m128 const dxd = _mm_mul_ps(_data, _data);
    __m128 const sum64 = _mm_add_ps(dxd, _mm_movehl_ps(dxd, dxd));
    __m128 const dot = _mm_add_ss(sum64, _mm_shuffle_ps(sum64, sum64, 1));
    __m128 const sqrt = _mm_sqrt_ss(dot);
    return _mm_cvtss_f32(sqrt);
  }

  constexpr auto data() const -> __m128 { return _data; }

private:
  __m128 _data;
};

static_assert(sizeof(vec4f) == sizeof(float) * 4);

constexpr auto normalize(vec4f v) -> vec4f {
  float const vvsum = (v * v).sum();
  float const w = std::sqrt(vvsum);
  return v / vec4f(fill, w);
}

} // namespace graphics_math
