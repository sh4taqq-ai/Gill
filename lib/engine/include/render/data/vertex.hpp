#pragma once
#include "mathpp.hpp"
#include <optional>

struct Vertex {
    mathpp::vec3f position;
    mathpp::vec3f normal;
    mathpp::vec2f texCoord;
};