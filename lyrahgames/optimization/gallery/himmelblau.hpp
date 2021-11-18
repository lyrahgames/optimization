#pragma once
#include <lyrahgames/optimization/meta.hpp>
#include <lyrahgames/optimization/utility.hpp>

namespace lyrahgames::optimization::gallery {

template <generic::real T>
struct himmelblau_problem {
  using real = T;

  static constexpr real a = 11;
  static constexpr real b = 7;

  static constexpr auto parameter_count() noexcept -> size_t { return 2; }
  static constexpr auto box_min(size_t i) noexcept -> real { return -5; }
  static constexpr auto box_max(size_t i) noexcept -> real { return 5; }

  constexpr auto operator()(auto x, auto y) const noexcept {
    return pow<2>(pow<2>(x) + y - a) + pow<2>(x + pow<2>(y) - b);
  }

  static constexpr auto gradient_evaluation(const auto& x,
                                            auto& grad) noexcept {
    const auto u = pow<2>(x[0]) + x[1] - a;
    const auto v = x[0] + pow<2>(x[1]) - b;
    grad[0] = 2 * u * x[0] + 2 * v;
    grad[1] = 2 * u + 2 * v * x[1];
    return pow<2>(u) + pow<2>(v);
  }
};

template <generic::real T>
himmelblau_problem<T> himmelblau{};

}  // namespace lyrahgames::optimization::gallery
