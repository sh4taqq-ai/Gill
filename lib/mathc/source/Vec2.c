#include "Vec2.h"
#include <math.h>

static const float EPSILON = 1e-6f;

vec2 vec2_add(vec2 a,vec2 b) {
    return (vec2){a.x + b.x, a.y + b.y};
}
vec2 vec2_sub(vec2 a,vec2 b) {
    return (vec2){a.x - b.x, a.y - b.y};
}
vec2 vec2_mul(vec2 a,vec2 b) {
    return (vec2){a.x * b.x, a.y * b.y};
}
vec2 vec2_scale(vec2 v,float s) {
    return (vec2){v.x * s, v.y * s};
}

float vec2_dot(vec2 a,vec2 b) {
    return (a.x * b.x + a.y * b.y);
}

float vec2_length_sq(vec2 v) {
    return (v.x * v.x + v.y * v.y);
}
float vec2_length(vec2 v) {
    return (sqrtf(vec2_length_sq(v)));
}

vec2 vec2_normalize(vec2 v) {
    float len = vec2_length(v);

    if (len<EPSILON)
    {return (vec2){0,0};}

    float inv_len = 1.0f / len;

    return (vec2){v.x * inv_len, v.y * inv_len};
}
float vec2_distance(vec2 a,vec2 b) {
    return vec2_length(vec2_sub(a,b));
}
vec2 vec2_lerp(vec2 a,vec2 b, float t)
{return(vec2){a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t};}
