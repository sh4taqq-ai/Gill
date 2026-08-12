#pragma once

namespace mathpp {
    template <typename T>
    struct vec4 {
        T x, y, z, w;

        vec4() : x(0), y(0), z(0), w(0) {}
        vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    };
    template <typename T>
    vec4<T> operator+(const vec4<T>& lhs, const vec4<T>& rhs) {return vec4(lhs.x+rhs.x,lhs.y+rhs.z,lhs.w+rhs.w); }
    template <typename T>
    vec4<T> operator-(const vec4<T> & lhs, const vec4<T>& rhs) {return vec4(lhs.x-rhs.x,lhs.y-rhs.z,lhs.w-rhs.w); }
    template <typename T>
    vec4<T> operator*(const vec4<T>& lhs, const T &s){return vec4(lhs.x*s,lhs.y*s,lhs.z*s,lhs.w*s); }
    template <typename T>
    vec4<T> operator*(const T&s, const vec4<T>& rhs){return vec4(s*rhs.x,s*rhs.y,s*rhs.z,s*rhs.w); }

    using vec4i = vec4<int>;
    using vec4f = vec4<float>;
    using vec3d = vec4<double>;

}