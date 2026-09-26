#include "editor/gizmo/gizmoController.hpp"
#include "core/component/raycast/raycast.hpp"
#include "core/system/transform/transform.hpp"
#include "editor/selector/selectionManager.hpp"
#include "core/system/hierarchy/hierarchy.hpp"


GizmoAxis GizmoController::GetActiveAxis() const {
    return p_gizmoData->axis;
}

GizmoController::~GizmoController() = default;

void GizmoController::Init(float width, float height,GizmoData* gizmoData,TransformSystem* transformSystem,SelectionManager* selectionManager,Hierarchy* hierarchy) {
    m_width = width;
    m_height = height;
    p_gizmoData = gizmoData;
    p_transformSystem = transformSystem;
    p_selectionManager = selectionManager;
    p_hierarchy = hierarchy;
}

bool GizmoController::IsDragging() {
    return isDragging;
}

void GizmoController::Begin(const mathpp::mat4f& view, const mathpp::mat4f& proj, float mouseX, float mouseY) {
    if (isDragging)
    {return;}
    m_transformAxis = GetAxis();
    auto selected = p_selectionManager->GetAllSelected();
    m_pivotStartPos = ComputeMedianPos(selected);

    for (auto entity : selected) {
        const auto& worldTransform = p_transformSystem->GetWorldTransform(entity);
        TransformSnapshot snapshot;
        snapshot.worldPos = mathpp::TranslateFromMat4(worldTransform);
        snapshot.worldRot = p_transformSystem->GetWorldRotation(entity);
        snapshot.worldScale = mathpp::ScaleFromMat4(worldTransform);
        um_worldTransforms.insert({entity, snapshot});
    }
    if (p_gizmoData->mode == GizmoMode::Rotate) {
        float ndcX{}, ndcY{};
        ComputeNDC(ndcX, ndcY, mouseX, mouseY);
        Ray ray = ScreenToRay(ndcX, ndcY, view, proj);
        float t{};
        if (IntersectPlane(m_transformAxis, m_pivotStartPos, ray, t)) {
            mathpp::vec3f hitPoint = ray.origin + ray.direction * t;
            m_dragStartRadial = hitPoint - m_pivotStartPos;
        }
    } else if (p_gizmoData->mode == GizmoMode::Scale) {
        mathpp::vec3f axisDir = m_transformAxis;
        float ndcX{}, ndcY{};
        ComputeNDC(ndcX, ndcY, mouseX, mouseY);
        Ray ray = ScreenToRay(ndcX, ndcY, view, proj);

        mathpp::vec3f w0 = m_pivotStartPos - ray.origin;
        float b = mathpp::dot(axisDir, ray.direction);
        float d = mathpp::dot(axisDir, w0);
        float e = mathpp::dot(ray.direction, w0);
        float denom = 1 - b * b;
        if (denom > minDenom) {
            m_scaleStartT = (b * e - d) / denom;
        }
    }
    isDragging = true;
}


void GizmoController::ComputeNDC(float &x, float &y,float mouseX,float mouseY) {
    x =(mouseX/m_width)*2.0f - 1.0f;
    y = 1.0f - (mouseY/m_height)*2.0f;
}

