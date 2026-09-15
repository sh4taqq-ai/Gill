#include "render/core/renderer.hpp"
#include "render/shader/shader.hpp"
#include "render/texture/texture.hpp"
#include "core/system/transform/transform.hpp"
#include "core/system/mesh/mesh.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "core/system/material/material.hpp"
#include "scene/scene.hpp"

void Renderer::Init(TransformSystem* transformSystem,MeshSystem* meshSystem,MaterialSystem* materialSystem) {
    glEnable(GL_DEPTH_TEST);
    _transformSystem = transformSystem;
    _meshSystem = meshSystem;
    _materialSystem = materialSystem;
    textureShader = std::make_unique<Shader>("asset/shader/textureShader/textureVert.glsl","asset/shader/textureShader/textureFrag.glsl");
    solidShader = std::make_unique<Shader>("asset/shader/solidShader/solidVert.glsl","asset/shader/solidShader/solidFrag.glsl");
    matCapTexture = std::make_unique<Texture>("asset/texture/core/SolidTex1.png");
    fallBackTexture = std::make_unique<Texture>("asset/texture/core/Debugempty.png");

}

void Renderer::SetViewportMode(ViewportMode mode) {
    viewportMode = mode;
}

ViewportMode Renderer::GetViewportMode() {
    return viewportMode;
}

void Renderer::renderScene(const Scene* scene, const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& viewVec) {
    scene->ForEach<comp::MeshComponent>([this, scene, &view, &projection,&viewVec](Entity entity,const comp::MeshComponent& meshComp) {
        DrawEntity(scene, entity, view, meshComp,projection,viewVec);
    });
}






void Renderer::DrawEntity(const Scene* scene, Entity entity,
                           const mathpp::mat4f& view,const comp::MeshComponent& meshComp, const mathpp::mat4f& proj, const mathpp::vec3f& viewVec) {
    const comp::MaterialComponent* matComp = scene->TryGetComponent<comp::MaterialComponent>(entity);
    const Material* mat = matComp ? _materialSystem->GetMaterial(matComp->materialID) : nullptr;
    auto mesh = _meshSystem->GetMesh(meshComp.meshID);
    if (!mesh) return;


    if (viewportMode == ViewportMode::Solid) {
        solidShader->Use();
        solidShader->setMat4f("view", view);
        solidShader->setMat4f("projection", proj);
        mathpp::mat4f model = _transformSystem->GetWorldTransform(entity);
        solidShader->setMat4f("model", model );
        mathpp::mat3f normalMat = mathpp::normal_matrix(view * model);
        solidShader->setMat3f("normalMatrix", normalMat);
        matCapTexture->Bind(0);
        solidShader->setInt("matCap",0);
    } else if (viewportMode == ViewportMode::Rendered) {
        const Shader* shader = mat ? _materialSystem->GetShader(mat->shaderID) : nullptr;
        if (!shader) shader = solidShader.get(); // no material yet -> fall back
        shader->Use();
        shader->setMat4f("view", view);
        shader->setMat4f("projection", proj);
        shader->setMat4f("model", _transformSystem->GetWorldTransform(entity));
        shader->setVec3f("albedo", mat ? mat->albedo : mathpp::vec3f{1.0f,1.0f,1.0f});
        shader->setVec3f("viewVec", viewVec);
        shader->setVec3f("lightDir", mathpp::vec3f{0.8f,0.2f,0.0f});
    }
    else {
        textureShader->Use();
        const Texture* tex = mat ? _materialSystem->GetTexture(mat->textureID) : nullptr;
        if (!tex) tex = fallBackTexture.get();
        tex->Bind(0);
        textureShader->setInt("meshTexture", 0);
        textureShader->setMat4f("view", view);
        textureShader->setMat4f("projection", proj);
        textureShader->setMat4f("model", _transformSystem->GetWorldTransform(entity));

    }

    mesh->Draw();
}

Renderer::~Renderer() = default;

