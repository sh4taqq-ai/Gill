#pragma once
#include <string>
#include <memory>
#include "../window/window.hpp"
#include "../include/core/component/raycast/raycast.hpp"
#include "../../scene/scene.hpp"
#include "../../render/core/renderer.hpp"
#include "../include/render/shader/shader.hpp"
#include "../include/core/system/selector/selector.hpp"



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
    Ray ray;

    double xPos,yPos;

    unsigned int wdth,hght;

    mathpp::mat4f view;
    mathpp::mat4f projection;
    Selector selector;
    Renderer renderer;
    Scene scene;
    TransformComponent tComp;
    MeshComponent meComp;


};