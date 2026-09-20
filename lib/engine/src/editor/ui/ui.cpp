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
#include "editor/inputAction/inputAction.hpp"
#include "editor/selector/selectionManager.hpp"


void UIManager::Init(Window* window,Scene* scene,TransformSystem* transformSystem, Hierarchy* hierarchy,GizmoData* gizmoData,Renderer* renderer,MeshSystem* meshSystem,MaterialSystem* materialSystem,EditorInputMap* editorInputMap,SelectionManager* selectionManager) {
    ImGui::CreateContext();
    p_renderer = renderer;
    p_io_ptr = &ImGui::GetIO(); (void)p_io_ptr;
    p_io_ptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable; //Enable Docking
    p_io_ptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    p_io_ptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    p_meshSystem = meshSystem;
    p_transformSystem = transformSystem;
    this->p_editorInputMap = editorInputMap;

    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(),true);
    EditorContext ctx{meshSystem,materialSystem};
    v_panels.emplace_back(std::make_unique<PropertiesPanel>(scene, transformSystem,selectionManager,ctx));
    v_panels.emplace_back(std::make_unique<HierarchyPanel>(scene, hierarchy,transformSystem));
    v_panels.emplace_back(std::make_unique<GizmoPanel>(gizmoData));
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
   AssetHandle meshID = p_meshSystem->AddPrimitive(type,m_primitiveData.rings,m_primitiveData.segments,m_primitiveData.radius,m_primitiveData.height);
   Entity entity = scene->CreateEntity();
    comp::MeshComponent meshComp;
    comp::MaterialComponent matComp;

    meshComp.meshID = meshID;
    m_primitiveData.entity = entity;
    m_primitiveData.type = type;
    p_transformSystem->AddTransform(entity);
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
    return p_io_ptr->WantCaptureMouse==true;
}


void GizmoPanel::Draw() {
    ImGui::Begin("Gizmo");

    const char* modeLabel = p_gizmoData->mode == GizmoMode::Translate ? "Translate"
                           : p_gizmoData->mode == GizmoMode::Rotate ? "Rotate" : "Scale";
    if (ImGui::Button(modeLabel, ImVec2{80, 20})) {
        SwitchMode(p_gizmoData);
    }

    ImGui::SameLine();

    const char* frameLabel = p_gizmoData->referenceFrame == ReferenceFrame::Local ? "Local" : "World";
    if (ImGui::Button(frameLabel, ImVec2{80, 20})) {
        ToggleReferenceFrame(p_gizmoData);
    }

    ImGui::End();
}

void PropertiesPanel::Draw(){
    ImGui::Begin("Properties");
    if (!p_selectionManager->GetActiveSelected().has_value()) { ImGui::End(); return; }
    Entity entity = p_selectionManager->GetActiveSelected().value();
    ImGui::Text("Entity: %u", entity);
    ImGui::NewLine();
    comp::TransformComponent transform = p_transformSystem->GetTransform(entity); // local copy, read-only source

    mathpp::vec3f pos = transform.position;
    if (ImGui::InputFloat3("Position", &pos.x)) {
        p_transformSystem->SetPosition(entity, pos);
    }

    mathpp::vec3f euler = mathpp::QuatToEulerAngles(transform.rotation);
    if (ImGui::InputFloat3("Rotation", &euler.x)) {
        p_transformSystem->SetRotation(entity, mathpp::QuatFromEulerAngles(euler));
    }

    mathpp::vec3f scale = transform.scale;
    if (ImGui::InputFloat3("Scale", &scale.x)) {
        p_transformSystem->SetScale(entity, scale);
    }
    for (const auto& type : ComponentTypes) {
        if (type.Has(p_scene, entity)) {
            if (ImGui::CollapsingHeader(type.name)) {
                type.DrawInspector(p_scene, entity, ctx);
            }
        }
    }
    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("Add Component");
    }
    if (ImGui::BeginPopup("Add Component")) {
        for (const auto& type : ComponentTypes) {
            if (!type.Has(p_scene, entity)) {
                if (ImGui::Selectable(type.name)) {
                    type.Add(p_scene, entity, ctx);
                }
            }
        }
        ImGui::EndPopup();
    }


    ImGui::End();
}


