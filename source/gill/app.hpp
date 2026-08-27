#pragma once
#include <memory>
#include <string>
#include "mathpp.hpp"


class Engine;
class Editor;
class Camera;
class Window;
class Scene;
class Input;

class App {
public:
    App();
    ~App();
    void Init(unsigned int width, unsigned int height);
    void Run();
    void Shutdown();

private:
    float wdth,hght;
    std::string title = "Gill";
    std::unique_ptr<Scene> scene;
    std::unique_ptr<Engine> engine;
    std::unique_ptr<Editor> editor;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Window> window;
    std::unique_ptr<Input> input;
    mathpp::mat4f projection;

    float aspect;
    float deltaTime;
    float lastFrame;

};