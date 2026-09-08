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

    wp = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!wp) {
        std::cerr << "glfwCreateWindow() failed" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(wp);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "gladLoadGL() failed" << std::endl;
    }
    while (glGetError() != GL_NO_ERROR) {}




}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(wp);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(wp);
}

void Window::PollEvents() {
    glfwPollEvents();
}

 GLFWwindow *Window::GetWindow() {
    return wp;
}

void Window::GetCursorPos(double &x, double &y) {
    glfwGetCursorPos(wp, &x, &y);
}

int Window::GetLeftMouseButton() const {
    return static_cast<bool>(glfwGetMouseButton(wp, GLFW_MOUSE_BUTTON_LEFT));
}

Window::~Window() {
    glfwDestroyWindow(wp);
}


void Window::GetFrameBufferSize(int *width, int *height) {
    glfwGetFramebufferSize(wp,width,height);
}
