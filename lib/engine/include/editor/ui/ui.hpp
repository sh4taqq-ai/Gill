#pragma once
#include "../include/core/window/window.hpp"
#include "../include/scene/scene.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdint>
#include "../include/render/mesh/primitive.hpp"


class UIManager {
public:
    void Init(Window* window,uint32_t shaderID);
    void BeginFrame();
    void EndFrame();
    void RenderProperties(Scene* scene);
    void AddPrimitive(Scene* scene, PrimitiveType type);
    void AddSunlight(Scene* scene);
    void RenderAddMenu(Scene* scene);

    void Shutdown();

private:

    uint32_t defaultShaderID;

};