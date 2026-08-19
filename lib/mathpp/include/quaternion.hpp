

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
        q.x =  normAxis.x * (rad/static_cast<T>(2));
        q.y =  normAxis.y * (rad/static_cast<T>(2));
        q.z = normAxis.z * (rad/static_cast<T>(2));
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


}