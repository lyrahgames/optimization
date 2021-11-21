#pragma once
#include <lyrahgames/optimization/meta.hpp>
#include <lyrahgames/optimization/utility.hpp>

namespace lyrahgames::optimization::gallery {

namespace detail {
template <generic::real T>
struct rastrigin {
  using real = T;
  static constexpr real a = 10;

  static constexpr auto loop(auto x) noexcept {
    constexpr auto pi = std::numbers::pi_v<real>;
    return pow<2>(x) - a * cos(2 * pi * x);
  }

  static constexpr auto loop(auto x, auto... tail) noexcept {
    return loop(x) + loop(tail...);
  }

  static constexpr auto value(auto... x) noexcept {
    return a * sizeof...(x) + loop(x...);
  }

  static constexpr auto value(const auto& args) noexcept {
    auto x = begin(args);
    auto result = loop(x);
    ++x;
    for (; x != end(args); ++x) result += loop(x);
    return result + a * size(args);
  }
};
}  // namespace detail

template <generic::real T, size_t N>
requires(N >= 2)  //
    struct static_rastrigin_problem {
  using detail = detail::rastrigin<T>;
  using real = typename detail::real;

  static constexpr auto parameter_count() noexcept { return N; }
  static constexpr auto box_min(size_t i) noexcept -> real { return -5.12; }
  static constexpr auto box_max(size_t i) noexcept -> real { return 5.12; }

  constexpr auto operator()(const auto& args) const noexcept {
    assert(size(args) == parameter_count());
    return detail::value(args);
  }

  constexpr auto operator()(auto... x) const noexcept
      requires(sizeof...(x) == N) {
    return detail::value(x...);
  }
};

template <generic::real T, size_t N = 2>
static_rastrigin_problem<T, N> static_rastrigin{};

}  // namespace lyrahgames::optimization::gallery
