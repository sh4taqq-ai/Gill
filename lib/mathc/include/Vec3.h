#ifndef MATHLIBS_VEC3_H
#define MATHLIBS_VEC3_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;


/* Constructors */
static inline vec3 vec3_create(float x, float y, float z) {
    return (vec3){x, y, z};
}


/* Basic Operations */
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, vec3 b);
vec3 vec3_scale(vec3 v, float s);

/* Vector Math */
float vec3_dot(vec3 a, vec3 b);
vec3 vec3_cross(vec3 a, vec3 b);

float vec3_length(vec3 v);
float vec3_length_sq(vec3 v);

vec3 vec3_normalize(vec3 v);
vec3 vec3_lerp( vec3 a, vec3 b, float t);
float vec3_distance(vec3 a, vec3 b);

#ifdef __cplusplus
}
#endif

#endif