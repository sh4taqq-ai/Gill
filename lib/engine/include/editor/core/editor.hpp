#pragma once
#include <memory>
#include "mathpp.hpp"
#include "../include/editor/gizmo/gizmo.hpp"

class Input;
class UIManager;
class GridRenderer;
class Window;
class Scene;
class Selector;
class Camera;


class Editor {
public:
    Editor();
    void Init(float width, float height,Window* window,Scene* scene,const mathpp::mat4f& projection,Camera* camera);
    void Run(float deltaT);
    void ShutDown();
    ~Editor();

private:
    float wdth,hght;

    float deltaTime;
    std::unique_ptr<Gizmo> gizmo;
    std::unique_ptr<GridRenderer> gridRenderer;
    std::unique_ptr<Input> input;
    std::unique_ptr<UIManager> ui;
    std::unique_ptr<Selector> selector;
    Camera* _camera;
    Scene* _scene;
    Window* _window;
    bool isDragging = false;
    GizmoAxis dragAxis = GizmoAxis::None;

    mathpp::mat4f proj;
};