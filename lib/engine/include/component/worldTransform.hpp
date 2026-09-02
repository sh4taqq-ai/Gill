#pragma once
#include "mathpp.hpp"

struct WorldTransformComponent {
    mathpp::mat4f world;
    mathpp::quatf rotation;
    bool dirty = true;
};
