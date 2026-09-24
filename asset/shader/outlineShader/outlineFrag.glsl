#version 330 core


out vec4 fragColor;

uniform vec3 outlineColor = {0.5f,0.5f,0.0f};

void main(){
    fragColor = vec4(outlineColor,1.0);
}