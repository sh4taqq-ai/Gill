// engine.cpp
#include "../include/core/engine/engine.hpp"
#include <GLFW/glfw3.h>


void Engine::Init(unsigned int width, unsigned int height, const std::string& title) {
    window = std::make_unique<Window>(width, height, title);

}

void Engine::Run() {
    while (!window->ShouldClose()) {
        window->PollEvents();

        window->SwapBuffers();
    }
}

void Engine::Shutdown() {
    glfwTerminate();
}