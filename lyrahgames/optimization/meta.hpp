#pragma once
#include <concepts>
//
#include <lyrahgames/xstd/meta.hpp>

namespace lyrahgames::optimization {

namespace generic {

using namespace lyrahgames::xstd::generic;

template <typename T>
concept real = true;

template <typename T>
concept problem = true;

template <typename T>
concept box_constrained_problem = true;

template <typename T>
concept differentiable_problem = true;

template <typename T>
concept constrained_problem = true;

}  // namespace generic

}  // namespace lyrahgames::optimization
