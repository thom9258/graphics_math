#include <gtest/gtest.h>

#include "../../include/graphics_math/vec4.hpp"

#include <format>

using namespace graphics_math;

namespace testing::internal {
template <>
class UniversalPrinter<vec4f> {
public:
    static void Print(const vec4f& v, ::std::ostream* os) {
        *os << std::format("{}", v);
    }
};
}


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
