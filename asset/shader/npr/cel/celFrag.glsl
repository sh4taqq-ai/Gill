#version 330 core

in vec3 normals;
in vec3 fragPos;

uniform vec3 albedo;
uniform vec3 lightDir;   // already normalized, surface-to-light, from your SunLightComponent
uniform float bands = 3.0;
uniform vec3 lightColor = vec3(1.0);
uniform float lightIntensity = 1.0;


out vec4 fragColor;

void main() {
    vec3 N = normalize(normals);
    vec3 L = normalize(lightDir);

    float NdotL = max(dot(N, L), 0.0);
    float quantized = floor(NdotL * bands) / bands;


    vec3 color = albedo * quantized * lightColor * lightIntensity;
    fragColor = vec4(color, 1.0);
}