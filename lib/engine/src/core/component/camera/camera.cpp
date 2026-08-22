#include "../include/core/component/camera/camera.hpp"
#include <iostream>
void OrbitCamera::Update(Input* input, const mathpp::vec3f& targetPos) {
    mathpp::vec2f delta = input->GetMouseDelta();

    yaw += delta.x * sens;
    pitch -= delta.y * sens;


    target = targetPos;

    float yawRad = mathpp::to_radians(yaw);
    float pitchRad = mathpp::to_radians(pitch);

    mathpp::vec3f eye;
    eye.x = target.x + distance * cos(pitchRad) * sin(yawRad);
    eye.y = target.y + distance * sin(pitchRad);
    eye.z = target.z + distance * cos(pitchRad) * cos(yawRad);

    viewMatrix = mathpp::look_at(eye, target, {0.0f, 1.0f, 0.0f});
}

mathpp::mat4f OrbitCamera::GetViewMatrix() const {
    return viewMatrix;
}

mathpp::vec3f OrbitCamera::GetViewTarget() const {
    return target;
}

void FreeCamera::Update(Input *input,float deltaTime) {
    mathpp::vec2f delta = input->GetMouseDelta();
    if (input->IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        yaw += delta.x * sens;
        pitch -= delta.y * sens;
        pitch = mathpp::clamp(pitch,-89.9f,89.9f);
    }


    mathpp::vec3f front;
    front.x = cos(mathpp::to_radians(yaw)) * cos(mathpp::to_radians(pitch));
    front.y = sin(mathpp::to_radians(pitch));
    front.z = sin(mathpp::to_radians(yaw)) * cos(mathpp::to_radians(pitch));
    camFront = mathpp::normalize(front);
    if (input->IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        if (input->IsKeyDown(GLFW_KEY_W)) {
            position +=  camFront * speed * deltaTime;
        }
        if (input->IsKeyDown(GLFW_KEY_S)) {
            position -=  camFront * speed * deltaTime;
        }
        if (input->IsKeyDown(GLFW_KEY_A))
        {position -=  mathpp::normalize(mathpp::cross(camFront,{0.0f,1.0f,0.0f})) * speed * deltaTime;}
        if (input->IsKeyDown(GLFW_KEY_D))
        {position +=  mathpp::normalize(mathpp::cross(camFront,{0.0f,1.0f,0.0f})) * speed * deltaTime;}
    }
    viewMatrix = mathpp::look_at(position,camFront+position,{0.0f,1.0f,0.0f});

}

mathpp::mat4f FreeCamera::GetViewMatrix() const {
    return viewMatrix;
}

mathpp::vec3f FreeCamera::GetPosition() const {
    return position;
}

mathpp::vec3f FreeCamera::GetViewTarget() const {
    return camFront;
}
