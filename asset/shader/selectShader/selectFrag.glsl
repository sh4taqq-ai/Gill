#version 330

out int fragColor;

uniform int ObjectID;
void main() {
    fragColor = ObjectID;
}
