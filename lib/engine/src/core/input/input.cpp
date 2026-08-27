#include "core/input/input.hpp"
#include <iostream>

void Input::Update() {
    double currentX,currentY;
    glfwGetCursorPos(window_->GetWindow(),&currentX,&currentY);
    if (firstUpdate) {
        lastMousePos  = {static_cast<float>(currentX),static_cast<float>(currentY)};
        firstUpdate = false;
        mouseDelta = {0.0f,0.0f};
        return;
    }

    mouseDelta.x = static_cast<float>(currentX) - lastMousePos.x;
    mouseDelta.y = static_cast<float>(currentY) - lastMousePos.y;
    lastMousePos = {static_cast<float>(currentX),static_cast<float>(currentY)};
    if (IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !wasHeld) {
        wasHeld = true;
        mouseDown.Fire(static_cast<int>(currentX),static_cast<int>(currentY));
    }
    if (!IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && wasHeld) {
        wasHeld = false;
        mouseUp.Fire(static_cast<int>(currentX),static_cast<int>(currentY));
    }
}

void Input::GetCursorPos(mathpp::vec2f &pos) {
    double x,y;
    window_->GetCursorPos(x,y);
    pos.x = static_cast<float>(x);
    pos.y = static_cast<float>(y);
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

Input::Input(Window* window) {
    window_ = window;
}

Input::~Input() = default;

void Input::SetCursorMode(int mode) {
    CursorMode cursor = static_cast<CursorMode>(mode);
    if (cursor == CursorMode::Normal) {
        glfwSetInputMode(window_->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (cursor == CursorMode::Hidden) {
        glfwSetInputMode(window_->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    if (cursor == CursorMode::Disabled) {
        glfwSetInputMode(window_->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}
