#pragma once
#include <vector>
#include "../component/entity.hpp"
#include "../component/transform.hpp"
#include "../component/mesh.hpp"

class Scene {
public:
    Scene();
    ~Scene();

    private:
    std::vector<Entity> entities;
};