#pragma once
#include "scene/scene.hpp"
#include "io/obj/obj.hpp"
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
    std::unique_ptr<Mesh> AxisMesh;
    std::unique_ptr<Shader> gizmoShader;
    std::unique_ptr<Shader> gizmoShaderID;
    unsigned int pickFBO, pickTexture, pickDepthRenderBuffer;
    unsigned int pickWidth, pickHeight;

    float wdth,hght;

    float AxisHeight = 1.0f;
};