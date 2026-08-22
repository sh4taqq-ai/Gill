#pragma once
#include <cstdint>
#include "../render/mesh/mesh.hpp"
#include "../render/shader/shader.hpp"

using AssetID = uint32_t;

struct MeshComponent {
 AssetID meshID;
 AssetID shaderID;
};