bool GizmoController::Apply(const mathpp::mat4f &view, const mathpp::mat4f &proj, float mouseX, float mouseY) {
    switch (p_gizmoData->mode) {
        case GizmoMode::Translate: {
            float t;
            if (!ContinueTranslate(view, proj, mouseX, mouseY, t)) return false;


            for (auto& [entity, snapshot] : um_worldTransforms) {
                mathpp::vec3f entityAxis = GetAxisFor(entity);
                mathpp::vec3f newWorldPos = snapshot.worldPos + entityAxis * t;
                mathpp::mat4f  invParentWorld = mathpp::inverse(p_transformSystem->GetParentWorldTransform(entity));
                mathpp::vec4f newWorldPos4 = {newWorldPos.x, newWorldPos.y, newWorldPos.z, 1.0f};
                mathpp::vec4f localPos4 = invParentWorld * newWorldPos4;
                mathpp::vec3f localPos = localPos4.xyz();
                p_transformSystem->SetPosition(entity, localPos);
            }
            return true;
        }

        case GizmoMode::Rotate: {
            mathpp::quatf deltaQuat;
            if (!ContinueRotate(view, proj, mouseX, mouseY, deltaQuat)) return false;

            for (auto& [entity, snapshot] : um_worldTransforms) {
                mathpp::vec3f newWorldPos;

                mathpp::quatf newWorldRot;

                switch (p_gizmoData->referenceFrame) {
                    case ReferenceFrame::World: {
                        // orbit around the shared multi-select pivot, delta expressed in world axes
                        mathpp::vec3f offset = snapshot.worldPos - m_pivotStartPos;
                        mathpp::vec3f rotatedOffset = mathpp::RotateVector(deltaQuat, offset);
                        newWorldPos = m_pivotStartPos + rotatedOffset;
                        newWorldRot = deltaQuat * snapshot.worldRot; // pre-multiply: rotate in world frame
                        break;
                    }
                    case ReferenceFrame::Local: {
                        // spin each object about its own origin, delta expressed in its own axes
                        newWorldPos = snapshot.worldPos; // origin doesn't move
                        newWorldRot = snapshot.worldRot * deltaQuat; // post-multiply: rotate in local frame
                        break;
                    }
                }

                if (p_hierarchy->HasParent(entity)) {
                    mathpp::quatf parentWorldRot = p_transformSystem->GetParentWorldRotation(entity);
                    mathpp::quatf parentWorldRotInv = mathpp::ConjugateQuat(parentWorldRot);

                    mathpp::mat4f parentWorldMat = p_transformSystem->GetParentWorldTransform(entity);
                    mathpp::vec3f parentWorldPos = mathpp::TranslateFromMat4(parentWorldMat);
                    mathpp::vec3f parentWorldScale = mathpp::ScaleFromMat4(parentWorldMat);

                    mathpp::vec3f rotatedOffset = mathpp::RotateVector(parentWorldRotInv, newWorldPos - parentWorldPos);
                    mathpp::vec3f newLocalPos = rotatedOffset / parentWorldScale; // component-wise divide

                    mathpp::quatf newLocalRot = parentWorldRotInv * newWorldRot; // rotation is unaffected by scale

                    p_transformSystem->SetPosition(entity, newLocalPos);
                    p_transformSystem->SetRotation(entity, newLocalRot);
                }
                else {
                    p_transformSystem->SetPosition(entity,newWorldPos);
                    p_transformSystem->SetRotation(entity,newWorldRot);
                }
            }
            return true;
        }
        case GizmoMode::Scale: {
            mathpp::vec3f deltaScale; // component-wise scale factor from the drag, e.g. (1.5, 1, 1)
            if (!ContinueScale(view, proj, mouseX, mouseY, deltaScale)) return false;

        for (auto& [entity, snapshot] : um_worldTransforms) {
            mathpp::vec3f newWorldPos;
            mathpp::vec3f newWorldScale;

            switch (p_gizmoData->referenceFrame) {
                case ReferenceFrame::World: {
                // reposition relative to shared pivot, scaled along world axes
                mathpp::vec3f offset = snapshot.worldPos - m_pivotStartPos;
                mathpp::vec3f scaledOffset = offset * deltaScale; // component-wise
                newWorldPos = m_pivotStartPos + scaledOffset;
                newWorldScale = snapshot.worldScale * deltaScale; // component-wise
                break;
                }
                case ReferenceFrame::Local: {
                // scale in place along the object's own axes
                newWorldPos = snapshot.worldPos;
                newWorldScale = snapshot.worldScale * deltaScale; // component-wise
                break;
                }
            }

            // parent-relative conversion (position needs full T*R*S inverse;
            // scale conversion is just component-wise divide by parent's world scale)
            if (p_hierarchy->HasParent(entity)) {
                mathpp::quatf parentWorldRot = p_transformSystem->GetParentWorldRotation(entity);
                mathpp::quatf parentWorldRotInv = mathpp::ConjugateQuat(parentWorldRot);

                mathpp::mat4f parentWorldMat = p_transformSystem->GetParentWorldTransform(entity);
                mathpp::vec3f parentWorldPos = mathpp::TranslateFromMat4(parentWorldMat);
                mathpp::vec3f parentWorldScale = mathpp::ScaleFromMat4(parentWorldMat);

                mathpp::vec3f rotatedOffset = mathpp::RotateVector(parentWorldRotInv, newWorldPos - parentWorldPos);
                mathpp::vec3f newLocalPos = rotatedOffset / parentWorldScale;
                mathpp::vec3f newLocalScale = newWorldScale / parentWorldScale;

                p_transformSystem->SetPosition(entity, newLocalPos);
                p_transformSystem->SetScale(entity, newLocalScale);
            } else {
                p_transformSystem->SetPosition(entity, newWorldPos);
                p_transformSystem->SetScale(entity, newWorldScale);
            }
        }
            return true;
        }
    }
    return false;
}


void GizmoController::End() {
    isDragging = false;
    p_gizmoData->axis =GizmoAxis::None;
    um_worldTransforms.clear();
}

void GizmoController::SetMode(const GizmoMode &mode) {
    p_gizmoData->mode = mode;
}

GizmoMode GizmoController::GetMode() {
    return p_gizmoData->mode;
}

bool GizmoController::ContinueTranslate(const mathpp::mat4f &view, const mathpp::mat4f &proj, float mouseX, float mouseY, float& outT) {
    mathpp::vec3f axisDir = m_transformAxis;
    float ndcX{}, ndcY{};
    ComputeNDC(ndcX, ndcY, mouseX, mouseY);
    Ray ray = ScreenToRay(ndcX, ndcY, view, proj);
    mathpp::vec3f w0 = m_pivotStartPos - ray.origin;
    float b = mathpp::dot(axisDir, ray.direction);
    float d = mathpp::dot(axisDir, w0);
    float e = mathpp::dot(ray.direction, w0);
    float denom = 1 - b * b;
    if (denom <= minDenom) return false;
    outT = (b * e - d) / denom;
    return true;
}

