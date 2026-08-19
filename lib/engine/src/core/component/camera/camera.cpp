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

void FreeCamera::Update(Input *input,float deltaTime) {
    mathpp::vec2f delta = input->GetMouseDelta();
    yaw += delta.x * sens;
    pitch -= delta.y * sens;



    if (pitch >89.9f)
    {pitch = 89.9f;}
    if (pitch < -89.9f)
    {pitch = -89.9f;}


    mathpp::vec3f front;
    front.x = cos(mathpp::to_radians(yaw)) * cos(mathpp::to_radians(pitch));
    front.y = sin(mathpp::to_radians(pitch));
    front.z = sin(mathpp::to_radians(yaw)) * cos(mathpp::to_radians(pitch));
    mathpp::vec3f cameraFront = mathpp::normalize(front);
    std::cout<<"front.x: "<<cameraFront.x<<"front.y: "<<cameraFront.y<<"front.z: "<<cameraFront.z<<std::endl;

    if (input->IsKeyDown(GLFW_KEY_W)) {
        std::cout<<"Input Read W"<<std::endl;
        position +=  cameraFront * speed * deltaTime;
    }
    if (input->IsKeyDown(GLFW_KEY_S)) {
        std::cout<<"Input Read S"<<std::endl;
        position -=  cameraFront * speed * deltaTime;
    }
    if (input->IsKeyDown(GLFW_KEY_A))
    {position -=  mathpp::normalize(mathpp::cross(cameraFront,{0.0f,1.0f,0.0f})) * speed * deltaTime;}
    if (input->IsKeyDown(GLFW_KEY_D))
    {position +=  mathpp::normalize(mathpp::cross(cameraFront,{0.0f,1.0f,0.0f})) * speed * deltaTime;}
    viewMatrix = mathpp::look_at(position,cameraFront+position,{0.0f,1.0f,0.0f});
    std::cout << position.x << " " << position.y << " " << position.z << std::endl;
}

mathpp::mat4f FreeCamera::GetViewMatrix() const {
    return viewMatrix;
}
