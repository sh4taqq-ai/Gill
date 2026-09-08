#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D texture;

void main(){
    fragColor = texture(texture,texCoord);
}
