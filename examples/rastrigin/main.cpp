#include <iostream>
//
#include <fmt/format.h>
//
#include <lyrahgames/gnuplot/gnuplot.hpp>
//
#include <lyrahgames/optimization/optimization.hpp>

using namespace std;
using namespace fmt;
using namespace lyrahgames;

int main() {
  using real = float;
  const auto problem = optimization::gallery::static_rastrigin<real>;

  gnuplot::temporary_file file{};

  const size_t n = 150;
  const real a = problem.box_min(0);
  const real b = problem.box_max(0);
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      const auto x = (b - a) / (n - 1) * i + a;
      const auto y = (b - a) / (n - 1) * j + a;
      const auto z = problem(x, y);
      file << x << ' ' << y << ' ' << z << '\n';
    }
    file << '\n';
  }
  file.flush();

  gnuplot::pipe plot{};
  plot << format("splot '{}' u 1:2:3 w pm3d\n", file.path().string());
}
