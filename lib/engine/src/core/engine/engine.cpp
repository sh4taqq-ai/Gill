#include "core/engine/engine.hpp"
#include <iostream>
#include <GLFW/glfw3.h>

#include "core/component/camera/camera.hpp"


void Engine::Init(unsigned int width, unsigned int height,Camera* camera,Scene* scene) {
    _cam = camera;
    _scene = scene;
    renderer.Init();

}

void Engine::Run() {
    renderer.renderScene(_scene,_cam->GetViewMatrix(),proj,_cam->GetPosition());
}

void Engine::Shutdown() {
    glfwTerminate();
}




Engine::~Engine() = default;
