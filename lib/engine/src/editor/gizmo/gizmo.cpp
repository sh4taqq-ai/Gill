#include "editor/gizmo/gizmo.hpp"
#include "glad/gl.h"
#include "core/input/input.hpp"
#include "io/obj/obj.hpp"


void Gizmo::Init(unsigned int width,unsigned int height) {
    wdth = width;
    hght = height;
    gizmoShader = std::make_unique<Shader>("asset/shader/gizmo/gizmoShader/gizmoVert.glsl", "asset/shader/gizmo/gizmoShader/gizmoFrag.glsl");
    gizmoShaderID = std::make_unique<Shader>("asset/shader/gizmo/gizmoShaderID/gizmoVertID.glsl","asset/shader/gizmo/gizmoShaderID/gizmoFragID.glsl");
    translateMesh = std::make_unique<Mesh>(LoadOBJ("asset/mesh/core/gizmo/GizmoTranslate.obj"));
    rotateMesh = std::make_unique<Mesh>(LoadOBJ("asset/mesh/core/gizmo/GizmoRotation.obj"));
    scaleMesh = std::make_unique<Mesh>(LoadOBJ("asset/mesh/core/gizmo/GizmoScale.obj"));
    planeMesh = std::make_unique<Mesh>(LoadOBJ("asset/mesh/core/gizmo/GizmoPlane.obj"));
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
    float scale = distance * 0.08f;

    mathpp::mat4f rotY = mathpp::EulerAnglesRotation<float>({0.0f, 0.0f, 0.0f});   // Y arrow: native
    mathpp::mat4f rotX = mathpp::EulerAnglesRotation<float>({0.0f, 0.0f, -90.0f}); // X arrow
    mathpp::mat4f rotZ = mathpp::EulerAnglesRotation<float>({90.0f, 0.0f, 0.0f});  // Z arrow

    mathpp::vec3f xColor = (highlightedAxis == GizmoAxis::X) ? mathpp::vec3f(1.0f,1.0f,0.0f) : mathpp::vec3f(0.8f,0.0f,0.0f);
    DrawAxis(gizmoPosition, rotX, xColor, scale);

    mathpp::vec3f yColor = (highlightedAxis == GizmoAxis::Y) ? mathpp::vec3f(0.0f,1.0f,1.0f) : mathpp::vec3f(0.0f,0.8f,0.0f);
    DrawAxis(gizmoPosition, rotY, yColor, scale);
    mathpp::vec3f zColor = (highlightedAxis == GizmoAxis::Z) ? mathpp::vec3f(1.0f,0.0f,1.0f) : mathpp::vec3f(0.0f,0.0f,0.8f);
    DrawAxis(gizmoPosition, rotZ, zColor, scale);


    glEnable(GL_DEPTH_TEST);
}



void Gizmo::DrawAxis(const mathpp::vec3f &gizmoPosition, const mathpp::mat4f &axisRotation, const mathpp::vec3f &color, float scale) {
    gizmoShader->setVec3f("axisColor", color);
    mathpp::mat4f identity;
    Mesh* drawMesh = nullptr;
    if (_state == GizmoState::Translate){drawMesh = translateMesh.get();}
    else if (_state == GizmoState::Rotate){drawMesh = rotateMesh.get();}
    else if (_state == GizmoState::Scale){drawMesh = scaleMesh.get();}
    mathpp::mat4f meshModel = mathpp::translate(identity, gizmoPosition);
    meshModel = meshModel * axisRotation;
    meshModel = mathpp::translate(meshModel, {0.0f, AxisHeight * 0.4f * scale, 0.0f});
    meshModel = mathpp::scale(meshModel, {scale, scale, scale});
    gizmoShaderID->setMat4f("model", meshModel);
    drawMesh->Draw();
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
    float scale = distance * 0.08f;

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
    Mesh* drawMesh = nullptr;
    if (_state == GizmoState::Translate){drawMesh = translateMesh.get();}
    else if (_state == GizmoState::Rotate){drawMesh = rotateMesh.get();}
    else if (_state == GizmoState::Scale){drawMesh = scaleMesh.get();}
    mathpp::mat4f axisModel = mathpp::translate(identity, gizmoPosition);
    axisModel = axisModel * axisRotation;
    axisModel = mathpp::translate(axisModel, {0.0f, AxisHeight * 0.4f * scale, 0.0f});
    axisModel = mathpp::scale(axisModel, {scale, scale, scale});
    gizmoShader->setMat4f("model", axisModel);
    translateMesh->Draw();

}

GizmoAxis Gizmo::ReadAxisAt(int x, int y) const {
    glBindFramebuffer(GL_FRAMEBUFFER, pickFBO);
    int flippedY = hght - y;  // flip since glReadPixels origin is bottom-left

    GLint pickedID = 0;
    glReadPixels(x, flippedY, 1, 1, GL_RED_INTEGER, GL_INT, &pickedID);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return static_cast<GizmoAxis>(pickedID);
}


void Gizmo::Switch() {
    if (_state == GizmoState::Translate) { _state = GizmoState::Rotate; }
    else if (_state == GizmoState::Rotate) { _state = GizmoState::Scale; }
    else { _state = GizmoState::Translate; }
}

void Gizmo::UpdateHighlight(int x, int y, GizmoAxis dragAxis, bool isDragging)  {
    if (isDragging) {
        highlightedAxis = dragAxis;
    }
    else {
        highlightedAxis = ReadAxisAt(x,y);
    }
}

