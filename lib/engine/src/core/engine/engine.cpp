// engine.cpp
#include "../include/core/engine/engine.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include "../include/render/mesh/primitive.hpp"


void Engine::Init(unsigned int width, unsigned int height, const std::string& title) {
    window = std::make_unique<Window>(width, height, title);
    wdth = width;
    hght = height;
    uiManager.Init(window.get());
    selector.Init(wdth,hght);
    renderer.Init();
    aspect = static_cast<float>(width) / static_cast<float>(height);


    projection = mathpp::perspective(45.0f,aspect,0.01f,300.0f);
   Entity entity = scene.CreateEntity();
    mesh = CreateSphere(15,15,2.0f);


    shader = std::make_unique<Shader>("asset/shader/simpleShader/simpleVert.glsl","asset/shader/simpleShader/simpleFrag.glsl");
    meComp.shader = shader.get();
    meComp.mesh = mesh.get();
    scene.AddTransform(entity,tComp);
    scene.AddMesh(entity,meComp);





}

void Engine::Run() {
    double lastTime = 0.0f;
    double lastClickTime = 0.0f;
    double delay = 0.4f;
    while (!window->ShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window->PollEvents();
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        selector.RenderScene(&scene,freecam.GetViewMatrix(),projection);
        input.Update(window.get());


        if (!ImGui::GetIO().WantCaptureMouse) {
            freecam.Update(&input,deltaTime);
            if (window->GetLeftMouseButton() ) {
                if (currentTime - lastClickTime >= delay) {
                    window->GetCursorPos(xPos,yPos);
                    scene.SetSelected( selector.ReadEntityAt(static_cast<int>(xPos),static_cast<int>(yPos)));
                    lastClickTime = currentTime;
                }
            }
        }


        renderer.renderScene(&scene,freecam.GetViewMatrix(),projection);
        uiManager.RenderProperties(&scene);
        window->SwapBuffers();
    }
}

void Engine::Shutdown() {
    uiManager.Shutdown();
    glfwTerminate();
}