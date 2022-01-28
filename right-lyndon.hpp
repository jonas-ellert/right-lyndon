#pragma once

#include "right-lyndon-extension-linear.hpp"

template <std::integral intX = uint32_t>
auto right_lyndon(std::integral auto const* text, size_t const n) {
  return right_lyndon_extension_linear<intX>(text, n);
}
