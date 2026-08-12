#include "Vec3.h"
#include <math.h>
extern "C";
static const float EPSILON = 1e-6f;


/* Arithmetic */

vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

vec3 vec3_sub( vec3 a, vec3 b) {
    return (vec3){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

vec3 vec3_mul(vec3 a, vec3 b) {
    return (vec3){a.x * b.x,a.y * b.y,a.z * b.z};
}

vec3 vec3_scale( vec3 v, float s) {
    return (vec3){
        v.x * s,
        v.y * s,
        v.z * s
    };
}


/* Vector Math */

float vec3_dot( vec3 a,  vec3 b) {
    return a.x * b.x +
           a.y * b.y +
           a.z * b.z;
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float vec3_length_sq(vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vec3_length(vec3 v) {
    return sqrtf(vec3_length_sq(v));
}

vec3 vec3_normalize(vec3 v) {
    float len = vec3_length(v);

    if (len < EPSILON) {
        return (vec3){0.0f, 0.0f, 0.0f};
    }

    float inv_len = 1.0f / len;

    return (vec3){
        v.x * inv_len,
        v.y * inv_len,
        v.z * inv_len
    };
}

float vec3_distance( vec3 a,  vec3 b) {
    return vec3_length(vec3_sub(a, b));
}

vec3 vec3_lerp( vec3 a,  vec3 b, float t) {
    return (vec3){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
}