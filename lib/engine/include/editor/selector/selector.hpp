#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <optional>
#include "mathpp.hpp"
#include <cstdint>

using Entity = uint32_t;

class Shader;
class Scene;
class TransformSystem;

struct MeshComponent;


class Selector {
    public:
    Selector() = default;
    void Init(unsigned int width, unsigned int height);
    void RenderScene(const Scene* scene,const mathpp::mat4f& view,const mathpp::mat4f& projection,TransformSystem* transformSystem);
    std::optional<Entity> ReadEntityAt(int x, int y) const;

private:
    void RenderEntityID(const Scene* scene,Entity entity, const MeshComponent& meshComp,const mathpp::mat4f& view, const mathpp::mat4f& projection,TransformSystem* transformSystem);
    unsigned int wdth,hght;
    unsigned int FBO;
    unsigned int Texture;
    unsigned int depthRenderBuffer;
    std::unique_ptr<Shader> selectShader;
};
