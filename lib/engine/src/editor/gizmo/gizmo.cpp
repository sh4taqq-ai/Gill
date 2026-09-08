#include "editor/gizmo/gizmo.hpp"
#include "core/debug/error.hpp"
#include <iostream>
#include <ostream>
#include "core/system/transform/transform.hpp"
#include "glad/gl.h"
#include "core/input/input.hpp"
#include "io/obj/obj.hpp"
#include "scene/scene.hpp"
#include "render/shader/shader.hpp"
#include <iostream>


void Gizmo::Init(unsigned int width,unsigned int height,GizmoData* gizmoData,TransformSystem* transformSystem) {
    wdth = width;
    hght = height;
    glEnable(GL_PROGRAM_POINT_SIZE);
    glGenVertexArrays(1, &originVAO);
    glBindVertexArray(originVAO);
    glGenBuffers(1, &originVBO);
    glBindBuffer(GL_ARRAY_BUFFER, originVBO);
    float origin[3] = {0.0f, 0.0f, 0.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(origin), &origin[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    _transformSystem = transformSystem;
    _gizmoData = gizmoData;
    gizmoShader = std::make_unique<Shader>("asset/shader/gizmo/gizmoShader/gizmoVert.glsl", "asset/shader/gizmo/gizmoShader/gizmoFrag.glsl");
    gizmoShaderID = std::make_unique<Shader>("asset/shader/gizmo/gizmoShaderID/gizmoVertID.glsl","asset/shader/gizmo/gizmoShaderID/gizmoFragID.glsl");
    translateMesh = std::make_unique<Mesh>(LoadOBJ("asset/mesh/core/gizmo/GizmoTranslate.obj"));
    rotateMesh = std::make_unique<Mesh>(LoadOBJ("asset/mesh/core/gizmo/GizmoRotate.obj"));
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

void Gizmo::Render(Scene* scene,const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& gizmoPosition, const mathpp::vec3f& cameraPos,Entity entity) {
    gizmoShader->Use();
    gizmoShader->setMat4f("view", view);
    gizmoShader->setMat4f("projection", projection);


    glDisable(GL_DEPTH_TEST);
    float distance = mathpp::length(cameraPos - gizmoPosition);
    float scale = distance * 0.08f;
    mathpp::mat4f rotY;
    mathpp::mat4f rotX;
    mathpp::mat4f rotZ;
    ComputeAxisRotations(entity, rotX, rotY, rotZ);

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
    if (_gizmoData->mode == GizmoMode::Translate){drawMesh = translateMesh.get();}
    else if (_gizmoData->mode == GizmoMode::Rotate){drawMesh = rotateMesh.get();}
    else if (_gizmoData->mode == GizmoMode::Scale){drawMesh = scaleMesh.get();}
    mathpp::mat4f meshModel = mathpp::translate(identity, gizmoPosition);
    meshModel = meshModel * axisRotation;
    if (_gizmoData->mode != GizmoMode::Rotate)
    {
    meshModel = mathpp::translate(meshModel, {0.0f, AxisHeight * 0.4f * scale, 0.0f});
        meshModel = meshModel = mathpp::scale(meshModel, {scale, scale, scale});

    }
    else{
        meshModel = meshModel = mathpp::scale(meshModel, {scale, scale, scale});
    }

    gizmoShader->setMat4f("model", meshModel);
    drawMesh->Draw();
}

void Gizmo::RenderIDs(const mathpp::mat4f &view, const mathpp::mat4f &projection, const mathpp::vec3f &gizmoPosition, const mathpp::vec3f &cameraPos,Entity entity) {
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

    mathpp::mat4f rotY;
    mathpp::mat4f rotX;
    mathpp::mat4f rotZ;

    ComputeAxisRotations(entity, rotX, rotY, rotZ);

    DrawAxisID(gizmoPosition, rotX, scale,static_cast<int>(GizmoAxis::X));
    DrawAxisID(gizmoPosition, rotY,  scale,static_cast<int>(GizmoAxis::Y));
    DrawAxisID(gizmoPosition, rotZ,  scale,static_cast<int>(GizmoAxis::Z));

    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Gizmo::DrawAxisID(const mathpp::vec3f &gizmoPosition, const mathpp::mat4f &axisRotation, float scale, unsigned int ID) {
    gizmoShaderID->setInt("GizmoAxis",ID);
    mathpp::mat4f identity;
    Mesh* drawMesh = nullptr;
    if (_gizmoData->mode == GizmoMode::Translate){drawMesh = translateMesh.get();}
    else if (_gizmoData->mode == GizmoMode::Rotate){drawMesh = rotateMesh.get();}
    else if (_gizmoData->mode == GizmoMode::Scale){drawMesh = scaleMesh.get();}
    mathpp::mat4f axisModel = mathpp::translate(identity, gizmoPosition);
    axisModel = axisModel * axisRotation;
    if (_gizmoData->mode != GizmoMode::Rotate)
    {
        axisModel = mathpp::translate(axisModel, {0.0f, AxisHeight * 0.4f * scale, 0.0f});
        axisModel = axisModel = mathpp::scale(axisModel, {scale, scale, scale});

    }
    else{
        axisModel = axisModel = mathpp::scale(axisModel, {scale, scale, scale});
    }
    gizmoShaderID->setMat4f("model", axisModel);
    drawMesh->Draw();

}

GizmoAxis Gizmo::ReadAxisAt(int x, int y) const {
    glBindFramebuffer(GL_FRAMEBUFFER, pickFBO);
    int flippedY = hght - y;  // flip since glReadPixels origin is bottom-left

    GLint pickedID = 0;
    glReadPixels(x, flippedY, 1, 1, GL_RED_INTEGER, GL_INT, &pickedID);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return static_cast<GizmoAxis>(pickedID);
}



void Gizmo::UpdateHighlight(int x, int y, GizmoAxis dragAxis, bool isDragging)  {
    if (isDragging) {
        highlightedAxis = dragAxis;
    }
    else {
        highlightedAxis = ReadAxisAt(x,y);
    }
}
Gizmo::~Gizmo() = default;

void Gizmo::ComputeAxisRotations(Entity entity, mathpp::mat4f& outRotX, mathpp::mat4f& outRotY, mathpp::mat4f& outRotZ) const {
    mathpp::quatf worldRot = (_gizmoData->referenceFrame == ReferenceFrame::Local)
        ? _transformSystem->GetWorldRotation(entity)
        : mathpp::quatf{};
    mathpp::mat4f worldRotMat = mathpp::QuatToMat4(worldRot);

    if (_gizmoData->mode != GizmoMode::Rotate) {
        outRotY = worldRotMat * mathpp::EulerAnglesRotation<float>({0.0f, 90.0f, 0.0f});
        outRotX = worldRotMat * mathpp::EulerAnglesRotation<float>({0.0f, 0.0f, -90.0f});
        outRotZ = worldRotMat * mathpp::EulerAnglesRotation<float>({90.0f, 0.0f, 0.0f});
    } else {
        outRotZ = worldRotMat * mathpp::EulerAnglesRotation<float>({0.0f, 90.0f, 0.0f});
        outRotY = worldRotMat * mathpp::EulerAnglesRotation<float>({0.0f, 0.0f, -90.0f});
        outRotX = worldRotMat * mathpp::EulerAnglesRotation<float>({90.0f, 0.0f, 0.0f});
    }
}

void Gizmo::DrawOriginMarker(const mathpp::mat4f& view, const mathpp::mat4f& projection, const mathpp::vec3f& gizmoPosition) {
    gizmoShader->Use();
    gizmoShader->setMat4f("view", view);
    gizmoShader->setMat4f("projection", projection);
    gizmoShader->setVec3f("axisColor", mathpp::vec3f(1.0f, 1.0f, 1.0f));

    mathpp::mat4f model = mathpp::translate(mathpp::mat4f(), gizmoPosition);
    gizmoShader->setMat4f("model", model);

    bool depthWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(originVAO);
    glPointSize(20.0f);
    glDrawArrays(GL_POINTS, 0, 1);

    if (depthWasEnabled) glEnable(GL_DEPTH_TEST);  // restore whatever state it found
}

