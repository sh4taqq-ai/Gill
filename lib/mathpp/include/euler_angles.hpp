#pragma once

#include "mat4.hpp"
#include "mat3.hpp"
#include "vec3.hpp"

namespace mathpp {
    struct EulerAngles {
        mathpp::mat4f Rx;
        mathpp::mat4f Ry;
        mathpp::mat4f Rz;
    };


    template <typename T>
    mathpp::mat4<T> EulerAnglesRotation(const vec3<T> & r) {
        EulerAngles euler;
        T cx = cos(r.x);
        T sx = sin(r.x);
        T cy = cos(r.y);
        T sy = sin(r.y);
        T cz = cos(r.z);
        T sz = sin(r.z);

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