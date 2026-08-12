#pragma once

#include <cmath>

namespace mathpp {
    template <typename T>
constexpr T epsilon = T(1e-6);

    template <>
constexpr double epsilon<double> = 1e-12;

    template <typename T>
    constexpr T PI = static_cast<T>(3.14159265358979323846);


}