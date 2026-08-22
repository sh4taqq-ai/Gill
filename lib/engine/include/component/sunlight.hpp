#pragma once
#include "mathpp.hpp"
struct SunlightComponent {
    mathpp::vec3f color = {1.0f, 1.0f, 1.0f};
    float intensity = 1.0f;

    SunlightComponent() :color(1.0f, 1.0f, 1.0f),intensity(1.0f) {}
};