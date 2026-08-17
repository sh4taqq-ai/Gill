#version 330 core
layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor; // Specify a color output to the fragment shader

void main()
{
    gl_Position = projection * view * model * vec4(position,1.0f);
    vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f); // Set the output variable to a dark-red color
}
