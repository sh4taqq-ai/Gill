#include "vec3.hpp"
#pragma once

namespace mathpp {
    template <typename T>
    struct quat {
        T w,x,y,z;

        quat() : w(1),x(0),y(0),z(0) {}
        quat(T w, T x, T y,T z): w(w),x(x),y(y),z(z) {}
    };

    template<typename T>
    quat<T> QuatFromAxisAngle(T angle,const  vec3<T>& axis) {
        quat<T> q;
        vec3<T> normAxis = normalize(axis);
        T rad = to_radians(angle);
        q.w = cos(rad/static_cast<T>(2));
        T halfSin = sin(rad / static_cast<T>(2));
        q.x = normAxis.x * halfSin;
        q.y = normAxis.y * halfSin;
        q.z = normAxis.z * halfSin;
        return q;
    }
    template<typename T>
    quat<T> QuatFromEulerAngles(const vec3<T>& euler) {
        quat<T> q;
        T cy = cos(to_radians(euler.y)/static_cast<T>(2));
        T sy = sin(to_radians(euler.y)/static_cast<T>(2));
        T cx = cos(to_radians(euler.x)/static_cast<T>(2));
        T sx = sin(to_radians(euler.x)/static_cast<T>(2));
        T cz = cos(to_radians(euler.z)/static_cast<T>(2));
        T sz = sin(to_radians(euler.z)/static_cast<T>(2));

        q.w = cy * cx * cz + sy * sx * sz;
        q.x = cy * sx * cz + sy * cx * sz;
        q.y = sy * cx * cz - cy * sx * sz;
        q.z = cy * cx * sz - sy * sx * cz;
        return q;
    }
    template<typename T>
    vec3<T> QuatToEulerAngles(const quat<T>& q) {
        vec3<T> r;
        r.x = to_degrees(atan2(T(2)*(q.w*q.x + q.y*q.z), T(1) - T(2)*(q.x*q.x + q.y*q.y)));
        r.z = to_degrees(atan2(T(2)*(q.w*q.z + q.x*q.y), T(1) - T(2)*(q.y*q.y + q.z*q.z)));
        T sinPitch = T(2)*(q.w*q.y - q.x*q.z);
        sinPitch = clamp(sinPitch, T(-1), T(1));
        r.y = to_degrees(asin(sinPitch));
        return r;
    }
    template<typename T>
    quat<T> ConjugateQuat(const quat<T>& q){
        quat<T> r;
        r.w = q.w;
        r.x = -q.x;
        r.y = -q.y;
        r.z = -q.z;
        return r;
    }



    template<typename T>
vec3<T> RotateVector(const quat<T>& q, const vec3<T>& v) {
        vec3<T> qv{q.x, q.y, q.z};
        vec3<T> t = cross(qv, v) * static_cast<T>(2);
        return v + t * q.w + cross(qv, t);
    }

    template<typename T>
    quat<T> operator*(const quat<T>& lhs, const quat<T>& rhs) {
        quat<T> q;
        q.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
        q.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
        q.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
        q.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
        return q;
    }
    template<typename T>
    mat4<T> QuatToMat4(const quat<T>& q) {
        float xx = q.x*q.x, yy = q.y*q.y, zz = q.z*q.z;
        float xy = q.x*q.y, xz = q.x*q.z, yz = q.y*q.z;
        float wx = q.w*q.x, wy = q.w*q.y, wz = q.w*q.z;

        mat4<T> m;
        m.col[0] = vec4<T>(1 - 2*(yy+zz), 2*(xy+wz),     2*(xz-wy),     0.0f);
        m.col[1] = vec4<T>(2*(xy-wz),     1 - 2*(xx+zz), 2*(yz+wx),     0.0f);
        m.col[2] = vec4<T>(2*(xz+wy),     2*(yz-wx),     1 - 2*(xx+yy), 0.0f);
        m.col[3] = vec4<T>(0.0f,          0.0f,          0.0f,         1.0f);
        return m;
    }

    using quatf = quat<float>;
    using quatd = quat<double>;

}