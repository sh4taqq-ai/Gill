#include "editor/ui/ui.hpp"
#include "core/window/window.hpp"
#include "scene/scene.hpp"
#include "core/system/hierarchy/hierarchy.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "core/system/transform/transform.hpp"
#include "editor/gizmo/gizmoData.hpp"


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

void UIManager::RenderProperties(Scene* scene,TransformSystem* transformSystem) {
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Once);
    ImGui::SetNextWindowPos(
    ImVec2(1,60 ),
    ImGuiCond_Always
);
    ImGui::Begin("Properties",nullptr,ImGuiWindowFlags_NoMove);
    if (scene->GetSelected().has_value()) {
        ImGui::Text("Entity: %u", scene->GetSelected().value());
        ImGui::NewLine();
        Entity entity = scene->GetSelected().value();
        TransformComponent transform = transformSystem->GetTransform(entity); // local copy, read-only source

        mathpp::vec3f pos = transform.position;
        if (ImGui::InputFloat3("Position", &pos.x)) {
            transformSystem->SetPosition(entity, pos);
        }

        mathpp::vec3f euler = mathpp::QuatToEulerAngles(transform.rotation);
        if (ImGui::InputFloat3("Rotation", &euler.x)) {
            transformSystem->SetRotation(entity, mathpp::QuatFromEulerAngles(euler));
        }

        mathpp::vec3f scale = transform.scale;
        if (ImGui::InputFloat3("Scale", &scale.x)) {
            transformSystem->SetScale(entity, scale);
        }

        SunlightComponent* sunlight = scene->TryGetComponent<SunlightComponent>(scene->GetSelected().value());
        if (sunlight!=nullptr) {
            ImGui::ColorEdit3("Sun Color", &sunlight->color.x);
            ImGui::DragFloat("Intensity", &sunlight->intensity, 0.05f, 0.0f, 10.0f);
        }
    }
    ImGui::End();
}

void UIManager::AddPrimitive(Scene* scene, PrimitiveType type,TransformSystem* transformSystem) {
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
    transformSystem->AddTransform(entity);
    scene->InsertComponent(entity, meComp);
}

void UIManager::RenderAddMenu(Scene* scene,TransformSystem* transformSystem) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Add")) {
            if (ImGui::MenuItem("Cube"))   AddPrimitive(scene, PrimitiveType::Cube,transformSystem);
            if (ImGui::MenuItem("Sphere")) AddPrimitive(scene, PrimitiveType::Sphere,transformSystem);
            if (ImGui::MenuItem("Plane"))  AddPrimitive(scene, PrimitiveType::Plane,transformSystem);
            if (ImGui::MenuItem("Cylinder")) AddPrimitive(scene, PrimitiveType::Cylinder,transformSystem);
            if (ImGui::MenuItem("Cone")) AddPrimitive(scene, PrimitiveType::Cone,transformSystem);
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
       ImGui::Spacing();
       ImGui::Separator();
       ImGui::Selectable("##DropToRootTarget", false, ImGuiSelectableFlags_DontClosePopups, ImGui::GetContentRegionAvail());
       if (ImGui::BeginDragDropTarget()) {
           if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG")) {
               Entity draggedEntity = *(Entity*)payload->Data;
               hierarchy->Unparent(draggedEntity);
           }
           ImGui::EndDragDropTarget();
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
    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("ENTITY_DRAG", &entity, sizeof(Entity));
        ImGui::Text("%s", title.c_str());  // optional: shows a small preview label while dragging
        ImGui::EndDragDropSource();
    }


    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
        {
            Entity draggedEntity = *(Entity*)payload->Data;
            hierarchy->SetParent(draggedEntity, entity);
        }
        ImGui::EndDragDropTarget();
    }

    if (isNodeOpen) {

        for (uint32_t i = 0; i < children.size(); i++) {
            DrawEntityNode(scene,children[i],hierarchy);
        }

        ImGui::TreePop();
    }
}

void UIManager::RenderGizmoControls(GizmoData* gizmoData) {
    ImGui::Begin("Gizmo");

    const char* modeLabel = gizmoData->mode == GizmoMode::Translate ? "Translate"
                           : gizmoData->mode == GizmoMode::Rotate ? "Rotate" : "Scale";
    if (ImGui::Button(modeLabel, ImVec2{80, 20})) {
        SwitchMode(gizmoData);
    }

    ImGui::SameLine();

    const char* frameLabel = gizmoData->referenceFrame == ReferenceFrame::Local ? "Local" : "World";
    if (ImGui::Button(frameLabel, ImVec2{80, 20})) {
        ToggleReferenceFrame(gizmoData);
    }

    ImGui::End();
}
