#pragma once

#include <cstdint>

namespace graphics_math::detail {

struct fill_tag {
  explicit constexpr fill_tag(int ignore);

private:
  const int _ignore{};
};

constexpr fill_tag::fill_tag(int ignore) : _ignore{ignore} {}

static constexpr const std::size_t cache_size{64};

static constexpr const std::size_t simd_aligned{sizeof(float) * 4};

}

namespace graphics_math {

static constexpr const detail::fill_tag fill{7};

}
