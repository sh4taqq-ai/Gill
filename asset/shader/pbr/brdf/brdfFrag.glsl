#version 330 core

#define PI 3.14159265f

in vec3 normals;
in vec3 vertexColor;

uniform vec3 albedo;
uniform vec3 viewVec;
uniform vec3 lightDir;

uniform float kS = 0.8;



vec3 ComputeHalf(vec3 light, vec3 view){
    return normalize(light + view);
}

float NormalDistribute(float rough, vec3 normal, vec3 halfVec) {
    float roughSq = rough * rough;
    float rough4  = roughSq * roughSq; // Standard GGX uses roughness^4
    float dotNH   = max(dot(normal, halfVec), 0.0f);
    float dotNHSq = dotNH * dotNH;

    float denom = (dotNHSq * (rough4 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return rough4 / denom;
}

float GeometryFactor(vec3 normal,float roughness,vec3 direction){
    float roughSq = roughness * roughness;
    float num = 2.0 * dot(normal,direction);
    float denom = dot(normal,direction) + sqrt(roughSq + (1.0 - roughSq)*pow(dot(normal,direction),2));
    return num/denom;
}

float GeometryTerm(vec3 view, vec3 light, vec3 normal, float rough){
    float result = GeometryFactor(normal, rough, view) * GeometryFactor(normal, rough, light);
    return result;
}

vec3 Schlick_Approx(vec3 view,vec3 halfVec,vec3 F0) {
    float part2 = (1.0f - dot(view,halfVec));
    return F0 + (vec3(1.0) - F0) * pow(part2,5.0);
}

vec3 SpecularBRDF(vec3 fresnel, float distribute, float geometryTerm, vec3 normal, vec3 light,vec3 view) {
    vec3 num = distribute * geometryTerm * fresnel;
    float denom = 4.0 * dot(normal,view) * dot(normal,light);
    return num/denom;
}



vec3 PBRShader(vec3 albedo, vec3 normal, vec3 view, vec3 light, float rough, vec3 F0) {
    float kD = 1.0 - kS;

    vec3 halfVec = ComputeHalf(light, view);
    float distribute = NormalDistribute(rough, normal, halfVec);
    vec3 fresnel = Schlick_Approx(view, halfVec, F0);
    float geoTerm = GeometryTerm(view, light, normal, rough);
    vec3 specular = SpecularBRDF(fresnel, distribute, geoTerm, normal, light, view);

    float NdotL = max(dot(normal, light), 0.0);

    vec3 term1 = kD * albedo / PI;
    vec3 term2 = kS * specular;
    vec3 result = (term1 + term2) * NdotL;
    return result;
}
out vec4 fragColor;
void main() {
    vec3 view = normalize(viewVec);
    vec3 light = normalize(lightDir);
    vec3 normal = normalize(normals);

    float roughness = 0.5; // temp constant, or wire up a uniform later
    vec3 F0 = vec3(0.04);  // typical dielectric F0, or wire up a uniform later

    vec3 color = PBRShader(albedo, normal, view, light, roughness, F0);
    fragColor = vec4(color, 1.0);
}