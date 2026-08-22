#pragma once
#include <vector>

#include "mat4.hpp"
class Scene;


class Renderer {
    public:
    void Init();
    void renderScene(const Scene* scene,const mathpp::mat4f& view,const mathpp::mat4f& projection,const mathpp::vec3f& viewVec);

private:
};