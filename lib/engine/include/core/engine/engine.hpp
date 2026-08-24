#pragma once
#include <string>
#include <memory>
#include "render/core/renderer.hpp"


class Engine {

public:
    Engine() = default;
    ~Engine() = default;
    void Init(unsigned int width, unsigned int height);
    void Run();
    void Shutdown();

private:
    unsigned int wdth,hght;
    Renderer renderer;


};
