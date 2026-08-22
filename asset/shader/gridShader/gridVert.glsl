#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = aPos;   // already world-space, no model matrix needed
    gl_Position = projection * view * vec4(aPos, 1.0);
}