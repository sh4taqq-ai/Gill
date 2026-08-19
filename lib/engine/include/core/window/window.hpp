#pragma once
#include <memory>
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
     GLFWwindow* GetWindow();
    void GetCursorPos(double&x,double&y);
    int GetLeftMouseButton() const;
    private:
    GLFWwindow* wp;
};



