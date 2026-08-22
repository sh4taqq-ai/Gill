#version 330

out int fragColor;

uniform int GizmoAxis;
void main() {
    fragColor = GizmoAxis;
}
