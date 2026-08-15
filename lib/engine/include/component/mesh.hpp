#pragma once
#include "../render/mesh/mesh.hpp"
#include "../render/shader/shader.hpp"

struct MeshComponent {
 Mesh* mesh;
 Shader* shader;
};