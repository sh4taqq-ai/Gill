#version 330

out vec4 fragColor;
in vec3 vNormal;
uniform sampler2D matCap;
void main(){
    vec3 N = normalize(vNormal);
    vec2 matCapUV = N.xy * 0.5 + 0.5 ;
    fragColor = texture(matCap,matCapUV);
}
