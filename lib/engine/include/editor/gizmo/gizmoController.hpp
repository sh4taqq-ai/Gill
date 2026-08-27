#pragma once
#include "gizmo.hpp"
#include "mathpp.hpp"
#include "gizmoData.hpp"
#include "core/component/raycast/raycast.hpp"

class GizmoController {
public:
    ~GizmoController();
    void Init(float width, float height);
    void Begin(const GizmoAxis& dragAxis,const mathpp::vec3f& dragStart,const GizmoState& state);
    bool Continue(const mathpp::mat4f& view,const mathpp::mat4f& proj,float mouseX,float mouseY,mathpp::vec3f& outValue);
    GizmoAxis GetActiveAxis() const;
    bool IsDragging();
    void End();

private:
    void ComputeNDC(float &x, float &y,float mouseX,float mouseY);
    bool isDragging = false;
    GizmoAxis _dragAxis;
    GizmoState _state = GizmoState::Translate;
    float wdth, hght;
    mathpp::vec3f _dragStartPos;
    static constexpr float minDenom = 0.01f;
};