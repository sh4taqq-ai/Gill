#pragma once


enum class GizmoAxis : int {
    None = 0,
    X = 1,
    Y = 2,
    Z = 3
};

enum class GizmoState : int {
    Translate = 0,
    Rotate = 1,
    Scale = 2
};

enum class GizmoPlane : int {
    None = 0,
    XZ = 1,
    YX = 2,
    ZY = 3
};
