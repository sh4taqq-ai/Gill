#pragma once
#include "mathpp.hpp"
#include "../include/core/window/window.hpp"
#include "../include/core/input/input.hpp"

class OrbitCamera {
public:
    void Update(Input* input,const mathpp::vec3f& target);
    [[nodiscard]] mathpp::mat4f GetViewMatrix() const;
    private:
    mathpp::mat4f viewMatrix;
    mathpp::vec3f target = {0.0f,0.0f,0.0f};
    float distance = 5.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float sens = 1.0f;
};

class FreeCamera {
    public:
    void Update(Input* input,float deltaTime);
    [[nodiscard]] mathpp::mat4f GetViewMatrix() const;
    private:
    mathpp::mat4f viewMatrix;
    mathpp:: vec3f position = {0.0f,0.0f,-4.0f};
    float speed = 20.0f;
    float sens = 1.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;

};