#pragma once
#include "constants.hpp"
namespace mathpp {
    template <typename T>
    T pow(const T& x, const T& n) {
        T ret = x;
        for (int i = 0; i < (n-1); i++) {
            ret *= x;
        }
        return ret;
    }
    template <typename T>
    T to_radians(const T& degrees) {
        T result;
        result = degrees *  (PI<T> / T(180));
        return result;
    }

    template<typename T>
    T to_degrees(const T& radians) {
        T result;
        result = radians * (180 / PI<T>);
        return result;
    }

    template <typename T>
    T clamp(T value,T minVal,T maxVal) {
        if (value < minVal) {
            return minVal;
        }
        if (value > maxVal) {
            return maxVal;
        }
        return value;
    }

}