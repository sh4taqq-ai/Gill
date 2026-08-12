#ifndef MATHLIBS_VEC4_H
#define MATHLIBS_VEC4_H

#ifdef __cplusplus
extern "C" {
#endif
typedef struct vec4 {
    float x, y, z, w;
}vec4;

static inline vec4 vec4_create(float x, float y, float z,float w) {
    return (vec4){ x, y, z, w};
}

vec4 vec4_add(vec4 a, vec4 b);
vec4 vec4_sub(vec4 a, vec4 b);
vec4 vec4_mul(vec4 a, vec4 b);
vec4 vec4_scale(vec4 a, float s);

float vec4_dot(vec4 a, vec4 b);
vec4 vec4_normalize(vec4 a);
float vec4_length(vec4 a);
float vec4_length_sq(vec4 a);

#ifdef __cplusplus
}
#endif
#endif //MATHLIBS_VEC4_H