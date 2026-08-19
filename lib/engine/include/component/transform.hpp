#pragma once
#include "mathpp.hpp"
struct TransformComponent {
    mathpp::vec3f position;
    mathpp::vec3f rotation;
    mathpp::vec3f scale;

    mathpp::mat4f getMatrix() const {
        mathpp::mat4f aMatrix;
       aMatrix = mathpp::translate(aMatrix, position);
       aMatrix = aMatrix * mathpp::EulerAnglesRotation(rotation);
       aMatrix = mathpp::scale(aMatrix,scale);
        return aMatrix;
    }

    TransformComponent() : position {0.0f,0.0f,0.0f}, rotation {0.0f,0.0f,0.0f}, scale {1.0f,1.0f,1.0f} {}

};

