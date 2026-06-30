#include <gtest/gtest.h>

#include <print>

#include "../include/graphics_math/vec4.hpp"
// using namespace graphics_math;

#if 0
void print(std::string_view name, vector_concept auto v) {
  std::println("{}: {}", name, to_string(v));
}

void test_plus() {
  vec3 a;
  a.set(0, 1.0f);
  a.set(1, -1.0f);
  a.set(2, 7.0f);

  vec3 b;
  b.set(0, 10.0f);
  b.set(1, 20.0f);
  b.set(2, 30.0f);

  auto apb = add(a, b);
  auto asb = subtract(a, b);
  auto amb = multiply(a, b);

  print("a", a);
  print("b", b);
  print("a+b", apb);
  print("a-b", asb);
  print("a*b", amb);
}

void test_pack_init() {
  vec3 a(0.0f, 1.0f, 2.0f);
  print("a", a);
}

void test_assignment() {
  vec3 a(0.0f, 1.0f, 2.0f);
  vec3 b = a;
  std::println("a = {}", to_string(a));
  std::println("b = {}", to_string(b));
}

void test_matrix_identity_mult() {
  mat4 a = mat4::identity();
  mat4 b = mat4::identity();
  mat4 amb = naive_multiply(a, b);
  std::println("a*b = {}", to_string(amb));
}

void test_translate() {
  mat4 const m = mat4::identity();

  vec3 const translation(3.5f, -2.1f, 7.0f);
  mat4 const translated = translate(translation, m);
  std::println("translated = {}", to_string(translated));

  vec3 const scalation(0.1f, 0.2f, 0.3f);
  mat4 const scaled = scale(scalation, m);
  std::println("scaled = {}", to_string(scaled));

  mat4 const translated_scaled = translate(translation, scale(scalation, m));
  std::println("translated+scaled = {}", to_string(translated_scaled));
}
#endif

using namespace graphics_math;

TEST(vec4f, construction) {
  vec4f a(1.0f, 2.0f, 3.0f, 4.0f);

  EXPECT_EQ(a[0], 1.0f);
  EXPECT_EQ(a[1], 2.0f);
  EXPECT_EQ(a[2], 3.0f);
  EXPECT_EQ(a[3], 4.0f);
}

TEST(vec4f, compare) {

  vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
  vec4f b = a;
  vec4f c(2.0f, 2.0f, 2.0f, 2.0f);

  EXPECT_EQ(a, a);
  EXPECT_EQ(a, b);
  EXPECT_EQ(b, a);
  EXPECT_NE(a, c);
  EXPECT_NE(c, b);
}

TEST(vec4f, multiplication) {
  vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
  vec4f b(2.0f, 2.0f, 2.0f, 2.0f);
  vec4f c = a * b;
  EXPECT_EQ(c, vec4f(2.0f, 4.0f, 6.0f, 8.0f));
}

TEST(vec4f, sum) {
  vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
  EXPECT_EQ(a.sum(), 10.0f);

  vec4f b(fill, 2.0f);
  EXPECT_EQ(b.sum(), 8.0f);
}

TEST(vec4f, length) {
  vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
  EXPECT_EQ(a.length(), 5.47722578f);

  vec4f b(fill, 2.0f);
  EXPECT_EQ(b.length(), 4.0f);
}

TEST(vec4f, normalize) {

  auto const a = normalize(vec4f(0.0f, 5.0f, 0.0f, 0.0f));
  EXPECT_EQ(a.length(), 1.0f);
  EXPECT_EQ(a[0], 0.0f);
  EXPECT_EQ(a[1], 1.0f);
  EXPECT_EQ(a[2], 0.0f);
  EXPECT_EQ(a[3], 0.0f);

  auto const b = normalize(vec4f(fill, 27.3f));
  EXPECT_EQ(b.length(), 1.0f);
  EXPECT_EQ(b, vec4f(fill, 0.5f));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
