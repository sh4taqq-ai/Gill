#pragma once
#include <string>
#include <memory>
#include "../window/window.hpp"

class Engine {

public:
    void Init(unsigned int width, unsigned int height, const std::string& title);
    void Run();
    void Shutdown();

private:
    std::unique_ptr<Window> window;
};