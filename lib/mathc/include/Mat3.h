#ifndef MATHLIBS_MAT3_H
#define MATHLIBS_MAT3_H

typedef struct  {
    float m[3][3]; // m[col][row]
}mat3;

mat3 mat3_mul(mat3 a, mat3 b);
mat3 mat3_add(mat3 a, mat3 b);
mat3 mat3_sub(mat3 a, mat3 b);
mat3 mat3_identity(void);
mat3 mat3_transpose(mat3 a);
mat3 mat3_inverse(mat3 a);
float mat3_det(mat3 a);


#endif