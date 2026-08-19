#include "../include/ui/ui.hpp"






void UIManager::Init(Window* window) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(),true);
}

void UIManager::RenderProperties(Scene* scene) {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Once);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Properties");
    if (scene->GetSelected().has_value()) {
        ImGui::Text("%", scene->GetSelected());
        ImGui::NewLine();
        const auto& transform = scene->GetComponent<TransformComponent>(scene->GetSelected().value());
        ImGui::Text("Position: %.1f|%.1f|%.1f",transform->position.x,transform->position.y,transform->position.z);
        ImGui::NewLine();
        ImGui::Text("Rotation: %.1f|%.1f|.1f",transform->rotation.x,transform->rotation.y,transform->rotation.z);
        ImGui::NewLine();
        ImGui::Text("Scale: %.1f|%.1f|.1f",transform->scale.x,transform->scale.y,transform->scale.z);

    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
