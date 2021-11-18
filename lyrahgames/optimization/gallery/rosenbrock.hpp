#pragma once
#include <array>
#include <cassert>
//
#include <lyrahgames/xstd/math.hpp>

namespace lyrahgames::optimization::gallery {

using xstd::pow;

// Structure for the duck-typed and detailed implementation.
// Should provide no checks at all to be most efficient.
template <std::floating_point T>
struct rosenbrock_detail {
  using real = T;
  static constexpr real a = 1;
  static constexpr real b = 100;

  // Remember, we do not know which type we are actually using for computations.
  // It should be very similar to the 'real' type but may also provide autodiff
  // functionality.
  // But we are allowed to assume that mathematical operations preserve the
  // type. So, std::ranges::range_value_t should also provide the correct return
  // type.
  // We also expect the given types to be trivially copyable.

  // Basic implementation function.
  // Here, every argument can have a differnt type.
  // At compile time, this improve the generated code.
  static constexpr auto value(auto x, auto y) noexcept {
    return pow<2>(a - x) + b * pow<2>(y - pow<2>(x));
  }

  // Here, every argument can have a differnt type.
  // At compile time, this improve the generated code.
  static constexpr auto value(auto x, auto y, auto... tail) noexcept {
    return value(x, y) + value(y, tail...);
  }

  // Every element in the range has to provide the same type.
  // For dynamic behavior, this is most efficient.
  static constexpr auto value(const auto& args) noexcept {
    auto x = begin(args);
    auto y = ++x;
    auto result = value(x, y);
    x = y;
    ++y;
    for (; y != end(args); x = y, ++y) result += value(x, y);
    return result;
  }

  static constexpr auto gradient(const auto& x, auto& grad) noexcept {
    const auto t1 = a - x[0];
    auto t2 = x[1] - sq(x[0]);
    grad[0] = -2 * t1 - 4 * b * t2 * x[0];
    auto value = sq(t1) + b * sq(t2);
    for (size_t i = 1; i < size(x) - 1; ++i) {
      const auto t1 = a - x[i];
      const auto t0 = t2;
      t2 = x[i + 1] - sq(x[i]);
      grad[i] = 2 * b * t0 - 2 * t1 - 4 * b * t2 * x[i];
      value += sq(t1) + b * sq(t2);
    }
    grad[size(x) - 1] = 2 * b * t2;
    return value;
  }
};

template <std::floating_point T, size_t N>
requires(N >= 2)  //
    struct static_rosenbrock_problem : rosenbrock_detail<T> {
  using detail = rosenbrock_detail<T>;
  using real = typename detail::real;
  static constexpr auto parameter_count() noexcept { return N; }
  constexpr auto operator()(const auto& args) const noexcept {
    assert(size(args) == parameter_count());
    return detail::value(args);
  }
  constexpr auto operator()(auto... x) const noexcept
      requires(sizeof...(x) == N) {
    return detail::value(x...);
  }
  static constexpr auto gradient(const auto& x, auto& grad) noexcept {
    assert(size(x) == parameter_count());
    assert(size(grad) == parameter_count());
    return detail::gradient(x, grad);
  }
  static constexpr auto gradient_evaluation(const auto& x,
                                            auto& grad) noexcept {
    assert(size(x) == parameter_count());
    assert(size(grad) == parameter_count());
    return detail::gradient(x, grad);
  }
};

template <std::floating_point T = float, size_t N = 2>
auto static_rosenbrock = static_rosenbrock_problem<T, N>{};

template <std::floating_point T>
struct rosenbrock_problem : rosenbrock_detail<T> {
  using detail = rosenbrock_detail<T>;
  using real = typename detail::real;
  constexpr auto parameter_count() const noexcept { return n; }
  constexpr auto operator()(const auto& args) const noexcept {
    assert(size(args) == parameter_count());
    return detail::value(args);
  }
  constexpr auto gradient(const auto& x, auto& grad) noexcept {
    assert(size(x) == parameter_count());
    assert(size(grad) == parameter_count());
    return detail::gradient(x, grad);
  }
  size_t n;
};

// The dynamic does not induce the count of parameters.
template <std::floating_point T>
struct dynamic_rosenbrock_problem : rosenbrock_detail<T> {
  using detail = rosenbrock_detail<T>;
  using real = typename detail::real;
  constexpr auto operator()(const auto& args) const noexcept {
    assert(size(args) >= 2);
    return detail::value(args);
  }
  static constexpr auto gradient(const auto& x, auto& grad) noexcept {
    assert(size(x) == size(grad));
    return detail::gradient(x, grad);
  }
};

}  // namespace lyrahgames::optimization::gallery
