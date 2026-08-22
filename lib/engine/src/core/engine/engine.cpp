
#include "../include/core/engine/engine.hpp"
#include <iostream>
#include <GLFW/glfw3.h>



void Engine::Init(unsigned int width, unsigned int height, const std::string& title) {
    window = std::make_unique<Window>(width, height, title);
    wdth = width;
    hght = height;
    selector.Init(wdth,hght);
    renderer.Init();
    gizmo.Init(wdth,hght);
    gridRenderer.Init(100);
    aspect = static_cast<float>(width) / static_cast<float>(height);
    projection = mathpp::perspective(45.0f,aspect,0.01f,300.0f);
    Shader celShader("asset/shader/npr/cel/celVert.glsl", "asset/shader/npr/cel/celFrag.glsl");
    AssetID celShaderID = scene.LoadShader(std::move(celShader));
    uiManager.Init(window.get(),celShaderID);





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
        gridRenderer.Render(freecam.GetViewMatrix(),projection,freecam.GetPosition());
        input.Update(window.get());

        if (!ImGui::GetIO().WantCaptureMouse) {
            freecam.Update(&input,deltaTime);
            if (window->GetLeftMouseButton()) {
                if (currentTime - lastClickTime >= delay) {
                    window->GetCursorPos(xPos, yPos);
                    int mx = static_cast<int>(xPos);
                    int my = static_cast<int>(yPos);
                    bool hitGizmo = false;
                    if (scene.GetSelected().has_value()) {
                        Entity selected = scene.GetSelected().value();
                        const TransformComponent* t = scene.GetComponent<TransformComponent>(selected);
                        if (t) {
                            gizmo.RenderIDs(freecam.GetViewMatrix(), projection, t->position, freecam.GetPosition());
                            GizmoAxis axis = gizmo.ReadAxisAt(mx, my);
                            if (axis != GizmoAxis::None) {
                                hitGizmo = true;
                            }
                        }
                    }

                    if (!hitGizmo) {
                        scene.SetSelected(selector.ReadEntityAt(mx, my));
                    }

                    lastClickTime = currentTime;
                }
            }
        }


        renderer.renderScene(&scene,freecam.GetViewMatrix(),projection,freecam.GetPosition());
        if (scene.GetSelected().has_value()) {
            gizmo.Render(&scene, freecam.GetViewMatrix(), projection,
                         scene.GetComponent<TransformComponent>(scene.GetSelected().value())->position,
                         freecam.GetPosition());
        }
        uiManager.BeginFrame();
        uiManager.RenderAddMenu(&scene);
        uiManager.RenderProperties(&scene);
        uiManager.EndFrame();
        window->SwapBuffers();
    }
}

void Engine::Shutdown() {
    uiManager.Shutdown();
    glfwTerminate();
}