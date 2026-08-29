#include "core/engine/engine.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include "render/core/renderer.hpp"
#include "core/component/camera/camera.hpp"


void Engine::Init(unsigned int width, unsigned int height,Camera* camera,Scene* scene) {
    _cam = camera;
    _scene = scene;
    renderer = std::make_unique<Renderer>();
    renderer->Init();
    wdth = width;
    hght = height;

}

void Engine::Run() {
    renderer->renderScene(_scene,_cam->GetViewMatrix(),proj,_cam->GetPosition());
}

void Engine::Shutdown() {
    glfwTerminate();
}


Engine::Engine(const mathpp::mat4f& projection)
    : proj(const_cast<mathpp::mat4f&>(projection))
{
}

Engine::~Engine() = default;
