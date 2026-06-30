#pragma once

#include <experimental/simd>

#include <concepts>
#include <initializer_list>
#include <limits>
#include <optional>
#include <sstream>
#include <type_traits>

namespace graphics_math {

struct vector_base_t {};

template <typename T>
concept vector_concept = requires(T t) {
  t[0];
  t.length();
} && std::derived_from<T, vector_base_t>;

template <typename t_scalar_type, std::size_t t_size_value>
struct vector_t : vector_base_t {
  using value_t = std::remove_cvref_t<t_scalar_type>;
  static constexpr std::size_t size = (t_size_value == 0) ? 1 : t_size_value;
  using simd_array_t = std::experimental::fixed_size_simd<value_t, size>;
  using simd_array_reference_t =
      std::experimental::fixed_size_simd<value_t, size>;

  explicit vector_t(simd_array_reference_t simd_array) : _values(simd_array) {}

  explicit constexpr vector_t() {}

  template <typename... Ts>
  explicit constexpr vector_t(Ts &&...ts)
    requires(sizeof...(Ts) == size)
      : vector_t({ts...}) {}

  constexpr auto get(std::size_t i) -> std::optional<value_t> {
    if (i > size) {
      return std::nullopt;
    }

    return _values[i];
  }

  constexpr auto unsafe_get(std::size_t i) -> value_t { return _values[i]; }

  constexpr auto operator[](std::size_t i) { return _values[i]; }

  constexpr auto unsafe_set(std::size_t i, value_t v) -> void {
    _values[i] = v;
  }

  constexpr auto set(std::size_t i, value_t v) -> bool {
    if (i > size) {
      return false;
    }

    _values[i] = v;
    return true;
  }

  constexpr auto length() const -> std::size_t { return size; }

  constexpr auto underlying_simd_array() -> simd_array_reference_t {
    return _values;
  }

private:
  explicit constexpr vector_t(std::initializer_list<value_t> init) {
    auto it = init.begin();
    for (std::size_t i = 0; i < init.size(); i++) {
      set(i, *it);
      it++;
    }
  }

  simd_array_t _values;
};

constexpr auto add(vector_concept auto lhs, decltype(lhs) rhs)
    -> decltype(lhs) {
  return decltype(lhs){lhs.underlying_simd_array() +
                       rhs.underlying_simd_array()};
}

// static_assert(std::invocable<add, vector_t<float, 3>, vector_t<float, 3>>);
// static_assert(not std::invocable<add, vector_t<float, 3>, vector_t<int, 3>>);

constexpr auto subtract(vector_concept auto lhs, decltype(lhs) rhs)
    -> decltype(lhs) {
  return decltype(lhs){lhs.underlying_simd_array() -
                       rhs.underlying_simd_array()};
}

constexpr auto multiply(vector_concept auto lhs, decltype(lhs) rhs)
    -> decltype(lhs) {
  return decltype(lhs){lhs.underlying_simd_array() *
                       rhs.underlying_simd_array()};
}

constexpr auto to_string(vector_concept auto v) -> std::string {
  std::stringstream ss;
  ss << "[";
  for (std::size_t i = 0; i < v.length(); i++) {
    if (i == v.length() - 1) {
      ss << v.unsafe_get(i);
    } else {
      ss << v.unsafe_get(i) << " ";
    }
  }

  ss << "]";
  return ss.str();
}

using vec2 = vector_t<float, 2>;
using vec3 = vector_t<float, 3>;
using vec4 = vector_t<float, 4>;
using ivec2 = vector_t<int, 2>;
using ivec3 = vector_t<int, 3>;

static_assert(vector_t<float, 0>::size == 1);
static_assert(vector_t<float, 2>::size == 2);
static_assert(vector_t<float, 3>::size == 3);

static_assert(
    std::is_constructible<vector_t<float, 3>, float, float, float>::value);
static_assert(not std::is_constructible_v<vec3, float, float>);
static_assert(not std::is_constructible_v<vec3, float, float, float, float>);

// TODO: I dont like this, maybe we can do some requires on types in the
// argument pack?
static_assert(std::is_constructible_v<vec3, int, int, float>);

static_assert(std::is_assignable_v<vec3, vec3>);
static_assert(not std::is_assignable_v<vec2, vec3>);
static_assert(not std::is_assignable_v<vec3, ivec3>);

struct matrix_base_t {};

template <typename T>
concept matrix_concept = requires(T t) {
  t[0][0];
  t.rows();
  t.cols();
} && std::derived_from<T, matrix_base_t>;

template <typename t_scalar_type, std::size_t t_rows_value,
          std::size_t t_cols_value>
struct matrix_t : matrix_base_t {
  using value_t = std::remove_cvref_t<t_scalar_type>;

  static constexpr std::size_t rows_v = (t_rows_value == 0) ? 1 : t_rows_value;
  static constexpr std::size_t cols_v = (t_cols_value == 0) ? 1 : t_cols_value;

  using rowvector_t = vector_t<value_t, rows_v>;
  using rowvector_reference_t = rowvector_t &;

  using container_t = matrix_t<value_t, rows_v, cols_v>;

  constexpr auto rows() const -> std::size_t { return rows_v; }
  constexpr auto cols() const -> std::size_t { return cols_v; }

  static constexpr auto identity() -> container_t {
    container_t res;
    std::size_t constexpr diameter = std::min(res.rows(), res.cols());

    for (std::size_t i = 0; i < diameter; i++) {
      for (std::size_t j = 0; j < diameter; j++) {
        if (i == j) {
          res.unsafe_set(i, j, 1);
        } else {
          res.unsafe_set(i, j, 0);
        }
      }
    }

    return res;
  }

