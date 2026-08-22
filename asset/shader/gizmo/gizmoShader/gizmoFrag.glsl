#version 330 core

uniform vec3 axisColor;
out vec4 fragColor;

void main()
{
    fragColor = vec4(axisColor,1.0);
}