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
    float m_width{};
    float m_height{};
    std::string title = "Gill";
    std::unique_ptr<Scene> up_scene;
    std::unique_ptr<Engine> up_engine;
    std::unique_ptr<Editor> up_editor;
    std::unique_ptr<Camera> up_camera;
    std::unique_ptr<Window> up_window;
    std::unique_ptr<Input> up_input;
    mathpp::mat4f m_projection;

    float m_aspect;
    float m_deltaTime;
    float m_lastFrame;

};