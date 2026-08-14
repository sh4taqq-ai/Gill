#pragma once
#include "mathpp.hpp"
struct TransformComponent {
    mathpp::vec3f position;
    mathpp::vec3f rotation;
    mathpp::vec3f scale;

    mathpp::mat4<float> getMatrix() const;
};

