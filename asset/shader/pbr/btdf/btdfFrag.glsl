#version 330 core

#define PI 3.14159265359
in vec3 normals;
uniform vec3 lightDir;
uniform vec3 viewVec;
uniform float IOR;


vec3 ComputeHalf(vec3 light, vec3 view) {
    return normalize(light + view);
}

float NormalDistribute(float rough, vec3 normal, vec3 halfVec) {
    float a = rough * rough;
    float a2 = a * a;
    float NdotH = max(dot(normal, halfVec), 0.0);
    float NdotHSq = NdotH * NdotH;

    float denom = (NdotHSq * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return a2 / max(denom, 0.0001);
}

vec3 Schlick_Approx(float VdotH, vec3 F0) {
    return F0 + (vec3(1.0) - F0) * pow(clamp(1.0 - VdotH, 0.0, 1.0), 5.0);
}

float SmithMasking(vec3 normal,float roughness){
    float denom = pow(dot(normal,viewVec),2.0);
    float num = roughness * roughness(1.0-pow(dot(normal,viewVec),2));
    float calc = -1.0 + sqrtf(1.0 + num/denom);
    return calc/2.0;
}




out vec4 fragColor;

void main() {

}
