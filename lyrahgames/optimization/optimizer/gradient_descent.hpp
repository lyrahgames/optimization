#pragma once
#include <vector>
//
#include <lyrahgames/optimization/meta.hpp>

namespace lyrahgames::optimization {

namespace gradient_descent {

template <generic::problem T>
class optimizer {
  using problem_type = T;
  using real = typename problem_type::real;
  using range = std::vector<real>;

  constexpr auto parameter_count() const noexcept {
    return problem.parameter_count();
  }

  constexpr optimizer(problem_type p)
      : problem{p},
        grad0(p.parameter_count()),
        grad1(p.parameter_count()),
        x0(p.parameter_count()),
        x1(p.parameter_count()) {}

  void optimize(
      auto& start,
      auto&& monitor = [](auto& x) {}) {
    constexpr size_t max_iterations = 100;
    constexpr real precision = 1e-5f;
    const auto n = parameter_count();

    for (size_t i = 0; i < n; ++i) x0[i] = start[i];
    value = problem.gradient_evaluation(x0, grad0);

    real step = 0.001;
    real error = 0;
    for (size_t i = 0; i < n; ++i) {
      const auto t = step * grad0[i];
      x1[i] = x0[i] - t;
      error += t * t;
    }
    error = sqrt(error);

    for (size_t it = 0; (it < max_iterations) && (error > precision); ++it) {
      value = problem.gradient_evaluation(x1, grad1);

      // compute step
      step = 0;
      real scale = 0;
      for (size_t i = 0; i < n; ++i) {
        const auto t = grad1[i] - grad0[i];
        step += (x1[i] - x0[i]) * t;
        scale += t * t;
      }
      step /= scale;

      swap(x0, x1);
      swap(grad0, grad1);

      error = 0;
      for (size_t i = 0; i < n; ++i) {
        const auto t = step * grad0[i];
        x1[i] = x0[i] - t;
        error += t * t;
      }
      error = sqrt(error);

      monitor(*this);
    }

    for (size_t i = 0; i < n; ++i) start[i] = x1[i];
  }

  problem_type problem{};
  range grad0;
  range grad1;
  range x0;
  range x1;
  real value;
};

}  // namespace gradient_descent

}  // namespace lyrahgames::optimization