bool GizmoController::ContinueScale(const mathpp::mat4f &view, const mathpp::mat4f &proj, float mouseX, float mouseY, mathpp::vec3f &outValue) {
    mathpp::vec3f axisDir = m_transformAxis;
    float ndcX{};
    float ndcY{};
    ComputeNDC(ndcX,ndcY,mouseX,mouseY);
    Ray ray = ScreenToRay(ndcX,ndcY,view,proj);

    mathpp::vec3f w0 = m_pivotStartPos - ray.origin;
    float b = mathpp::dot(axisDir,ray.direction);
    float d = mathpp::dot(axisDir,w0);
    float e = mathpp::dot(ray.direction,w0);
    float denom = 1 - b*b;
    if (denom <= minDenom) return false;
    float t = (b*e - d) / denom;

    if (std::abs(m_scaleStartT) <= minDenom) return false;

    float multiplier = t / m_scaleStartT;
    if (std::abs(multiplier) < 0.001f) {
        multiplier = multiplier < 0 ? -0.001f : 0.001f;
    }

    outValue = mathpp::vec3f(1.0f, 1.0f, 1.0f);
    switch (p_gizmoData->axis) {
        case GizmoAxis::X: outValue.x = multiplier; break;
        case GizmoAxis::Y: outValue.y = multiplier; break;
        case GizmoAxis::Z: outValue.z = multiplier; break;
        default: break;
    }
    return true;
}

bool GizmoController::ContinueRotate(const mathpp::mat4f& view, const mathpp::mat4f& proj, float mouseX, float mouseY, mathpp::quatf& outDelta) {

    mathpp::vec3f axisDir = m_transformAxis;
    if (p_gizmoData->axis == GizmoAxis::None) return false;

    float ndcX{}, ndcY{};
    ComputeNDC(ndcX, ndcY, mouseX, mouseY);
    Ray ray = ScreenToRay(ndcX, ndcY, view, proj);

    float t{};
    if (!IntersectPlane(axisDir, m_pivotStartPos, ray, t)) return false;
    mathpp::vec3f hitPoint = ray.origin + ray.direction * t;
    mathpp::vec3f currentRadial = hitPoint - m_pivotStartPos;

    float dotVal = mathpp::dot(m_dragStartRadial, currentRadial);
    mathpp::vec3f crossVal = mathpp::cross(m_dragStartRadial, currentRadial);
    float deltaTheta = atan2(mathpp::dot(crossVal, axisDir), dotVal);
    float halfAngle = deltaTheta/2.0f;
    float cosH = cosf(halfAngle);
    float sinH = sinf(halfAngle);

    outDelta = {cosH, m_transformAxis.x * sinH, m_transformAxis.y * sinH, m_transformAxis.z * sinH};
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
    switch (p_gizmoData->axis) {
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
    if (p_gizmoData->referenceFrame == ReferenceFrame::Local) {
        axisDir = mathpp::RotateVector(p_transformSystem->GetWorldRotation(p_selectionManager->GetActiveSelected().value()),axisDir);
    }
    return axisDir;
}

mathpp::vec3f GizmoController::ComputeMedianPos(const std::unordered_set<Entity>& selected) {
    mathpp::vec3f sum;
    float size = static_cast<float>(selected.size());
    for (auto entity : selected) {
        mathpp::mat4f worldTransform = p_transformSystem->GetWorldTransform(entity);
        mathpp::vec3f worldPos = mathpp::TranslateFromMat4(worldTransform);
        sum += worldPos;
    }
    return sum/size;
}

mathpp::vec3f GizmoController::GetAxisFor(Entity entity) {
    mathpp::vec3f axisDir;
    switch (p_gizmoData->axis) {
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
    if (p_gizmoData->referenceFrame == ReferenceFrame::Local) {
        axisDir = mathpp::RotateVector(p_transformSystem->GetWorldRotation(entity),axisDir);

    }
    return axisDir;
}

void GizmoController::Cancel() {
    if (!isDragging) return;

    for (auto& [entity, snapshot] : um_worldTransforms) {
        mathpp::mat4f invParentWorld = mathpp::inverse(p_transformSystem->GetParentWorldTransform(entity));

        mathpp::vec4f worldPos4 = {snapshot.worldPos.x, snapshot.worldPos.y, snapshot.worldPos.z, 1.0f};
        mathpp::vec3f localPos = (invParentWorld * worldPos4).xyz();

        mathpp::quatf parentWorldRot = p_transformSystem->GetParentWorldRotation(entity);
        mathpp::quatf localRot = mathpp::ConjugateQuat(parentWorldRot) * snapshot.worldRot;

        p_transformSystem->SetPosition(entity, localPos);
        p_transformSystem->SetRotation(entity, localRot);
        // SetScale too, once Scale is wired
    }

    End();
}