#include "core/component/camera/camera.hpp"
#include "core/input/input.hpp"
#include <cmath>

//  FreeCamera

void FreeCamera::Update(Input* input, float deltaTime, const mathpp::vec3f& /*target*/) {
    mathpp::vec2f delta = input->GetMouseDelta();

    if (input->IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        yaw += delta.x * sens;
        pitch -= delta.y * sens;
        pitch = mathpp::clamp(pitch, -89.9f, 89.9f);
    }

    mathpp::vec3f front;
    front.x = cos(mathpp::to_radians(yaw)) * cos(mathpp::to_radians(pitch));
    front.y = sin(mathpp::to_radians(pitch));
    front.z = sin(mathpp::to_radians(yaw)) * cos(mathpp::to_radians(pitch));
    camFront = mathpp::normalize(front);

    if (input->IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        input->SetCursorMode(2);

        if (input->IsKeyDown(GLFW_KEY_W))
            position += camFront * speed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_S))
            position -= camFront * speed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_A))
            position -= mathpp::normalize(mathpp::cross(camFront, {0.0f, 1.0f, 0.0f})) * speed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_D))
            position += mathpp::normalize(mathpp::cross(camFront, {0.0f, 1.0f, 0.0f})) * speed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_SPACE))
            position -= mathpp::normalize(mathpp::cross(camFront, {0.0f, 0.0f, 1.0f})) * speed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
            position += mathpp::normalize(mathpp::cross(camFront, {0.0f, 0.0f, 1.0f})) * speed * deltaTime;
    }
    else {input->SetCursorMode(0);}

    viewMatrix = mathpp::look_at(position, camFront + position, {0.0f, 1.0f, 0.0f});
}

mathpp::vec3f FreeCamera::GetPosition() const {
    return position;
}

mathpp::mat4f FreeCamera::GetViewMatrix() const {
    return viewMatrix;
}


//  OrbitCamera

void OrbitCamera::Update(Input* input, float /*deltaTime*/, const mathpp::vec3f& targetPos) {
    mathpp::vec2f delta = input->GetMouseDelta();

    yaw += delta.x * sens;
    pitch -= delta.y * sens;
    pitch = mathpp::clamp(pitch, -89.9f, 89.9f); // same gimbal-flip guard as FreeCamera

    target = targetPos;

    float yawRad = mathpp::to_radians(yaw);
    float pitchRad = mathpp::to_radians(pitch);

    eye = mathpp::vec3f();
    eye.x = target.x + distance * cos(pitchRad) * sin(yawRad);
    eye.y = target.y + distance * sin(pitchRad);
    eye.z = target.z + distance * cos(pitchRad) * cos(yawRad);

    viewMatrix = mathpp::look_at(eye, target, {0.0f, 1.0f, 0.0f});
}

mathpp::vec3f OrbitCamera::GetPosition() const {
    return eye;
}

mathpp::mat4f OrbitCamera::GetViewMatrix() const {
    return viewMatrix;
}

//  Camera

Camera::Camera() : active(&freeCam), mode(CameraType::Free) {}

void Camera::Update(Input* input, float deltaTime, const mathpp::vec3f& target = {0.0f,0.0f,0.0f}) {
    active->Update(input, deltaTime, target);
}

mathpp::vec3f Camera::GetPosition() const {
    return active->GetPosition();
}

mathpp::mat4f Camera::GetViewMatrix() const {
    return active->GetViewMatrix();
}

void Camera::Switch() {
    if (mode == CameraType::Free) {
        mode = CameraType::Orbit;
        active = &orbitCam;
    } else {
        mode = CameraType::Free;
        active = &freeCam;
    }
}
Camera::~Camera() = default;