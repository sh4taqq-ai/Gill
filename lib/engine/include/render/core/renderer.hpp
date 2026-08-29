#pragma once
#include "mat4.hpp"
#include <cstdint>
class Scene;

using Entity = uint32_t;

struct MeshComponent;

class Renderer {
    public:
    ~Renderer();
    void Init();
    void renderScene(const Scene* scene,const mathpp::mat4f& view,const mathpp::mat4f& projection,const mathpp::vec3f& viewVec);

private:
    void DrawEntity(const Scene* scene, Entity entity, const MeshComponent& meshComp, const mathpp::mat4f& view, const mathpp::mat4f& proj);
};