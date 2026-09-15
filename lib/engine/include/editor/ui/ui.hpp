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
    std::string name;
};

class Window;
class Hierarchy;
class Gizmo;
class TransformSystem;
class Renderer;
class Input;
struct GizmoData;




class PropertiesPanel : public EditorPanel {
public:
    PropertiesPanel(Scene* scene, TransformSystem* transformSystem,EditorContext ctx)
        : scene(scene), transformSystem(transformSystem),ctx(ctx) {}
    void Draw() override;
private:
    Scene* scene;
    TransformSystem* transformSystem;
    EditorContext ctx;
    static inline const ComponentTypeInfo ComponentTypes[] = {
        { "Mesh",     AddMesh,    RemoveMesh,DrawMeshInspector,HasMesh },
    };
};

class HierarchyPanel : public EditorPanel {
    public:
    HierarchyPanel(Scene* scene, Hierarchy* hierarchy) : scene(scene), hierarchy(hierarchy) {}
    void Draw() override;
private:
    void DrawEntityNode(Entity entity);
    Scene* scene;
    Hierarchy* hierarchy;
};

class GizmoPanel : public EditorPanel {
public:
    GizmoPanel(GizmoData* gizmoData) : _gizmoData(gizmoData) {}

    void Draw() override;

private:
    GizmoData* _gizmoData;
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
    void Init(Window* window,Scene* scene,TransformSystem* transformSystem, Hierarchy* hierarchy,GizmoData* gizmoData,Renderer* renderer,MeshSystem* meshSystem,MaterialSystem* materialSystem);
    void RenderPanels();
    void RenderAddMenu(Scene* scene);
    void RenderPrimitiveOp(Scene* scene);
    void RenderViewportMode(Input* input);
    void BeginFrame();
    void EndFrame();
    bool WantCaptureMouse();
    void Shutdown();

private:
    void DrawDockspace(Scene* scene);
    void AdjustLastOp(Scene* scene);
    PrimitiveData primitiveData;
    Renderer* _renderer;
    MeshSystem* _meshSystem;
    TransformSystem* _transformSystem;
    std::vector<std::unique_ptr<EditorPanel>> panels;
    void AddPrimitive(Scene* scene, PrimitiveType type);
    void AddSunlight(Scene* scene);
    ImGuiIO* io_ptr;



};