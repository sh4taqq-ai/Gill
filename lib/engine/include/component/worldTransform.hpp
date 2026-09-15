#pragma once
#include "mathpp.hpp"

namespace comp {
    struct WorldTransformComponent {
        mathpp::mat4f world;
        mathpp::quatf rotation;
        bool dirty = true;
    };
}