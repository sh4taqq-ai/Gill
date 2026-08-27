#include "editor/gizmo/gizmoController.hpp"

GizmoAxis GizmoController::GetActiveAxis() const {
    return _dragAxis;
}

GizmoController::~GizmoController() = default;

void GizmoController::Init(float width, float height) {
    wdth = width;
    hght = height;
}

bool GizmoController::IsDragging() {
    return isDragging;
}

void GizmoController::Begin(const GizmoAxis &dragAxis, const mathpp::vec3f &dragStart, const GizmoState &state) {
    if (isDragging)
    {return;}
    _dragAxis = dragAxis;
    _state = state;
    _dragStartPos = dragStart;
    isDragging = true;
}


void GizmoController::ComputeNDC(float &x, float &y,float mouseX,float mouseY) {
    x =(mouseX/wdth)*2.0f - 1.0f;
    y = 1.0f - (mouseY/hght)*2.0f;
}

bool GizmoController::Continue(const mathpp::mat4f &view, const mathpp::mat4f &proj, float mouseX, float mouseY, mathpp::vec3f &outValue) {
    mathpp::vec3f axisDir;
    float ndcX{};
    float ndcY{};
    ComputeNDC(ndcX,ndcY,mouseX,mouseY);
    Ray ray = ScreenToRay(ndcX,ndcY,view,proj);
    switch (_dragAxis) {
        case GizmoAxis::Z:
            axisDir = mathpp::vec3f(0.0f, 0.0f, 1.0f);
            break;
        case GizmoAxis::X:
            axisDir = mathpp::vec3f(1.0f, 0.0f, 0.0f);
            break;
        case GizmoAxis::Y:
            axisDir = mathpp::vec3f(0.0f, 1.0f, 0.0f);
            break;
        case GizmoAxis::None:
            return false;
            default:
            axisDir = mathpp::vec3f(1.0f, 0.0f, 0.0f);
    }
   mathpp::vec3f w0 = _dragStartPos - ray.origin;
    float b = mathpp::dot(axisDir,ray.direction);
    float d = mathpp::dot(axisDir,w0);
    float e = mathpp::dot(ray.direction,w0);
    float denom = 1 - b*b;
    float t = (b*e - d) / denom;



    if (denom <= minDenom) return false;
    outValue = _dragStartPos + axisDir * t;
    return true;
}


void GizmoController::End() {
    isDragging = false;
    _dragAxis =GizmoAxis::None;
}
