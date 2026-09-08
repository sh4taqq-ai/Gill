#pragma once
#include "assetID.hpp"


/*TODO: Implement the MaterialComponent*/

struct MaterialComponent {
    AssetID shaderID = INVALID_ASSETID;
    AssetID textureID = INVALID_ASSETID;
};
