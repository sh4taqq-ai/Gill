// engine.cpp
#include "../include/core/engine/engine.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include "../include/render/mesh/primitive.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


void Engine::Init(unsigned int width, unsigned int height, const std::string& title) {
    window = std::make_unique<Window>(width, height, title);
    wdth = width;
    hght = height;
    ImGui::CreateContext();


    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    selector.Init(wdth,hght);
    renderer.Init();
    aspect = static_cast<float>(width) / static_cast<float>(height);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(),true);
    view = mathpp::look_at(mathpp::vec3f{3.0f,3.0f,-3.0f},{0.0f,0.0f,0.0f},{0.0f,1.0f,0.0f});
    projection = mathpp::perspective(45.0f,aspect,0.01f,100.0f);
   Entity entity = scene.CreateEntity();
    mesh = CreateCube();


    shader = std::make_unique<Shader>("asset/shader/simpleShader/simpleVert.glsl","asset/shader/simpleShader/simpleFrag.glsl");
    meComp.shader = shader.get();
    meComp.mesh = mesh.get();
    scene.AddTransform(entity,tComp);
    scene.AddMesh(entity,meComp);





}

void Engine::Run() {
    double lastTime = 0.0f;
    double delay = 0.8f;
    while (!window->ShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window->PollEvents();
        selector.RenderScene(&scene,view,projection);


        if (window->GetLeftMouseButton()) {
            double currentTime = glfwGetTime();
            if (currentTime-lastTime >= delay) {
                window->GetCursorPos(xPos,yPos);
               Entity picked = selector.ReadEntityAt(static_cast<int>(xPos), static_cast<int>(yPos));
                std::cout<<"Picked "<<picked<<std::endl;
                lastTime = currentTime;
            }
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        renderer.renderScene(&scene,view,projection);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window->SwapBuffers();
    }
}

void Engine::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}