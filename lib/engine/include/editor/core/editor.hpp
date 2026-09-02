#pragma once
#include <memory>
#include "mathpp.hpp"
#include "core/component/event/event.hpp"
#include <utility>
#include "../include/editor/gizmo/gizmo.hpp"


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


using MouseEvent = EventDelegate<int,int>;

class Editor {
public:
    Editor();
    void Init(float width, float height,Window* window,Scene* scene,const mathpp::mat4f& projection,Camera* camera,TransformSystem* transformSystem,Hierarchy* hierarchy);
    void Run(float deltaT);
    void ShutDown();
    void OnMouseDown(int mx, int my);
    void OnMouseUp(int mx, int my);
    ~Editor();

private:
    float wdth,hght;

    void TrySelect(int mx, int my);
    float deltaTime;
    std::unique_ptr<Gizmo> gizmo;
    std::unique_ptr<GridRenderer> gridRenderer;
    std::unique_ptr<Input> input;
    std::unique_ptr<UIManager> ui;
    std::unique_ptr<Selector> selector;
    std::unique_ptr<GizmoController> gizmoController;
    Hierarchy* _hierarchy;
    Camera* _camera;
    Scene* _scene;
    Window* _window;
    GizmoData gizmoData;
    TransformSystem* _transformSystem;
    std::vector<std::pair<MouseEvent*, MouseEvent::Handle>> _handles;
    mathpp::mat4f proj;
};