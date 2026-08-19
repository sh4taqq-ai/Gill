#pragma once
#include "../window/window.hpp"
#include "mathpp.hpp"

class Input {
public:
    void Update(Window* window);
    bool IsKeyDown( int Key) const;
    void GetScroll(mathpp::vec2f& scroll);
    mathpp::vec2f GetMouseDelta() const;
private:
    mathpp::vec2f mouseDelta;
    mathpp::vec2f lastMousePos;
    bool firstUpdate = true;
    Window* window_;
};