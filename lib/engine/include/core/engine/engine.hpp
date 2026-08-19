#pragma once
#include <string>
#include <memory>
#include "../window/window.hpp"
#include "../../scene/scene.hpp"
#include "../../render/core/renderer.hpp"
#include "../include/render/shader/shader.hpp"
#include "../include/core/system/selector/selector.hpp"
#include "../include/ui/ui.hpp"
#include "../input/input.hpp"
#include "../include/core/component/camera/camera.hpp"



class Engine {

public:
    void Init(unsigned int width, unsigned int height, const std::string& title);
    void Run();
    void Shutdown();

private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Mesh> mesh;
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



};