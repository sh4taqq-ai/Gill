#pragma once
#include "mathpp.hpp"

class Input; // forward declare — only pointers/references used here

enum class CameraType {
    Free,
    Orbit
};

// Interface
class ICamera {
public:
    virtual ~ICamera() = default;

    virtual void Update(Input* input, float deltaTime, const mathpp::vec3f& target) = 0;
    virtual mathpp::vec3f GetPosition() const = 0;
    virtual mathpp::mat4f GetViewMatrix() const = 0;
};

// Free-fly camera
class FreeCamera : public ICamera {
public:
    void Update(Input* input, float deltaTime, const mathpp::vec3f& target) override;
    mathpp::vec3f GetPosition() const override;
    mathpp::mat4f GetViewMatrix() const override;

private:
    mathpp::vec3f position;
    mathpp::vec3f camFront;
    mathpp::mat4f viewMatrix;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 5.0f;
    float sens = 0.1f;
};

//Orbit camera
class OrbitCamera : public ICamera {
public:
    void Update(Input* input, float deltaTime, const mathpp::vec3f& target) override;
    mathpp::vec3f GetPosition() const override;
    mathpp::mat4f GetViewMatrix() const override;

private:
    mathpp::vec3f eye;
    mathpp::vec3f target;
    mathpp::mat4f viewMatrix;

    float yaw = 0.0f;
    float pitch = 0.0f;
    float distance = 10.0f;
    float sens = 0.1f;
};

// Owner / mode switcher
class Camera {
public:
    Camera();

    void Update(Input* input, float deltaTime, const mathpp::vec3f& target);
    mathpp::vec3f GetPosition() const;
    mathpp::mat4f GetViewMatrix() const;

    void Switch();
    CameraType GetType() const { return mode; }

private:
    FreeCamera freeCam;
    OrbitCamera orbitCam;
    ICamera* active;
    CameraType mode;
};