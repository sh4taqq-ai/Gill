#pragma once
#include "vec3.hpp"
#include "vec2.hpp"

namespace mathpp {
    template <typename T>
    struct vec4 {
        T x, y, z, w;

        vec4() : x(0), y(0), z(0), w(0) {}
        vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        T& operator[](int i) {
            switch (i) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                default: return w;
            }
        }

        const T& operator[](int i) const {
            switch (i) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                default: return w;
            }
        }
        vec4& operator+=(const vec4& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        vec4& operator-=(const vec4& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }

        vec3<T> xyz() const {
            return vec3<T>{x, y, z};
        }
        vec2<T> xy() const {
            return vec2<T>{x, y};
        }
    };

    template <typename T>
    vec4<T> operator+(const vec4<T>& lhs, const vec4<T>& rhs) {
        return vec4(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z, lhs.w+rhs.w);
    }
    template <typename T>
    vec4<T> operator-(const vec4<T>& lhs, const vec4<T>& rhs) {
        return vec4(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z, lhs.w-rhs.w);
    }
    template <typename T>
    vec4<T> operator*(const vec4<T>& lhs, const T &s){return vec4(lhs.x*s,lhs.y*s,lhs.z*s,lhs.w*s); }
    template <typename T>
    vec4<T> operator*(const T&s, const vec4<T>& rhs){return vec4(s*rhs.x,s*rhs.y,s*rhs.z,s*rhs.w); }

    using vec4i = vec4<int>;
    using vec4f = vec4<float>;
    using vec4d = vec4<double>;

}