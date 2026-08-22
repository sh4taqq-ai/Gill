#pragma once
#include "mathpp.hpp"
#include <memory>
#include "../include/render/mesh/primitive.hpp"
#include "../include/render/shader/shader.hpp"

class GridRenderer {
public:
    void Init(float extent = 100.0f); // builds the quad mesh + compiles grid shader once
    void Render(const mathpp::mat4f& view, const mathpp::mat4f& projection, const mathpp::vec3f& camPos);

private:
    std::unique_ptr<Mesh> gridMesh;
    std::unique_ptr<Shader> gridShader;
    float cellSize = 1.0f;
    float ext;
};