  explicit constexpr matrix_t() {}

  constexpr auto get(std::size_t row, std::size_t col)
      -> std::optional<value_t> {
    if (row > rows() || col > cols()) {
      return std::nullopt;
    }

    return unsafe_get(row, col);
  }

  constexpr auto unsafe_get(std::size_t row, std::size_t col) -> value_t {
    return _rows[row][col];
  }

  constexpr auto operator[](std::size_t row) -> rowvector_reference_t {
    return _rows[row];
  }

  constexpr auto unsafe_set(std::size_t row, std::size_t col, value_t v)
      -> void {
    _rows[row].unsafe_set(col, v);
  }

  constexpr auto set(std::size_t row, std::size_t col, value_t v) -> bool {
    if (row > rows || col > cols) {
      return false;
    }

    unsafe_set(row, col, v);
    return true;
  }

  constexpr auto underlying_rowvector(std::size_t row)
      -> rowvector_reference_t {
    return _rows[row];
  }

private:
  rowvector_t _rows[cols_v];
};

constexpr auto naive_multiply(matrix_concept auto lhs, decltype(lhs) rhs)
    -> decltype(auto) {

  using value_t = typename decltype(lhs)::value_t;
  matrix_t<value_t, lhs.rows(), rhs.cols()> res;

  for (std::size_t i = 0; i < lhs.rows(); i++) {
    for (std::size_t j = 0; j < rhs.cols(); j++) {
      res[i][j] = 0;
      for (std::size_t k = 0; k < rhs.rows(); k++) {
        res[i][j] += lhs[i][k] * rhs[k][j];
      }
    }
  }

  return res;
}

constexpr auto to_string(matrix_concept auto m) -> std::string {
  std::stringstream ss;
  ss << "[";
  for (std::size_t i = 0; i < m.rows(); i++) {
    ss << "[";
    for (std::size_t j = 0; j < m.cols(); j++) {
      if (j == m.cols() - 1) {
        ss << m[i][j];
      } else {
        ss << m[i][j] << " ";
      }
    }
    ss << "]";

    if (i != m.rows() - 1) {
      ss << " ";
    }
  }

  ss << "]";
  return ss.str();
}

// https://stackoverflow.com/questions/18499971/efficient-4x4-matrix-multiplication-c-vs-assembly
#if 0
void matrix_multiply_c(float32_t *A, float32_t *B, float32_t *C, uint32_t n, uint32_t m, uint32_t k) {
    for (int i_idx=0; i_idx < n; i_idx++) {
        for (int j_idx=0; j_idx < m; j_idx++) {
            C[n*j_idx + i_idx] = 0;
            for (int k_idx=0; k_idx < k; k_idx++) {
                C[n*j_idx + i_idx] += A[n*k_idx + i_idx]*B[k*j_idx + k_idx];
            }
        }
    }
}

constexpr auto multiply(matrix_concept auto lhs, decltype(lhs) rhs)
    -> decltype(lhs) {

    vector_t<float, 4> row1 = rhs.underlying_rowvector(0);
    vector_t<float, 4> row2 = rhs.underlying_rowvector(1);
    vector_t<float, 4> row3 = rhs.underlying_rowvector(2);
    vector_t<float, 4> row4 = rhs.underlying_rowvector(3);

    for(int i=0; i<4; i++) {
        __m128 brod1 = _mm_set1_ps(A[4*i + 0]);
        __m128 brod2 = _mm_set1_ps(A[4*i + 1]);
        __m128 brod3 = _mm_set1_ps(A[4*i + 2]);
        __m128 brod4 = _mm_set1_ps(A[4*i + 3]);
        __m128 row = _mm_add_ps(
                    _mm_add_ps(
                        _mm_mul_ps(brod1, row1),
                        _mm_mul_ps(brod2, row2)),
                    _mm_add_ps(
                        _mm_mul_ps(brod3, row3),
                        _mm_mul_ps(brod4, row4)));
        _mm_store_ps(&C[4*i], row);
    }
}
#endif

using mat4 = matrix_t<float, 4, 4>;
using imat4 = matrix_t<float, 4, 4>;

constexpr auto translate(vec3 v, mat4 m) -> mat4 {
  m[3][0] += v[0];
  m[3][1] += v[1];
  m[3][2] += v[2];
  return m;
}

// https://codesignal.com/learn/courses/3d-worlds-and-matrix-transformations/lessons/implementing-glm-transformations
constexpr auto scale(vec3 v, mat4 m) -> mat4 {
  mat4 mv = mat4::identity();
  mv[0][0] = v[0];
  mv[1][1] = v[1];
  mv[2][2] = v[2];
  return naive_multiply(mv, m);
}
	
constexpr auto multiply(mat4 a, mat4 b)
    -> mat4 {

    vec4 a0 = a[0];
    vec4 a1 = a[1];
    vec4 a2 = a[2];
    vec4 a3 = a[3];
	
    vec4 b0 = b[0];
    vec4 b1 = b[1];
    vec4 b2 = b[2];
    vec4 b3 = b[3];

	mat4 c;
	c[0] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	c[1] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	c[2] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	c[3] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	c[0] += a0 * b0[0];
	c[1] += a1 * b0[1];
	c[2] += a2 * b0[2];
	c[3] += a3 * b0[3];



	return c;
}



} // namespace math
