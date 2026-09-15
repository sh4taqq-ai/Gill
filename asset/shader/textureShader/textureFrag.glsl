#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D meshTexture;

void main(){
    fragColor = texture(meshTexture,texCoord);
}
