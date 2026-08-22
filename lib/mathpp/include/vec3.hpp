#pragma once


namespace mathpp {
    template <typename T>
    struct vec3 {
        T x, y, z;
        vec3(T x, T y, T z) : x(x), y(y), z(z) {}
        vec3() : x(0), y(0), z(0) {}
        T& operator[](int i) {
            switch (i) {
                case 0: return x;
                case 1: return y;
                default: return z;
            }
        }

        const T& operator[](int i) const {
            switch (i) {
                case 0: return x;
                case 1: return y;
                default: return z;
            }
        }
        vec3& operator+=(const vec3& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        vec3& operator-=(const vec3& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
    };

    template <typename T>
    vec3<T> operator+(const vec3<T>& lhs, const vec3<T>& rhs){return vec3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
    template <typename T>
    vec3<T> operator-(const vec3<T>& lhs, const vec3<T>& rhs){return vec3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
    template <typename T>
    vec3<T> operator*(const T s, const vec3<T>& rhs){ return vec3<T>(s*rhs.x, s*rhs.y, s*rhs.z); }
    template <typename T>
    vec3<T> operator*(const vec3<T>& lhs, const T s) { return vec3<T>(lhs.x*s, lhs.y*s, lhs.z*s); }
    template <typename T>
    vec3<T> operator-(const vec3<T>& a) {
        return vec3<T>(-a.x, -a.y, -a.z);
    }
;

    using vec3f = vec3<float>;
    using vec3d = vec3<double>;
    using vec3i = vec3<int>;
};





