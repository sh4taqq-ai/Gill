#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <optional>
#include "mathpp.hpp"
#include <cstdint>
#include "component/mesh.hpp"

using Entity = uint32_t;

class Shader;
class Scene;
class TransformSystem;
class MeshSystem;

class Selector {
    public:
    Selector() = default;
    void Init(unsigned int width, unsigned int height);
    void RenderScene(const Scene* scene,const mathpp::mat4f& view,const mathpp::mat4f& projection,TransformSystem* transformSystem,MeshSystem* meshSystem);
    std::optional<Entity> ReadEntityAt(int x, int y) const;

private:
    void RenderEntityID(const Scene* scene,Entity entity, const comp::MeshComponent& meshComp,const mathpp::mat4f& view, const mathpp::mat4f& projection,TransformSystem* transformSystem,MeshSystem* meshSystem);
    unsigned int m_width,m_height;
    unsigned int m_FBO;
    unsigned int m_Texture;
    unsigned int m_depthRenderBuffer;
    std::unique_ptr<Shader> up_selectShader;
};
