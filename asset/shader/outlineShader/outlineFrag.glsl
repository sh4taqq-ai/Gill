#version 330 core


out vec4 fragColor;

uniform vec3 outlineColor = {0.0, 1.0, 0.251};

void main(){
    fragColor = vec4(outlineColor,1.0);
}