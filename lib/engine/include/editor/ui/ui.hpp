#pragma once
#include "imgui.h"
#include "component/entity.hpp"
#include "render/mesh/primitive.hpp"

class Scene;
class Window;
class Hierarchy;
class Gizmo;
class TransformSystem;

struct GizmoData;

class UIManager {
public:
    void Init(Window* window,uint32_t shaderID);
    void BeginFrame();
    void EndFrame();
    void RenderProperties(Scene* scene,TransformSystem* transformSystem);
    void RenderAddMenu(Scene* scene,TransformSystem* transformSystem);
    void RenderHierarchy(Scene* scene,Hierarchy* hierarchy);
    void RenderGizmoControls(GizmoData* gizmoData);
    void RenderButton(Gizmo* gizmo);
    bool WantCaptureMouse();
    void Shutdown();

private:



    uint32_t defaultShaderID;
    void AddPrimitive(Scene* scene, PrimitiveType type,TransformSystem* transformSystem);
    void AddSunlight(Scene* scene);
    void DrawEntityNode(Scene* scene, Entity entity,Hierarchy* hierarchy);
    ImGuiIO* io_ptr;

};