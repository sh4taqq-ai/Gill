#pragma once
#include "imgui.h"
#include "component/entity.hpp"
#include "render/mesh/primitive.hpp"
#include "editor/registry/registry.hpp"
#include "core/system/mesh/mesh.hpp"
#include "core/system/material/material.hpp"
#include "uiComponent/mesh.hpp"
#include <vector>
#include "scene/scene.hpp "




class EditorPanel {
public:
    virtual ~EditorPanel() = default;
    virtual void Draw() = 0;
    std::string m_name;
};

class Window;
class Hierarchy;
class Gizmo;
class TransformSystem;
class Renderer;
class Input;
class EditorInputMap;
class SelectionManager;
struct GizmoData;

struct UIContext {
    GizmoData* p_gizmoData;
    TransformSystem* p_transformSystem;
    Renderer* p_renderer;
    MeshSystem* p_meshSystem;
    MaterialSystem* p_materialSystem;
    EditorInputMap* p_editorInputMap;
    SelectionManager* p_selectionManager;
    Scene* p_scene;
};





class PropertiesPanel : public EditorPanel {
public:
    PropertiesPanel(Scene* scene, TransformSystem* transformSystem,SelectionManager* selectionManager,EditorContext ctx)
        : p_scene(scene), p_transformSystem(transformSystem),p_selectionManager(selectionManager),ctx(ctx) {}
    void Draw() override;
private:
    Scene* p_scene;
    SelectionManager* p_selectionManager;
    TransformSystem* p_transformSystem;
    EditorContext ctx;
    static inline const ComponentTypeInfo ComponentTypes[] = {
        { "Mesh",     AddMesh,    RemoveMesh,DrawMeshInspector,HasMesh },
    };
};

class HierarchyPanel : public EditorPanel {
    public:
    HierarchyPanel(Scene* scene, Hierarchy* hierarchy, TransformSystem* transformSystem) : p_scene(scene), p_hierarchy(hierarchy), p_transformSystem(transformSystem) {}
    void Draw() override;
private:
    void DrawEntityNode(Entity entity);
    Scene* p_scene;
    Hierarchy* p_hierarchy;
    TransformSystem* p_transformSystem;
    SelectionManager* p_selectionManager;
};

class GizmoPanel : public EditorPanel {
public:
    GizmoPanel(GizmoData* gizmoData) : p_gizmoData(gizmoData) {}

    void Draw() override;

private:
    GizmoData* p_gizmoData;
};

struct PrimitiveData {
    Entity entity = UINT32_MAX;
    PrimitiveType type;
    int rings = 32;
    int segments = 16;
    float radius = 1.0f;
    float height = 1.0f;
};

class UIManager {
public:
    void Init(Window* window,Scene* scene,TransformSystem* transformSystem, Hierarchy* hierarchy,GizmoData* gizmoData,Renderer* renderer,MeshSystem* meshSystem,MaterialSystem* materialSystem,EditorInputMap* editorInputMap,SelectionManager* selectionManager);
    void RenderPanels();
    void RenderAddMenu(Scene* scene);
    void RenderPrimitiveOp(Scene* scene);
    void RenderViewportMode();
    void BeginFrame();
    void EndFrame();
    bool WantCaptureMouse();
    void Shutdown();

private:
    void DrawDockspace(Scene* scene);
    void AdjustLastOp(Scene* scene);
    EditorInputMap* p_editorInputMap;
    PrimitiveData m_primitiveData;
    Renderer* p_renderer;
    MeshSystem* p_meshSystem;
    TransformSystem* p_transformSystem;
    SelectionManager* p_selectionManager;
    std::vector<std::unique_ptr<EditorPanel>> v_panels;
    void AddPrimitive(Scene* scene, PrimitiveType type);
    void AddSunlight(Scene* scene);
    ImGuiIO* p_io_ptr;



};