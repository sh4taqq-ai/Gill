#pragma once
#include "core/system/asset/assetID.hpp"
#include "mathpp.hpp"

struct Material {
    AssetHandle shaderID;
    AssetHandle textureID;
    mathpp::vec3f albedo{1.0f,2.0f,0.0f};
    float metallic{0.2f};
    float roughness{1.0f};

};

namespace comp {



    struct MaterialComponent {
        AssetHandle materialID;
    };
}