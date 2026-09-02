#pragma once
#include "mathpp.hpp"
struct TransformComponent {
    mathpp::vec3f position;
    mathpp::quatf rotation;
    mathpp::vec3f scale;


    mathpp::mat4f getMatrix() const {
        mathpp::mat4f aMatrix;
       aMatrix = mathpp::translate(aMatrix, position);
       aMatrix = aMatrix * mathpp::QuatToMat4(rotation);
       aMatrix = mathpp::scale(aMatrix,scale);
        return aMatrix;
    }

    TransformComponent() : position {0.0f,0.0f,0.0f}, rotation(), scale {1.0f,1.0f,1.0f} {}

};

