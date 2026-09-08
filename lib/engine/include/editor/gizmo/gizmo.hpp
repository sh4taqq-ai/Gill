#pragma once
#include "mathpp.hpp"
#include "gizmoData.hpp"
#include <memory>

class Scene;
class Mesh;
class Shader;
using Entity = uint32_t;
class TransformSystem;


class Gizmo {
public:

    void Init(unsigned int width,unsigned int height,GizmoData* gizmoData,TransformSystem* transformSystem);
    void Render(Scene* scene,const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& gizmoPosition, const mathpp::vec3f& cameraPos,Entity entity);
    void RenderIDs(const mathpp::mat4f& view, const mathpp::mat4f& projection, const mathpp::vec3f& gizmoPosition, const mathpp::vec3f& cameraPos, Entity entity);
    void UpdateHighlight(int x, int y,GizmoAxis dragAxis,bool isDragging = false);
    void DrawOriginMarker(const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& gizmoPosition);
    GizmoAxis ReadAxisAt(int x, int y) const;
    ~Gizmo();

private:
    void DrawAxis(const mathpp::vec3f& gizmoPosition, const mathpp::mat4f& axisRotation,const mathpp::vec3f& color, float scale);
    void DrawAxisID(const mathpp::vec3f& gizmoPosition, const mathpp::mat4f& axisRotation, float scale, unsigned int ID);
    void ComputeAxisRotations(Entity entity, mathpp::mat4f& outRotX, mathpp::mat4f& outRotY, mathpp::mat4f& outRotZ) const;
    std::unique_ptr<Mesh> translateMesh;
    std::unique_ptr<Mesh> rotateMesh;
    std::unique_ptr<Mesh> scaleMesh;
    std::unique_ptr<Mesh> planeMesh;
    std::unique_ptr<Shader> gizmoShader;
    std::unique_ptr<Shader> gizmoShaderID;
    unsigned int pickFBO, pickTexture, pickDepthRenderBuffer;
    unsigned int pickWidth, pickHeight;
    unsigned int originVBO,originVAO;
    float wdth,hght;
    float AxisHeight = 1.0f;
    GizmoData* _gizmoData;
    GizmoAxis highlightedAxis = GizmoAxis::None;
    TransformSystem* _transformSystem;
};