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
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once);
    ImGui::SetNextWindowPos(
    ImVec2(-100,100 ),
    ImGuiCond_Always
);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Properties",nullptr,ImGuiWindowFlags_NoMove);
    if (scene->GetSelected().has_value()) {
        ImGui::Text("Entity: %u", scene->GetSelected());
        ImGui::NewLine();
         TransformComponent* transform = scene->GetComponent<TransformComponent>(scene->GetSelected().value());
        ImGui::InputFloat3(
       "Position",
       const_cast<float *>(&transform->position.x)
   );

        ImGui::InputFloat3(
            "Rotation",
            const_cast<float *>(&transform->rotation.x)
        );

        ImGui::InputFloat3(
            "Scale",
            const_cast<float *>(&transform->scale.x)
        );
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
