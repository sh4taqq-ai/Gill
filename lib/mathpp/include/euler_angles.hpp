#pragma once

#include "mat4.hpp"
#include "mat3.hpp"
#include "vec3.hpp"

namespace mathpp {
    struct EulerAngles {
        mat4f Rx;
        mat4f Ry;
        mat4f Rz;
    };


    template <typename T>
    mat4<T> EulerAnglesRotation(const vec3<T> & r) {
        EulerAngles euler;
        T rx = to_radians(r.x);
        T ry = to_radians(r.y);
        T rz = to_radians(r.z);

        T cx = cos(rx);
        T sx = sin(rx);
        T cy = cos(ry);
        T sy = sin(ry);
        T cz = cos(rz);
        T sz = sin(rz);

        euler.Rx.col[0][0] = 1;
        euler.Rx.col[1][0] = 0;
        euler.Rx.col[2][0] = 0;
        euler.Rx.col[0][1] = 0;
        euler.Rx.col[1][1] = cx;
        euler.Rx.col[2][1] = -sx;
        euler.Rx.col[0][2] = 0;
        euler.Rx.col[1][2] = sx;
        euler.Rx.col[2][2] = cx;

        euler.Ry.col[0][0] = cy;
        euler.Ry.col[1][0] = 0;
        euler.Ry.col[2][0] = sy;
        euler.Ry.col[0][1] = 0;
        euler.Ry.col[1][1] = 1;
        euler.Ry.col[2][1] = 0;
        euler.Ry.col[0][2] = -sy;
        euler.Ry.col[1][2] = 0;
        euler.Ry.col[2][2] = cy;

        euler.Rz.col[0][0] = cz;
        euler.Rz.col[1][0] = -sz;
        euler.Rz.col[2][0] = 0;
        euler.Rz.col[0][1] = sz;
        euler.Rz.col[1][1] = cz;
        euler.Rz.col[2][1] = 0;
        euler.Rz.col[0][2] = 0;
        euler.Rz.col[1][2] = 0;
        euler.Rz.col[2][2] = 1;


        return  euler.Ry * euler.Rx * euler.Rz;

    }
}