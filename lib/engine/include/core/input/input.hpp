#pragma once
#include "mathpp.hpp"
#include  "core/window/window.hpp"
#include "core/component/event/event.hpp"

enum class CursorMode {
    Normal = 0,
    Hidden = 1,
    Disabled = 2
};

class Input {
public:
    Input(Window* window);
    void Update();
    bool IsKeyDown( int Key) const;
    bool IsMouseButtonDown( int Button) const;
    void GetScroll(mathpp::vec2f& scroll);
    void SetCursorMode(int mode);
    void GetCursorPos(mathpp::vec2f& pos);
    ~Input();
    mathpp::vec2f GetMouseDelta() const;
    EventDelegate<int,int> mouseUp;
    EventDelegate<int,int> mouseDown;
private:
    mathpp::vec2f mouseDelta;
    mathpp::vec2f lastMousePos;
    bool firstUpdate = true;
    bool wasHeld = false;
    Window* window_;
};