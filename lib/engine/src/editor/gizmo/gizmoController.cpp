#include "editor/gizmo/gizmoController.hpp"
#include "core/component/raycast/raycast.hpp"
#include "core/system/transform/transform.hpp"


GizmoAxis GizmoController::GetActiveAxis() const {
    return _gizmoData->axis;
}

GizmoController::~GizmoController() = default;

void GizmoController::Init(float width, float height,GizmoData* gizmoData) {
    wdth = width;
    hght = height;
    _gizmoData = gizmoData;
}

bool GizmoController::IsDragging() {
    return isDragging;
}

void GizmoController::Begin(const mathpp::mat4f& view, const mathpp::mat4f& proj, float mouseX, float mouseY,const mathpp::vec3f &dragPosStart,const mathpp::quatf& dragRotStart, const mathpp::vec3f& dragScaleStart ) {
    if (isDragging)
    {return;}
    mathpp::vec3f axisDir = GetAxis();

    if (_gizmoData->mode==GizmoMode::Rotate) {
        float ndcX{};
        float ndcY{};
        ComputeNDC(ndcX,ndcY,mouseX,mouseY);
        Ray ray = ScreenToRay(ndcX,ndcY,view,proj);
        float t{};
        if (IntersectPlane(axisDir,dragPosStart,ray,t)) {
            mathpp::vec3f hitPoint = ray.origin + ray.direction * t;
            _dragStartRadial = hitPoint - dragPosStart;
        }
    }
    _dragStartPos = dragPosStart;
    _dragStartScale = dragPosStart;
    isDragging = true;
}


void GizmoController::ComputeNDC(float &x, float &y,float mouseX,float mouseY) {
    x =(mouseX/wdth)*2.0f - 1.0f;
    y = 1.0f - (mouseY/hght)*2.0f;
}

bool GizmoController::Apply(const mathpp::mat4f &view, const mathpp::mat4f &proj, float mouseX, float mouseY, TransformSystem *transformSystem, Entity entity) {
    switch (_gizmoData->mode) {
        case GizmoMode::Translate: {
            mathpp::vec3f pos;
            if (!ContinueTranslate(view, proj, mouseX, mouseY, pos)) return false;
            transformSystem->SetPosition(entity, pos);
            return true;
        }
        case GizmoMode::Rotate: {
            mathpp::quatf rot;
            if (!ContinueRotate(view, proj, mouseX, mouseY, rot)) return false;
            transformSystem->SetRotation(entity, rot);
            return true;
        }
        case GizmoMode::Scale: {
            mathpp::vec3f scale;
            if (!ContinueScale(view, proj, mouseX, mouseY, scale)) return false;
            transformSystem->SetScale(entity, scale);
            return true;
        }
    }
    return false;
}


void GizmoController::End() {
    isDragging = false;
    _gizmoData->axis =GizmoAxis::None;
}

void GizmoController::SetMode(const GizmoMode &mode) {
    _gizmoData->mode = mode;
}

GizmoMode GizmoController::GetMode() {
    return _gizmoData->mode;
}

bool GizmoController::ContinueTranslate(const mathpp::mat4f &view, const mathpp::mat4f &proj, float mouseX, float mouseY, mathpp::vec3f &outValue) {
    mathpp::vec3f axisDir = GetAxis();
    float ndcX{};
    float ndcY{};
    ComputeNDC(ndcX,ndcY,mouseX,mouseY);
    Ray ray = ScreenToRay(ndcX,ndcY,view,proj);
    mathpp::vec3f w0 = _dragStartPos - ray.origin;
    float b = mathpp::dot(axisDir,ray.direction);
    float d = mathpp::dot(axisDir,w0);
    float e = mathpp::dot(ray.direction,w0);
    float denom = 1 - b*b;
    if (denom <= minDenom) return false;
    float t = (b*e - d) / denom;



    outValue = _dragStartPos + axisDir * t;
    return true;
}

