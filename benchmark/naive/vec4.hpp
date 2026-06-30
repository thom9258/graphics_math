#pragma once

namespace naive
{

class vec4f {
public:
  constexpr vec4f() = default;
  constexpr vec4f(float a, float b, float c, float d)
      : data{a, b, c, d} {}

  constexpr auto operator*(const vec4f &other) -> vec4f {
    return {data[0] * other.data[0], data[1] * other.data[1],
            data[2] * other.data[2], data[3] * other.data[3]};
  }

private:
  float data[4];
};

}
