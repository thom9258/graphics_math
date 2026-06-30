#include <benchmark/benchmark.h>

#include "../include/graphics_math/vec4.hpp"

#include "naive_implementation.hpp"
#include "sse2_implementation.hpp"

#include <glm/glm.hpp>


namespace bm = benchmark;

static void bm_naive_vec4_multiply(bm::State &state) {
  naive::vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
  naive::vec4f b(2.0f, 4.0f, 8.0f, 16.0f);

  for (auto _ : state) {
  naive::vec4f c = a * b;
    bm::DoNotOptimize(c);
  }

  bm::DoNotOptimize(a);
  bm::DoNotOptimize(b);
}

static void bm_sse2_vec4_multiply(bm::State &state) {
  sse2::vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
  sse2::vec4f b(2.0f, 4.0f, 8.0f, 16.0f);

  for (auto _ : state) {
	sse2::vec4f c  = a * b;
    bm::DoNotOptimize(c);
  }

  bm::DoNotOptimize(a);
  bm::DoNotOptimize(b);
}

static void bm_glm_vec4_multiply(bm::State &state) {
  glm::vec4 a(1.0f, 2.0f, 3.0f, 4.0f);
  glm::vec4 b(2.0f, 4.0f, 8.0f, 16.0f);

  for (auto _ : state) {
	  glm::vec4 c = a * b;
    bm::DoNotOptimize(c);
  }

  bm::DoNotOptimize(a);
  bm::DoNotOptimize(b);
}

static void bm_gm_vec4_multiply(bm::State &state) {
	namespace gm = graphics_math;
	gm::vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	gm::vec4f b(2.0f, 4.0f, 8.0f, 16.0f);

  for (auto _ : state) {
   gm::vec4f c = a * b;
    bm::DoNotOptimize(c);
  }

  bm::DoNotOptimize(a);
  bm::DoNotOptimize(b);
}

static void bm_naive_mat4_multiply(bm::State &state) {
  auto a = naive::matrix4x4f::identity();
  auto b = naive::matrix4x4f::identity();
  for (auto _ : state) {
    auto c = a * b;
    bm::DoNotOptimize(c);
  }

  bm::DoNotOptimize(a);
  bm::DoNotOptimize(b);
}

static void bm_sse2_mat4_multiply(bm::State &state) {
  auto a = sse2::matrix4x4f::identity();
  auto b = sse2::matrix4x4f::identity();

  for (auto _ : state) {
  auto c = a * b;
    bm::DoNotOptimize(c);
  }

  bm::DoNotOptimize(a);
  bm::DoNotOptimize(b);
}

static void bm_glm_mat4_multiply(bm::State &state) {
  glm::mat4 a(1.0f);
  glm::mat4 b(1.0f);

  for (auto _ : state) {
    glm::mat4 c = a * b;
    bm::DoNotOptimize(c);
  }

  bm::DoNotOptimize(a);
  bm::DoNotOptimize(b);
}

BENCHMARK(bm_gm_vec4_multiply);
BENCHMARK(bm_naive_vec4_multiply);
BENCHMARK(bm_sse2_vec4_multiply);
BENCHMARK(bm_glm_vec4_multiply);

BENCHMARK(bm_naive_mat4_multiply);
BENCHMARK(bm_sse2_mat4_multiply);
BENCHMARK(bm_glm_mat4_multiply);

BENCHMARK_MAIN();