bool GizmoController::ContinueScale(const mathpp::mat4f &view, const mathpp::mat4f &proj, float mouseX, float mouseY, mathpp::vec3f &outValue) {
    mathpp::vec3f axisDir = GetAxis();
    float ndcX{};
    float ndcY{};
    ComputeNDC(ndcX,ndcY,mouseX,mouseY);
    Ray ray = ScreenToRay(ndcX,ndcY,view,proj);

    mathpp::vec3f w0 = _dragStartPos - ray.origin;
    float b = mathpp::dot(axisDir,ray.direction);
    float d = mathpp::dot(axisDir,w0);
    float e = mathpp::dot(ray.direction,w0);
    float denom = 1 - b*b;
    if (denom <= minDenom) return false;
    float t = (b*e - d) / denom;
    float multiplier = 1.0f * t * sensitivity;
    if (std::abs(multiplier) < 0.001f) {
        multiplier = multiplier < 0 ? -0.001f : 0.001f;
    }
    outValue = _dragStartScale;
    switch (_gizmoData->axis) {
        case GizmoAxis::X: outValue.x *= multiplier; break;
        case GizmoAxis::Y: outValue.y *= multiplier; break;
        case GizmoAxis::Z: outValue.z *= multiplier; break;
        default: break;
    }
    return true;
}

bool GizmoController::ContinueRotate(const mathpp::mat4f& view, const mathpp::mat4f& proj, float mouseX, float mouseY, mathpp::quatf& outValue) {
    mathpp::vec3f axisDir = GetAxis();
    if (_gizmoData->axis == GizmoAxis::None) return false;/**/

    float ndcX{}, ndcY{};
    ComputeNDC(ndcX, ndcY, mouseX, mouseY);
    Ray ray = ScreenToRay(ndcX, ndcY, view, proj);

    float t{};
    if (!IntersectPlane(axisDir, _dragStartPos, ray, t)) return false;

    mathpp::vec3f hitPoint = ray.origin + ray.direction * t;
    mathpp::vec3f currentRadial = hitPoint - _dragStartPos;

    float dotVal = mathpp::dot(_dragStartRadial, currentRadial);
    mathpp::vec3f crossVal = mathpp::cross(_dragStartRadial, currentRadial);
    float deltaTheta = atan2(mathpp::dot(crossVal, axisDir), dotVal);
    float halfAngle = deltaTheta/2.0f;
    float cosH = cosf(halfAngle);
    float sinH = sinf(halfAngle);
    mathpp::quatf deltaQuat = {cosH,axisDir.x * sinH,axisDir.y * sinH,axisDir.z * sinH};

    outValue = (_gizmoData->referenceFrame == ReferenceFrame::World)
        ? deltaQuat * _dragStartRot
        : _dragStartRot * deltaQuat;

    return true;
}

bool GizmoController::IntersectPlane(const mathpp::vec3f& planeNormal, const mathpp::vec3f& planePoint,const Ray& ray, float& outT) const {
    float denom = mathpp::dot(planeNormal, ray.direction);
    if (std::abs(denom) <= minDenom) return false;
    outT = mathpp::dot(planePoint - ray.origin, planeNormal) / denom;
    return true;
}

mathpp::vec3f GizmoController::GetAxis() {
    mathpp::vec3f axisDir;
    switch (_gizmoData->axis) {
        case GizmoAxis::Z:
             axisDir = mathpp::vec3f(0.0f, 0.0f, 1.0f);
            break;
        case GizmoAxis::X:
            axisDir = mathpp::vec3f(1.0f, 0.0f, 0.0f);
            break;
        case GizmoAxis::Y:
            axisDir = mathpp::vec3f(0.0f, 1.0f, 0.0f);
            break;
        default:
            axisDir = mathpp::vec3f(1.0f, 0.0f, 0.0f);
    }
    return axisDir;
}
