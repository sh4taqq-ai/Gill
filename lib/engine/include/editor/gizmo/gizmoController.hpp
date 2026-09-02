#pragma once
#include "gizmo.hpp"
#include "mathpp.hpp"
#include "gizmoData.hpp"

class TransformSystem;
using Entity = uint32_t;
struct Ray;

class GizmoController {
public:
    ~GizmoController();
    void Init(float width, float height,GizmoData* gizmoData);
    void Begin(const mathpp::mat4f& view, const mathpp::mat4f& proj, float mouseX, float mouseY,const mathpp::vec3f& dragStart,const mathpp::quatf& dragRotStart, const mathpp::vec3f& dragScaleStart);
    bool Apply(const mathpp::mat4f& view, const mathpp::mat4f& proj,float mouseX, float mouseY,TransformSystem* transformSystem, Entity entity);
    GizmoAxis GetActiveAxis() const;
    bool IsDragging();
    void End();
    GizmoMode GetMode();
    void SetMode(const GizmoMode& mode);

private:
    bool IntersectPlane(const mathpp::vec3f& planeNormal, const mathpp::vec3f& planePoint,const Ray& ray, float& outT) const;
    void ComputeNDC(float &x, float &y,float mouseX,float mouseY);
    mathpp::vec3f GetAxis();
    bool ContinueTranslate(const mathpp::mat4f& view,const mathpp::mat4f& proj,float mouseX,float mouseY,mathpp::vec3f& outValue);
    bool ContinueScale(const mathpp::mat4f& view,const mathpp::mat4f& proj,float mouseX,float mouseY,mathpp::vec3f& outValue);
    bool ContinueRotate(const mathpp::mat4f& view,const mathpp::mat4f& proj,float mouseX,float mouseY,mathpp::quatf& outValue);
    bool isDragging = false;
    GizmoData* _gizmoData;
    float wdth, hght;
    mathpp::vec3f _dragStartPos;
    mathpp::vec3f _dragStartScale;
    mathpp::vec3f _dragStartRadial;
    mathpp::quatf _dragStartRot;
    static constexpr float minDenom = 0.01f;
    static constexpr float sensitivity = 0.4f;
};

