#include "mat3.h"

mat3 mat3_mul(mat3 a, mat3 b) {
    mat3 result;
    for (int col = 0; col < 3; col++) {
        for (int row = 0; row < 3; row++) {
            float sum = 0.0f;
            for (int k = 0; k < 3; k++) {
                sum += a.m[k][row] * b.m[col][k];
            }
            result.m[col][row] = sum;
        }
    }
    return result;
}

mat3 mat3_add(mat3 a, mat3 b) {
    mat3 result;
    for (int col = 0; col < 3; col++) {
        for (int row = 0; row < 3; row++) {
            float sum = 0.0f;
            sum = a.m[col][row] + b.m[col][row];
            result.m[col][row] = sum;
        }
    }
}

mat3 mat3_sub(mat3 a, mat3 b) {
    mat3 result;
    for (int col = 0; col < 3; col++) {
        for (int row = 0; row < 3; row++) {
            float sum = 0.0f;
            sum = a.m[col][row] - b.m[col][row];
            result.m[col][row] = sum;
        }
    }
}

mat3 mat3_identity(void) {
    mat3 result;
    result.m[0][0] = 1.0f; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f;
    result.m[1][0] = 0.0f; result.m[1][1] = 1.0f; result.m[1][2] = 0.0f;
    result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f;
    return result;
}
mat3 mat3_inverse(mat3 a) {
    mat3 result;
    for (int col = 0; col < 3; col++) {
        for (int row = 0; row < 3; row++) {

        }
    }
}

