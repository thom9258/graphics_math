#include <gtest/gtest.h>

#include "../../include/graphics_math/mat4.hpp"

using namespace graphics_math;

namespace testing::internal {
template <>
class UniversalPrinter<mat4f> {
public:
    static void Print(const mat4f& m, ::std::ostream* os) {
        *os << std::format("{}", m);
    }
};

}TEST(mat4f, construction) {
  mat4f m(vec4f(1.0f, 2.0f, 3.0f, 4.0f), vec4f(5.0f, 6.0f, 7.0f, 8.0f),
          vec4f(9.0f, 10.0f, 11.0f, 12.0f), vec4f(13.0f, 14.0f, 15.0f, 16.0f));

  EXPECT_EQ(m[0], vec4f(1.0f, 2.0f, 3.0f, 4.0f));
  EXPECT_EQ(m[1], vec4f(5.0f, 6.0f, 7.0f, 8.0f));
  EXPECT_EQ(m[2], vec4f(9.0f, 10.0f, 11.0f, 12.0f));
  EXPECT_EQ(m[3], vec4f(13.0f, 14.0f, 15.0f, 16.0f));
}

TEST(mat4f, compare) {
  mat4f a(vec4f(1.0f, 2.0f, 3.0f, 4.0f), vec4f(5.0f, 6.0f, 7.0f, 8.0f),
          vec4f(9.0f, 10.0f, 11.0f, 12.0f), vec4f(13.0f, 14.0f, 15.0f, 16.0f));
  mat4f b = a;
  mat4f c = mat4f::identity();

  EXPECT_EQ(a, a);
  EXPECT_EQ(a, b);
  EXPECT_EQ(b, a);
  EXPECT_NE(a, c);
  EXPECT_NE(c, b);
}

TEST(mat4f, identity_identity_multiplication) {
  EXPECT_EQ(mat4f::identity() * mat4f::identity(), mat4f::identity());
}

TEST(mat4f, multiplication) {

  mat4f a(vec4f(4.37f, 9.56f, 7.59f, 6.39f), vec4f(2.40f, 2.40f, 1.52f, 8.80f),
          vec4f(6.41f, 7.37f, 1.19f, 9.73f), vec4f(8.49f, 2.91f, 2.64f, 2.65f));

  mat4f b(vec4f(3.74f, 5.72f, 4.89f, 3.62f), vec4f(6.51f, 2.26f, 3.63f, 4.30f),
          vec4f(5.10f, 8.07f, 2.80f, 5.63f), vec4f(6.33f, 1.42f, 6.47f, 2.53f));

  mat4f res(vec4f(157.7371f, 116.9271f, 118.66739f, 115.825806f),
            vec4f(88.056f, 43.9144f, 81.64f, 49.829597f),
            vec4f(139.612f, 76.741295f, 124.383095f, 86.2118f),
            vec4f(80.9352f, 80.2072f, 76.6169f, 64.8145f));

  EXPECT_EQ(a * b, res);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
