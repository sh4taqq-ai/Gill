#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

inline void CheckGLError(const char* label) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL Error: 0x%X at %s\n", err, label);
    }
}