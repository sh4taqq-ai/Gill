#include "editor/gizmo/gizmo.hpp"
#include "glad/gl.h"


void Gizmo::Init(unsigned int width,unsigned int height) {
    wdth = width;
    hght = height;
    gizmoShader = std::make_unique<Shader>("asset/shader/gizmo/gizmoShader/gizmoVert.glsl", "asset/shader/gizmo/gizmoShader/gizmoFrag.glsl");
    gizmoShaderID = std::make_unique<Shader>("asset/shader/gizmo/gizmoShaderID/gizmoVertID.glsl","asset/shader/gizmo/gizmoShaderID/gizmoFragID.glsl");
    AxisMesh = std::make_unique<Mesh>(LoadOBJ("asset/mesh/core/GizmoArrow.obj"));

    glGenFramebuffers(1,&pickFBO);
    glBindFramebuffer(GL_FRAMEBUFFER,pickFBO);

    glGenTextures(1,&pickTexture);
    glBindTexture(GL_TEXTURE_2D,pickTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_R32I,wdth,hght,0,GL_RED_INTEGER,GL_INT,nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,pickTexture,0);
    glGenRenderbuffers(1,&pickDepthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER,pickDepthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,wdth,hght);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,pickDepthRenderBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Gizmo::Render(Scene* scene,const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& gizmoPosition, const mathpp::vec3f& cameraPos) {
    gizmoShader->Use();
    gizmoShader->setMat4f("view", view);
    gizmoShader->setMat4f("projection", projection);

    glDisable(GL_DEPTH_TEST);

    float distance = mathpp::length(cameraPos - gizmoPosition);
    float scale = distance * 0.15f;

    mathpp::mat4f rotY = mathpp::EulerAnglesRotation<float>({0.0f, 0.0f, 0.0f});   // Y arrow: native
    mathpp::mat4f rotX = mathpp::EulerAnglesRotation<float>({0.0f, 0.0f, -90.0f}); // X arrow
    mathpp::mat4f rotZ = mathpp::EulerAnglesRotation<float>({90.0f, 0.0f, 0.0f});  // Z arrow

    DrawAxis(gizmoPosition, rotX, {1.0f, 0.0f, 0.0f}, scale);
    DrawAxis(gizmoPosition, rotY, {0.0f, 1.0f, 0.0f}, scale);
    DrawAxis(gizmoPosition, rotZ, {0.0f, 0.0f, 1.0f}, scale);

    glEnable(GL_DEPTH_TEST);
}



void Gizmo::DrawAxis(const mathpp::vec3f &gizmoPosition, const mathpp::mat4f &axisRotation, const mathpp::vec3f &color, float scale) {
    gizmoShader->setVec3f("axisColor", color);

    mathpp::mat4f identity;

    mathpp::mat4f shaftModel = mathpp::translate(identity, gizmoPosition);
    shaftModel = shaftModel * axisRotation;
    shaftModel = mathpp::translate(shaftModel, {0.0f, AxisHeight * 0.5f * scale, 0.0f});
    shaftModel = mathpp::scale(shaftModel, {scale, scale, scale});
    gizmoShaderID->setMat4f("model", shaftModel);
    AxisMesh->Draw();


}

void Gizmo::RenderIDs(const mathpp::mat4f &view, const mathpp::mat4f &projection, const mathpp::vec3f &gizmoPosition, const mathpp::vec3f &cameraPos) {
    GLint clearValue = 0;
    glBindFramebuffer(GL_FRAMEBUFFER, pickFBO);
    glClearBufferiv(GL_COLOR, 0, &clearValue);
    glClear(GL_DEPTH_BUFFER_BIT);
    gizmoShaderID->Use();
    gizmoShaderID->setMat4f("view", view);
    gizmoShaderID->setMat4f("projection", projection);

    glDisable(GL_DEPTH_TEST);

    float distance = mathpp::length(cameraPos - gizmoPosition);
    float scale = distance * 0.15f;

    mathpp::mat4f rotY = mathpp::EulerAnglesRotation<float>({0.0f, 0.0f, 0.0f});   // Y arrow: native
    mathpp::mat4f rotX = mathpp::EulerAnglesRotation<float>({0.0f, 0.0f, -90.0f}); // X arrow
    mathpp::mat4f rotZ = mathpp::EulerAnglesRotation<float>({90.0f, 0.0f, 0.0f});  // Z arrow

    DrawAxisID(gizmoPosition, rotX, scale,static_cast<int>(GizmoAxis::X));
    DrawAxisID(gizmoPosition, rotY,  scale,static_cast<int>(GizmoAxis::Y));
    DrawAxisID(gizmoPosition, rotZ,  scale,static_cast<int>(GizmoAxis::Z));

    glEnable(GL_DEPTH_TEST);


}

void Gizmo::DrawAxisID(const mathpp::vec3f &gizmoPosition, const mathpp::mat4f &axisRotation, float scale, unsigned int ID) {
    gizmoShaderID->setInt("GizmoAxis",ID);
    mathpp::mat4f identity;

    mathpp::mat4f axisModel = mathpp::translate(identity, gizmoPosition);
    axisModel = axisModel * axisRotation;
    axisModel = mathpp::translate(axisModel, {0.0f, AxisHeight * 0.5f * scale, 0.0f});
    axisModel = mathpp::scale(axisModel, {scale, scale, scale});
    gizmoShader->setMat4f("model", axisModel);
    AxisMesh->Draw();

}

GizmoAxis Gizmo::ReadAxisAt(int x, int y) const {
    glBindFramebuffer(GL_FRAMEBUFFER, pickFBO);
    int flippedY = hght - y;  // flip since glReadPixels origin is bottom-left

    GLint pickedID = 0;
    glReadPixels(x, flippedY, 1, 1, GL_RED_INTEGER, GL_INT, &pickedID);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return static_cast<GizmoAxis>(pickedID);
}
