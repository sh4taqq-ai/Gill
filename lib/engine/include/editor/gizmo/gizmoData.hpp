#pragma once


enum class GizmoAxis : int {
    None = 0,
    X = 1,
    Y = 2,
    Z = 3
};

enum class GizmoMode : int {
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

enum class ReferenceFrame : int {
    Local = 0,
    World = 1
};

struct GizmoData {
    GizmoAxis axis = GizmoAxis::None;
    GizmoPlane plane = GizmoPlane::None;
    ReferenceFrame referenceFrame = ReferenceFrame::Local;
    GizmoMode mode = GizmoMode::Translate;
};

inline void SwitchMode(GizmoData* gizmoData) {

    if (gizmoData->mode == GizmoMode::Translate) {gizmoData->mode = GizmoMode::Rotate; }
    else if (gizmoData->mode == GizmoMode::Rotate) {gizmoData->mode = GizmoMode::Scale; }
    else {gizmoData->mode = GizmoMode::Translate; }
}

inline void ToggleReferenceFrame(GizmoData* gizmoData) {

    gizmoData->referenceFrame  = (gizmoData->referenceFrame == ReferenceFrame::Local) ? ReferenceFrame::World : ReferenceFrame::Local;
}
