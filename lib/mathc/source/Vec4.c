#include "Vec4.h"
#include <math.h>

extern "C";
static const float EPSILON = 1e-6f;

vec4 vec4_add(vec4 a, vec4 b) {
    return (vec4){a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};
}
vec4 vec4_sub(vec4 a,vec4 b) {
    return (vec4){a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
}
vec4 vec4_mul(vec4 a, vec4 b) {
    return(vec4){a.x *b.x,a.y*b.y,a.z*b.z,a.w*b.w};
}
vec4 vec4_scale(vec4 v,float s) {
    return(vec4){v.x*s,v.y*s,v.z*s,v.w*s};
}

float vec4_dot(vec4 a,vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float vec4_length_sq(vec4 v) {
    return v.x*v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}
float vec4_length(vec4 v) {
    return sqrtf(vec4_length_sq(v));
}

vec4 vec4_normalize(vec4 v) {
    float len = vec4_length(v);
    if (len<EPSILON) return (vec4){0,0,0,0};
    return (vec4){v.x/len,v.y/len,v.z/len,v.w/len};
}