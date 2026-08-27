#pragma once
#include <string>
#include <memory>
#include "render/core/renderer.hpp"

class Camera;
class Scene;


class Engine {

public:
    Engine(const mathpp::mat4f& projection) : proj(const_cast<mathpp::mat4f&>(projection)) {}
    ~Engine();
    void Init(unsigned int width, unsigned int height,Camera* cam,Scene* scene);
    void Run();
    void Shutdown();

private:
    unsigned int wdth,hght;
    Renderer renderer;
    Camera* _cam;
    Scene* _scene;
    mathpp::mat4f& proj;

};
