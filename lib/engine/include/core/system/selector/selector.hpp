#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "../include/render/shader/shader.hpp"
#include "scene/scene.hpp"

class Selector {
    public:
    void Init(unsigned int width, unsigned int height);
    void RenderScene(Scene* scene,const mathpp::mat4f& view,const mathpp::mat4f& projection);
    std::optional<Entity> ReadEntityAt(int x, int y) const;
private:
    unsigned int wdth,hght;
    unsigned int FBO;
    unsigned int Texture;
    unsigned int depthRenderBuffer;
    std::unique_ptr<Shader> selectShader;
};
