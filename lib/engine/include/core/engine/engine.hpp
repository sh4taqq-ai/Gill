#pragma once
#include <string>
#include <memory>
#include "../window/window.hpp"
#include "../../scene/scene.hpp"
#include "../../render/core/renderer.hpp"
#include "../include/render/shader/shader.hpp"
#include "../include/editor/selector/selector.hpp"
#include "../include/editor/ui/ui.hpp"
#include "../input/input.hpp"
#include "../include/core/component/camera/camera.hpp"
#include "../include/editor/grid/grid.hpp"
#include "../include/editor/gizmo/gizmo.hpp"



class Engine {

public:
    void Init(unsigned int width, unsigned int height, const std::string& title);
    void Run();
    void Shutdown();

private:
    std::unique_ptr<Window> window;
    float aspect;
    float deltaTime;
    UIManager uiManager;
    double xPos,yPos;
    unsigned int wdth,hght;
    mathpp::mat4f projection;
    Selector selector;
    Renderer renderer;
    Scene scene;
    TransformComponent tComp;
    MeshComponent meComp;
    Input input;
    FreeCamera freecam;
    GridRenderer gridRenderer;
    Gizmo gizmo;

};