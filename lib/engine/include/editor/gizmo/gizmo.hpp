#pragma once
#include "../include/scene/scene.hpp"
#include "../include/render/mesh/primitive.hpp"
#include "mathpp.hpp"
enum class GizmoAxis : int {
    None = 0,
    X = 1,
    Y = 2,
    Z = 3
};
class Gizmo {
public:

    void Init(unsigned int width,unsigned int height);
    void Render(Scene* scene,const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& gizmoPosition, const mathpp::vec3f& cameraPos);
    void RenderIDs(const mathpp::mat4f& view, const mathpp::mat4f& projection,
                   const mathpp::vec3f& gizmoPosition, const mathpp::vec3f& cameraPos);
    GizmoAxis ReadAxisAt(int x, int y) const;

private:
    void DrawAxis(const mathpp::vec3f& gizmoPosition, const mathpp::mat4f& axisRotation,
                const mathpp::vec3f& color, float scale);
    void DrawAxisID(const mathpp::vec3f& gizmoPosition, const mathpp::mat4f& axisRotation, float scale, unsigned int ID);
    std::unique_ptr<Mesh> cylinderMesh;
    std::unique_ptr<Mesh> coneMesh;
    std::unique_ptr<Shader> gizmoShader;
    std::unique_ptr<Shader> gizmoShaderID;
    unsigned int pickFBO, pickTexture, pickDepthRenderBuffer;
    unsigned int pickWidth, pickHeight;

    float wdth,hght;

    float shaftHeight = 1.5f;
    float shaftRadius = 0.0005f;
    float tipHeight = 0.3f;
    float tipRadius = 0.012f;
};