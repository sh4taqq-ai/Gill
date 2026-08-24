#pragma once
#include "mathpp.hpp"
#include  "core/window/window.hpp"

class Input {
public:
    Input(Window* window);
    void Update();
    bool IsKeyDown( int Key) const;
    bool IsMouseButtonDown( int Button) const;
    void GetScroll(mathpp::vec2f& scroll);

    mathpp::vec2f GetMouseDelta() const;
private:
    mathpp::vec2f mouseDelta;
    mathpp::vec2f lastMousePos;
    bool firstUpdate = true;
    Window* window_;
};