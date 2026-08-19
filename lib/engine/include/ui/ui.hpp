#pragma once
#include "../include/core/window/window.hpp"
#include "../include/scene/scene.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class UIManager {
public:
    void Init(Window* window);
    void RenderProperties(Scene* scene);
    void Shutdown();

};