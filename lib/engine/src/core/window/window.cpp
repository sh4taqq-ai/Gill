#include "../include/core/window/window.hpp"
#include <cassert>
#include <iostream>
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

Window::~Window() {
    glfwDestroyWindow(wp);
}


