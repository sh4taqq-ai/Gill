#pragma once
#include "scene/scene.hpp"
#include "mathpp.hpp"
#include "gizmoData.hpp"


class Gizmo {
public:

    void Init(unsigned int width,unsigned int height);
    void Render(Scene* scene,const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& gizmoPosition, const mathpp::vec3f& cameraPos);
    void RenderIDs(const mathpp::mat4f& view, const mathpp::mat4f& projection, const mathpp::vec3f& gizmoPosition, const mathpp::vec3f& cameraPos);
    void Switch();
    void UpdateHighlight(int x, int y,GizmoAxis dragAxis,bool isDragging = false);
    GizmoAxis ReadAxisAt(int x, int y) const;

private:
    void DrawAxis(const mathpp::vec3f& gizmoPosition, const mathpp::mat4f& axisRotation,
                const mathpp::vec3f& color, float scale);
    void DrawAxisID(const mathpp::vec3f& gizmoPosition, const mathpp::mat4f& axisRotation, float scale, unsigned int ID);
    std::unique_ptr<Mesh> translateMesh;
    std::unique_ptr<Mesh> rotateMesh;
    std::unique_ptr<Mesh> scaleMesh;
    std::unique_ptr<Mesh> planeMesh;
    std::unique_ptr<Shader> gizmoShader;
    std::unique_ptr<Shader> gizmoShaderID;
    unsigned int pickFBO, pickTexture, pickDepthRenderBuffer;
    unsigned int pickWidth, pickHeight;
    float wdth,hght;
    float AxisHeight = 1.0f;
    GizmoState _state = GizmoState::Translate;
    GizmoAxis highlightedAxis = GizmoAxis::None;
};