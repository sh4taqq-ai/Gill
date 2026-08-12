#ifndef MATHLIBS_VEC2_H
#define MATHLIBS_VEC2_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vec2 {
    float x;
    float y;
} vec2;

/* constructor */
static inline vec2 vec2_create(float x, float y) {
    return (vec2){x, y};
}

/* arithmetic */
vec2 vec2_add( vec2 a,  vec2 b);
vec2 vec2_sub( vec2 a, vec2 b);
vec2 vec2_mul( vec2 a, vec2 b);
vec2 vec2_scale(vec2 v, float s);

float vec2_dot(vec2 a, vec2 b);
float vec2_length(vec2 v);
float vec2_length_sq(vec2 v);

vec2 vec2_normalize(vec2 v);
vec2 vec2_lerp(vec2 a, vec2 b, float t);
float vec2_distance(vec2 a,vec2 b);

#ifdef __cplusplus
}
#endif
#endif