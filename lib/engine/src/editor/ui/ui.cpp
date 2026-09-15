#include "editor/ui/ui.hpp"
#include "core/window/window.hpp"
#include "scene/scene.hpp"
#include "core/system/hierarchy/hierarchy.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "core/system/transform/transform.hpp"
#include "editor/gizmo/gizmoData.hpp"
#include "render/core/renderer.hpp"
#include "core/input/input.hpp"
#include "core/system/mesh/mesh.hpp"


void UIManager::Init(Window* window,Scene* scene,TransformSystem* transformSystem, Hierarchy* hierarchy,GizmoData* gizmoData,Renderer* renderer,MeshSystem* meshSystem,MaterialSystem* materialSystem) {
    ImGui::CreateContext();
    _renderer = renderer;
    io_ptr = &ImGui::GetIO(); (void)io_ptr;
    io_ptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable; //Enable Docking
    io_ptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io_ptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    _meshSystem = meshSystem;
    _transformSystem = transformSystem;

    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(),true);
    EditorContext ctx{meshSystem,materialSystem};
    panels.emplace_back(std::make_unique<PropertiesPanel>(scene, transformSystem,ctx));
    panels.emplace_back(std::make_unique<HierarchyPanel>(scene, hierarchy));
    panels.emplace_back(std::make_unique<GizmoPanel>(gizmoData));
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


void UIManager::AddPrimitive(Scene* scene, PrimitiveType type) {
   AssetHandle meshID = _meshSystem->AddPrimitive(type,primitiveData.rings,primitiveData.segments,primitiveData.radius,primitiveData.height);
   Entity entity = scene->CreateEntity();
    comp::MeshComponent meshComp;
    meshComp.meshID = meshID;
    primitiveData.entity = entity;
    primitiveData.type = type;
    _transformSystem->AddTransform(entity);
    scene->InsertComponent(entity,meshComp);
}

void UIManager::RenderAddMenu(Scene* scene) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Add")) {
            if (ImGui::MenuItem("Cube"))   AddPrimitive(scene, PrimitiveType::Cube);
            if (ImGui::MenuItem("Sphere")) AddPrimitive(scene, PrimitiveType::Sphere);
            if (ImGui::MenuItem("Plane"))  AddPrimitive(scene, PrimitiveType::Plane);
            if (ImGui::MenuItem("Cylinder")) AddPrimitive(scene, PrimitiveType::Cylinder);
            if (ImGui::MenuItem("Cone")) AddPrimitive(scene, PrimitiveType::Cone);
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


void GizmoPanel::Draw() {
    ImGui::Begin("Gizmo");

    const char* modeLabel = _gizmoData->mode == GizmoMode::Translate ? "Translate"
                           : _gizmoData->mode == GizmoMode::Rotate ? "Rotate" : "Scale";
    if (ImGui::Button(modeLabel, ImVec2{80, 20})) {
        SwitchMode(_gizmoData);
    }

    ImGui::SameLine();

    const char* frameLabel = _gizmoData->referenceFrame == ReferenceFrame::Local ? "Local" : "World";
    if (ImGui::Button(frameLabel, ImVec2{80, 20})) {
        ToggleReferenceFrame(_gizmoData);
    }

    ImGui::End();
}

void PropertiesPanel::Draw(){
    ImGui::Begin("Properties");
    if (!scene->GetSelected().has_value()) { ImGui::End(); return; }
    ImGui::Text("Entity: %u", scene->GetSelected().value());
    ImGui::NewLine();
    Entity entity = scene->GetSelected().value();
    comp::TransformComponent transform = transformSystem->GetTransform(entity); // local copy, read-only source

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
    for (const auto& type : ComponentTypes) {
        if (type.Has(scene, entity)) {
            if (ImGui::CollapsingHeader(type.name)) {
                type.DrawInspector(scene, entity, ctx);
            }
        }
    }
    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("Add Component");
    }
    if (ImGui::BeginPopup("Add Component")) {
        for (const auto& type : ComponentTypes) {
            if (!type.Has(scene, entity)) {
                if (ImGui::Selectable(type.name)) {
                    type.Add(scene, entity, ctx);
                }
            }
        }
        ImGui::EndPopup();
    }


    ImGui::End();
}


void HierarchyPanel::Draw() {
    std::vector<Entity> liveEntities = scene->GetLivingEntities();
    if (ImGui::Begin("Hierarchy")) {
        for (uint32_t entity : liveEntities) {
            std::optional<Entity> parent = hierarchy->TryGetParent(entity);
            if (!parent.has_value()) {
                DrawEntityNode(entity);
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

void HierarchyPanel::DrawEntityNode(Entity entity) {
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
            DrawEntityNode(children[i]);
        }

        ImGui::TreePop();
    }
}




void UIManager::RenderPrimitiveOp(Scene *scene) {
    if (primitiveData.entity == UINT32_MAX)
    {return;}
    if (primitiveData.type != PrimitiveType::Cube && primitiveData.type != PrimitiveType::Plane) {
        ImGui::Begin("Primitive Properties");
        if (ImGui::DragInt("Segments",&primitiveData.segments)) {
            AdjustLastOp(scene);
        }
        if (primitiveData.type == PrimitiveType::Sphere) {
            if (ImGui::DragInt("Rings",&primitiveData.rings)) {
                AdjustLastOp(scene);
            }
        }
        if (primitiveData.type != PrimitiveType::Sphere) {
            if (ImGui::DragFloat("Height",&primitiveData.height)) {
                AdjustLastOp(scene);
            }
        }
        if (ImGui::DragFloat("Radius",&primitiveData.radius)) {
            AdjustLastOp(scene);
        }
        ImGui::End();
    }
}

void UIManager::AdjustLastOp(Scene *scene) {
    comp::MeshComponent* meshComp = scene->TryGetComponent<comp::MeshComponent>(primitiveData.entity);
    if (meshComp == nullptr) return;
    if (meshComp->meshID != AssetHandle{}) {
        _meshSystem->RemoveMesh(meshComp->meshID);
    }

    meshComp->meshID=_meshSystem->AddPrimitive(primitiveData.type,primitiveData.rings,primitiveData.segments,primitiveData.radius,primitiveData.height);
}

void UIManager::DrawDockspace(Scene *scene) {
    return;
}

void UIManager::RenderPanels() {
    ImGui::DockSpaceOverViewport(0,ImGui::GetMainViewport(),ImGuiDockNodeFlags_PassthruCentralNode);
    for (auto& panel : panels) {
        panel->Draw();
    }
}

void UIManager::RenderViewportMode(Input* input) {
    if (input->IsKeyPressed(GLFW_KEY_TAB)) {
        ImGui::OpenPopup("ViewportModePopup");
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
    }

    if (ImGui::BeginPopup("ViewportModePopup")) {
        if (ImGui::MenuItem("Solid"))    _renderer->SetViewportMode(ViewportMode::Solid);
        if (ImGui::MenuItem("Textured")) _renderer->SetViewportMode(ViewportMode::Textured);
        if (ImGui::MenuItem("Rendered")) _renderer->SetViewportMode(ViewportMode::Rendered);
        ImGui::EndPopup();
    }
}
