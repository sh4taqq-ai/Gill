#include "vec3.hpp"
#pragma once

namespace mathpp {
    template <typename T>
    struct quat {
        T w,x,y,z;

        quat() : w(1),x(0),y(0),z(0) {}
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


}