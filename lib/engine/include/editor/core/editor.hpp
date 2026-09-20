#pragma once
#include <memory>
#include "mathpp.hpp"
#include "core/component/event/event.hpp"
#include <utility>
#include "../include/editor/gizmo/gizmo.hpp"
#include <GLFW/glfw3.h>




class Renderer;
class GizmoController;
class Input;
class UIManager;
class GridRenderer;
class Window;
class Scene;
class Selector;
class Camera;
class TransformSystem;
class Hierarchy;
class MeshSystem;
class MaterialSystem;
class EditorInputMap;
class SelectionManager;

using MouseEvent = EventDelegate<int,int>;



class Editor {
public:
    Editor();
    void Init(float width, float height,Window* window,Scene* scene,const mathpp::mat4f& projection,Camera* camera,TransformSystem* transformSystem,Hierarchy* hierarchy,MeshSystem* meshSystem, MaterialSystem* materialSystem,Renderer* renderer);
    void Run(float deltaT);
    void ShutDown();
    void OnMouseDown(int mx, int my);
    void OnMouseUp(int mx, int my);
    ~Editor();

private:
    float m_width,m_height;

    void TrySelect(int mx, int my);
    mathpp::vec3f ComputeMedianPos();

    std::unique_ptr<Gizmo> up_gizmo;
    std::unique_ptr<GridRenderer> up_gridRenderer;
    std::unique_ptr<Input> up_input;
    std::unique_ptr<UIManager> up_ui;
    std::unique_ptr<Selector> up_selector;
    std::unique_ptr<SelectionManager> up_selectionManager;
    std::unique_ptr<GizmoController> up_gizmoController;
    std::unique_ptr<EditorInputMap> up_editorInputMap;
    Hierarchy* p_hierarchy;
    Camera* p_camera;
    Scene* p_scene;
    Window* p_window;
    GizmoData m_gizmoData;
    MeshSystem* p_meshSystem;
    MaterialSystem* p_materialSystem;
    TransformSystem* p_transformSystem;
    Renderer* p_renderer;
    std::vector<std::pair<MouseEvent*, MouseEvent::Handle>> v_handles;
    mathpp::mat4f m_proj;
};