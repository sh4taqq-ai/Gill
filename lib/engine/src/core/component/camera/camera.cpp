#include "core/component/camera/camera.hpp"
#include "core/input/input.hpp"
#include <cmath>

//  FreeCamera

void FreeCamera::Update(Input* input, float deltaTime, const mathpp::vec3f&) {
    mathpp::vec2f delta = input->GetMouseDelta();

    if (input->IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        m_yaw += delta.x * m_sens;
        m_pitch -= delta.y * m_sens;
        m_pitch = mathpp::clamp(m_pitch, -89.9f, 89.9f);
    }

    mathpp::vec3f front;
    front.x = cos(mathpp::to_radians(m_yaw)) * cos(mathpp::to_radians(m_pitch));
    front.y = sin(mathpp::to_radians(m_pitch));
    front.z = sin(mathpp::to_radians(m_yaw)) * cos(mathpp::to_radians(m_pitch));
    m_camFront = mathpp::normalize(front);
    if (input->IsKeyDown(GLFW_KEY_W))
    {m_position += m_camFront * m_speed * deltaTime;}
    if (input->IsKeyDown(GLFW_KEY_S))
    {m_position -= m_camFront * m_speed * deltaTime;}
    if (input->IsKeyDown(GLFW_KEY_A))
    {m_position -= mathpp::normalize(mathpp::cross(m_camFront, {0.0f, 1.0f, 0.0f})) * m_speed * deltaTime;}
    if (input->IsKeyDown(GLFW_KEY_D))
    {m_position += mathpp::normalize(mathpp::cross(m_camFront, {0.0f, 1.0f, 0.0f})) * m_speed * deltaTime;}
    if (input->IsKeyDown(GLFW_KEY_SPACE))
    {m_position -= mathpp::normalize(mathpp::cross(m_camFront, {0.0f, 0.0f, 1.0f})) * m_speed * deltaTime;}
    if (input->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
    {m_position += mathpp::normalize(mathpp::cross(m_camFront, {0.0f, 0.0f, 1.0f})) * m_speed * deltaTime;}

    if (input->IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        input->SetCursorMode(2);
    }
    else {input->SetCursorMode(0);}

    m_viewMatrix = mathpp::look_at(m_position, m_camFront + m_position, {0.0f, 1.0f, 0.0f});
}

mathpp::vec3f FreeCamera::GetPosition() const {
    return m_position;
}

mathpp::mat4f FreeCamera::GetViewMatrix() const {
    return m_viewMatrix;
}


//  OrbitCamera

void OrbitCamera::Update(Input* input, float /*deltaTime*/, const mathpp::vec3f& targetPos) {
    mathpp::vec2f delta = input->GetMouseDelta();

    m_yaw += delta.x * m_sens;
    m_pitch -= delta.y * m_sens;
    m_pitch = mathpp::clamp(m_pitch, -89.9f, 89.9f); // same gimbal-flip guard as FreeCamera

    m_target = targetPos;

    float yawRad = mathpp::to_radians(m_yaw);
    float pitchRad = mathpp::to_radians(m_pitch);

    m_eye = mathpp::vec3f();
    m_eye.x = m_target.x + m_distance * cos(pitchRad) * sin(yawRad);
    m_eye.y = m_target.y + m_distance * sin(pitchRad);
    m_eye.z = m_target.z + m_distance * cos(pitchRad) * cos(yawRad);

    m_viewMatrix = mathpp::look_at(m_eye, m_target, {0.0f, 1.0f, 0.0f});
}

mathpp::vec3f OrbitCamera::GetPosition() const {
    return m_eye;
}

mathpp::mat4f OrbitCamera::GetViewMatrix() const {
    return m_viewMatrix;
}

//  Camera

Camera::Camera() : p_active(&m_freeCam), em_mode(CameraType::Free) {}

void Camera::Update(Input* input, float deltaTime, const mathpp::vec3f& target = {0.0f,0.0f,0.0f}) {
    p_active->Update(input, deltaTime, target);
}

mathpp::vec3f Camera::GetPosition() const {
    return p_active->GetPosition();
}

mathpp::mat4f Camera::GetViewMatrix() const {
    return p_active->GetViewMatrix();
}

void Camera::Switch() {
    if (em_mode == CameraType::Free) {
        em_mode = CameraType::Orbit;
        p_active = &m_orbitCam;
    } else {
        em_mode = CameraType::Free;
        p_active = &m_freeCam;
    }
}
Camera::~Camera() = default;