#include "../include/core/input/input.hpp"
#include <iostream>

void Input::Update(Window *window) {
    window_ = window;
    double currentX,currentY;
    glfwGetCursorPos(window->GetWindow(),&currentX,&currentY);
    if (firstUpdate) {
        lastMousePos  = {static_cast<float>(currentX),static_cast<float>(currentY)};
        firstUpdate = false;
        mouseDelta = {0.0f,0.0f};
        return;
    }

    mouseDelta.x = static_cast<float>(currentX) - lastMousePos.x;
    mouseDelta.y = static_cast<float>(currentY) - lastMousePos.y;
    lastMousePos = {static_cast<float>(currentX),static_cast<float>(currentY)};

}

bool Input::IsKeyDown(int Key) const {
    return glfwGetKey(window_->GetWindow(),Key) == GLFW_PRESS;
}

bool Input::IsMouseButtonDown(int Button) const {
    return glfwGetMouseButton(window_->GetWindow(),Button) == GLFW_PRESS;
}

mathpp::vec2f Input::GetMouseDelta() const {
    return mouseDelta;
}

void Input::GetScroll(mathpp::vec2f &scroll) {

}
