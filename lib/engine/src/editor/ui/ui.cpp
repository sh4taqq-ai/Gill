#include "editor/ui/ui.hpp"
#include "core/window/window.hpp"
#include "scene/scene.hpp"
#include "core/system/hierarchy/hierarchy.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"








void UIManager::Init(Window* window,uint32_t shaderID) {
    ImGui::CreateContext();
    io_ptr = &ImGui::GetIO();
    io_ptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io_ptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(),true);
    defaultShaderID = shaderID;
}

void UIManager::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::RenderProperties(Scene* scene) {
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Once);
    ImGui::SetNextWindowPos(
    ImVec2(1,60 ),
    ImGuiCond_Always
);
    ImGui::Begin("Properties",nullptr,ImGuiWindowFlags_NoMove);
    if (scene->GetSelected().has_value()) {
        ImGui::Text("Entity: %u", scene->GetSelected().value());
        ImGui::NewLine();
         TransformComponent* transform = scene->TryGetComponent<TransformComponent>(scene->GetSelected().value());
        if (transform != nullptr) {
            ImGui::InputFloat3(
           "Position",
           &transform->position.x);

            ImGui::InputFloat3(
                "Rotation",
                &transform->rotation.x);

            ImGui::InputFloat3(
                "Scale",
                &transform->scale.x);
        }

        SunlightComponent* sunlight = scene->TryGetComponent<SunlightComponent>(scene->GetSelected().value());
        if (sunlight!=nullptr) {
            ImGui::ColorEdit3("Sun Color", &sunlight->color.x);
            ImGui::DragFloat("Intensity", &sunlight->intensity, 0.05f, 0.0f, 10.0f);
        }
    }
    ImGui::End();
}

void UIManager::AddPrimitive(Scene* scene, PrimitiveType type) {
    Mesh mesh = [&] {
        switch (type) {
            case PrimitiveType::Cube:   return CreateCube();
            case PrimitiveType::Sphere: return CreateSphere(15, 15, 2.0f);
            case PrimitiveType::Plane:  return CreatePlane();
            case PrimitiveType::Cylinder: return CreateCylinder(15,2.0f,6.0f);
            case PrimitiveType::Cone : return CreateCone(15,1.0f,1.0f);
        }
    }();

    Entity entity = scene->CreateEntity();
    AssetID meshID = scene->LoadMesh(std::move(mesh));

    MeshComponent meComp;
    meComp.meshID = meshID;
    meComp.shaderID = defaultShaderID;

    TransformComponent tComp;

    scene->InsertComponent(entity, tComp);
    scene->InsertComponent(entity, meComp);
}

void UIManager::RenderAddMenu(Scene* scene) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Add")) {
            if (ImGui::MenuItem("Cube"))   AddPrimitive(scene, PrimitiveType::Cube);
            if (ImGui::MenuItem("Sphere")) AddPrimitive(scene, PrimitiveType::Sphere);
            if (ImGui::MenuItem("Plane"))  AddPrimitive(scene, PrimitiveType::Plane);
            if (ImGui::MenuItem("Cylinder")) AddPrimitive(scene, PrimitiveType::Cylinder);
            if (ImGui::MenuItem("Cone")) AddPrimitive(scene, PrimitiveType::Cone);
            if (ImGui::MenuItem("Sun Light")) AddSunlight(scene);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


void UIManager::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::AddSunlight(Scene* scene) {
    Mesh mesh = CreateSphere(8, 8, 0.3f);

    Entity entity = scene->CreateEntity();
    AssetID meshID = scene->LoadMesh(std::move(mesh));

    MeshComponent meComp;
    meComp.meshID = meshID;
    meComp.shaderID = defaultShaderID;

    TransformComponent tComp;

    SunlightComponent sunComp;


    scene->InsertComponent(entity, tComp);
    scene->InsertComponent(entity, meComp);
    scene->InsertComponent(entity, sunComp);
}

bool UIManager::WantCaptureMouse() {
    return io_ptr->WantCaptureMouse==true;
}

void UIManager::RenderHierarchy(Scene *scene,Hierarchy* hierarchy) {
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once); // reasonable on-screen default, once
    std::vector<Entity> liveEntities = scene->GetLivingEntities();
   if (ImGui::Begin("Hierarchy")) {
       for (uint32_t entity : liveEntities) {
           std::optional<Entity> parent = hierarchy->TryGetParent(entity);
           if (!parent.has_value()) {
               DrawEntityNode(scene,entity,hierarchy);
           }
       }
   }

    ImGui::End();
}

void UIManager::DrawEntityNode(Scene *scene, Entity entity, Hierarchy *hierarchy) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (scene->GetSelected().has_value() && scene->GetSelected().value() == entity) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    std::string title = "Entity " + std::to_string(entity);
    std::vector children = hierarchy->GetChild(entity);
    bool isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entity, flags, "%s", title.c_str());
    if (ImGui::IsItemClicked()) {
        scene->SetSelected(entity);
    }
    if (isNodeOpen) {

        for (uint32_t i = 0; i < children.size(); i++) {
            DrawEntityNode(scene,children[i],hierarchy);
        }

        ImGui::TreePop();
    }
}
