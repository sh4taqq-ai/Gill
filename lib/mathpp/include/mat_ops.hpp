#pragma once
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat3.hpp"
#include "mat4.hpp"

namespace mathpp {
    template <typename T>
    vec3<T> operator*(const mat3<T>& m, const vec3<T>& v) {
        return vec3<T>(
            m.col[0][0]*v.x + m.col[1][0]*v.y + m.col[2][0]*v.z,
            m.col[0][1]*v.x + m.col[1][1]*v.y + m.col[2][1]*v.z,
            m.col[0][2]*v.x + m.col[1][2]*v.y + m.col[2][2]*v.z
        );
    }
    template <typename T>
    vec4<T> operator*(const mat4<T>& m, const vec4<T>& v) {
        return vec4<T>(
            m.col[0][0]*v.x + m.col[1][0]*v.y + m.col[2][0]*v.z + m.col[3][0]*v.w,
            m.col[0][1]*v.x + m.col[1][1]*v.y + m.col[2][1]*v.z + m.col[3][1]*v.w,
            m.col[0][2]*v.x + m.col[1][2]*v.y + m.col[2][2]*v.z + m.col[3][2]*v.w,
            m.col[0][3]*v.x + m.col[1][3]*v.y + m.col[2][3]*v.z + m.col[3][3]*v.w
        );
    }

    template <typename T>
    mat3<T> normal_matrix(const mat4<T>& model) {
        mat3<T> upper(
            vec3<T>(model.col[0][0], model.col[0][1], model.col[0][2]),
            vec3<T>(model.col[1][0], model.col[1][1], model.col[1][2]),
            vec3<T>(model.col[2][0], model.col[2][1], model.col[2][2])
        );
        return transpose(inverse(upper));
    }
}