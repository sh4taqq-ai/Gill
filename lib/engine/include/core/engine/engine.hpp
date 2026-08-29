#pragma once
#include <string>
#include <memory>
#include "mathpp.hpp"

class Renderer ;
class Camera;
class Scene;


class Engine {

public:
    Engine(const mathpp::mat4f& projection);
    ~Engine();
    void Init(unsigned int width, unsigned int height,Camera* cam,Scene* scene);
    void Run();
    void Shutdown();

private:
    unsigned int wdth,hght;
    std::unique_ptr<Renderer> renderer;
    Camera* _cam;
    Scene* _scene;
    mathpp::mat4f& proj;

};
