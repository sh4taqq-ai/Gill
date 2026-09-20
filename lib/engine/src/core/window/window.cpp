#include "core/window/window.hpp"
#include <cassert>
#include <iostream>
#include "core/debug/error.hpp"
Window::Window(unsigned int width, unsigned int height,const std::string &title) {
    if (!glfwInit()) {
        std::cerr << "glfwInit() failed" << std::endl;
        glfwTerminate();
        return;
    }

    p_wp = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!p_wp) {
        std::cerr << "glfwCreateWindow() failed" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(p_wp);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "gladLoadGL() failed" << std::endl;
    }
    while (glGetError() != GL_NO_ERROR) {}




}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(p_wp);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(p_wp);
}

void Window::PollEvents() {
    glfwPollEvents();
}

 GLFWwindow *Window::GetWindow() {
    return p_wp;
}

void Window::GetCursorPos(double &x, double &y) {
    glfwGetCursorPos(p_wp, &x, &y);
}

int Window::GetLeftMouseButton() const {
    return static_cast<bool>(glfwGetMouseButton(p_wp, GLFW_MOUSE_BUTTON_LEFT));
}

Window::~Window() {
    glfwDestroyWindow(p_wp);
}


void Window::GetFrameBufferSize(int *width, int *height) {
    glfwGetFramebufferSize(p_wp,width,height);
}