void HierarchyPanel::Draw() {
    std::vector<Entity> liveEntities = p_scene->GetLivingEntities();
    if (ImGui::Begin("Hierarchy")) {
        for (uint32_t entity : liveEntities) {
            std::optional<Entity> parent = p_hierarchy->TryGetParent(entity);
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
                p_hierarchy->Unparent(draggedEntity);
                p_transformSystem->MarkDirty(draggedEntity);
            }
            ImGui::EndDragDropTarget();
        }
    }

    ImGui::End();
}

void HierarchyPanel::DrawEntityNode(Entity entity) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (p_selectionManager->GetActiveSelected().has_value() && p_selectionManager->GetActiveSelected().value() == entity) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    std::string title = "Entity " + std::to_string(entity);
    std::vector children = p_hierarchy->GetChild(entity);
    bool isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entity, flags, "%s", title.c_str());
    if (ImGui::IsItemClicked()) {
        p_scene->SetSelected(entity);
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
            p_hierarchy->SetParent(draggedEntity, entity);
            p_transformSystem->MarkDirty(draggedEntity);
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
    if (m_primitiveData.entity == UINT32_MAX)
    {return;}
    if (m_primitiveData.type != PrimitiveType::Cube && m_primitiveData.type != PrimitiveType::Plane) {
        ImGui::Begin("Primitive Properties");
        if (ImGui::DragInt("Segments",&m_primitiveData.segments)) {
            AdjustLastOp(scene);
        }
        if (m_primitiveData.type == PrimitiveType::Sphere) {
            if (ImGui::DragInt("Rings",&m_primitiveData.rings)) {
                AdjustLastOp(scene);
            }
        }
        if (m_primitiveData.type != PrimitiveType::Sphere) {
            if (ImGui::DragFloat("Height",&m_primitiveData.height)) {
                AdjustLastOp(scene);
            }
        }
        if (ImGui::DragFloat("Radius",&m_primitiveData.radius)) {
            AdjustLastOp(scene);
        }
        ImGui::End();
    }
}

void UIManager::AdjustLastOp(Scene *scene) {
    comp::MeshComponent* meshComp = scene->TryGetComponent<comp::MeshComponent>(m_primitiveData.entity);
    if (meshComp == nullptr) return;
    if (meshComp->meshID != AssetHandle{}) {
        p_meshSystem->RemoveMesh(meshComp->meshID);
    }

    meshComp->meshID=p_meshSystem->AddPrimitive(m_primitiveData.type,m_primitiveData.rings,m_primitiveData.segments,m_primitiveData.radius,m_primitiveData.height);
}

void UIManager::DrawDockspace(Scene *scene) {
    return;
}

void UIManager::RenderPanels() {
    ImGui::DockSpaceOverViewport(0,ImGui::GetMainViewport(),ImGuiDockNodeFlags_PassthruCentralNode);
    for (auto& panel : v_panels) {
        panel->Draw();
    }
}

void UIManager::RenderViewportMode() {
    if (p_editorInputMap->IsActionPressed(EditorAction::ToggleViewportMode)){
        ImGui::OpenPopup("ViewportModePopup");
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
    }

    if (ImGui::BeginPopup("ViewportModePopup")) {
        if (ImGui::MenuItem("Solid"))    p_renderer->SetViewportMode(ViewportMode::Solid);
        if (ImGui::MenuItem("Textured")) p_renderer->SetViewportMode(ViewportMode::Textured);
        if (ImGui::MenuItem("Rendered")) p_renderer->SetViewportMode(ViewportMode::Rendered);
        ImGui::EndPopup();
    }
}
