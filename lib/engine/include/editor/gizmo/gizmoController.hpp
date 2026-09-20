#pragma once
#include "gizmo.hpp"
#include "mathpp.hpp"
#include "gizmoData.hpp"
#include <unordered_map>
#include <unordered_set>


class TransformSystem;
class SelectionManager;
using Entity = uint32_t;
struct Ray;

class GizmoController {
public:
    ~GizmoController();
    void Init(float width, float height,GizmoData* gizmoData,TransformSystem* transformSystem,SelectionManager* selectionManager);
    void Begin(const mathpp::mat4f& view, const mathpp::mat4f& proj, float mouseX, float mouseY);
    bool Apply(const mathpp::mat4f& view, const mathpp::mat4f& proj,float mouseX, float mouseY);
    GizmoAxis GetActiveAxis() const;
    bool IsDragging();
    void End();
    GizmoMode GetMode();
    void SetMode(const GizmoMode& mode);

private:
    bool IntersectPlane(const mathpp::vec3f& planeNormal, const mathpp::vec3f& planePoint,const Ray& ray, float& outT) const;
    void ComputeNDC(float &x, float &y,float mouseX,float mouseY);
    mathpp::vec3f ComputeMedianPos(const std::unordered_set<Entity>& selected);
    mathpp::vec3f GetAxis();
    mathpp::vec3f GetAxisFor(Entity entity);
    bool ContinueTranslate(const mathpp::mat4f& view,const mathpp::mat4f& proj,float mouseX,float mouseY,float& outT);
    bool ContinueScale(const mathpp::mat4f& view,const mathpp::mat4f& proj,float mouseX,float mouseY,mathpp::vec3f& outValue);
    bool ContinueRotate(const mathpp::mat4f& view,const mathpp::mat4f& proj,float mouseX,float mouseY,mathpp::quatf& outValue);
    bool isDragging = false;
    GizmoData* p_gizmoData;
    TransformSystem* p_transformSystem;
    SelectionManager* p_selectionManager;
    mathpp::vec3f m_pivotStartPos;
    mathpp::vec3f m_dragStartRadial;
    std::unordered_map<Entity,TransformSnapshot> um_worldTransforms;
    float m_width, m_height;
    mathpp::vec3f m_transformAxis;
    static constexpr float minDenom = 0.01f;
    static constexpr float sensitivity = 0.4f;
};


