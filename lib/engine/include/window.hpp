#pragma once
#include <glad/gl.h>
#include <string>
#include <GLFW/glfw3.h>

class Window {
    public:
    Window(unsigned int width, unsigned int height,const std::string& title);
    ~Window();
    bool ShouldClose();
    void SwapBuffers();
    void PollEvents();
    private:
    GLFWwindow* wp;
};




