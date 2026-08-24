#include "editor/selector/selector.hpp"




void Selector::Init(unsigned int width, unsigned int height) {
    selectShader = std::make_unique<Shader>("asset/shader/selectShader/selectVert.glsl","asset/shader/selectShader/selectFrag.glsl");
    wdth = width;
    hght = height;
    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);

    glGenTextures(1,&Texture);
    glBindTexture(GL_TEXTURE_2D,Texture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_R32I,wdth,hght,0,GL_RED_INTEGER,GL_INT,nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,Texture,0);
    glGenRenderbuffers(1,&depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER,depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthRenderBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}



void Selector::RenderScene(Scene* scene, const mathpp::mat4f& view, const mathpp::mat4f& projection) {
    GLint clearValue = -1;
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClearBufferiv(GL_COLOR, 0, &clearValue);
    glClear(GL_DEPTH_BUFFER_BIT);
    selectShader->Use();

    const auto& meshes = scene->GetAllMeshes();
    for (const auto& [entity, meshComp] : meshes) {
        const auto* transform = scene->GetComponent<TransformComponent>(entity);
        if (!transform) continue;

        selectShader->setMat4f("model", transform->getMatrix());
        selectShader->setMat4f("view", view);
        selectShader->setMat4f("projection", projection);
        selectShader->setInt("ObjectID", static_cast<int>(entity));
        scene->GetMesh(meshComp.meshID)->Draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::optional<Entity> Selector::ReadEntityAt(int x, int y) const {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    int flippedY = hght - y;  // flip since glReadPixels origin is bottom-left

    GLint pickedID = -1;
    glReadPixels(x, flippedY, 1, 1, GL_RED_INTEGER, GL_INT, &pickedID);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (pickedID == -1)
    {return std::nullopt;}

    return static_cast<Entity>(pickedID);
}

