#include "render/core/renderer.hpp"
#include "render/shader/shader.hpp"
#include "render/texture/texture.hpp"
#include "core/system/transform/transform.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "scene/scene.hpp"

void Renderer::Init(TransformSystem* transformSystem) {
    glEnable(GL_DEPTH_TEST);
    _transformSystem = transformSystem;
    textureShader = std::make_unique<Shader>("asset/shader/textureShader/textureVert.glsl","assets/shader/textureShader/textureFrag.glsl");
    solidShader = std::make_unique<Shader>("asset/shader/solidShader/solidVert.glsl","asset/shader/solidShader/solidFrag.glsl");
    matCapTexture = std::make_unique<Texture>("asset/texture/core/SolidTex1.png");
    fallBackTexture = std::make_unique<Texture>("asset/texture/core/Debugempty.png");
}

void Renderer::SetViewportMode(ViewportMode mode) {
    _viewportMode = mode;
}

ViewportMode Renderer::GetViewportMode() {
    return _viewportMode;
}

void Renderer::renderScene(const Scene* scene, const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& viewVec) {
    scene->ForEach<MeshComponent>([this, scene, &view, &projection,&viewVec](Entity entity,const MeshComponent& meshComp) {
        DrawEntity(scene, entity, view, projection,viewVec);
    });
}




void Renderer::DrawEntity(const Scene* scene, Entity entity, const mathpp::mat4f& view, const mathpp::mat4f& proj,const mathpp::vec3f& viewVec) {
    const MeshComponent& meshComp = scene->GetComponent<MeshComponent>(entity);
    const MaterialComponent& matComp = scene->GetComponent<MaterialComponent>(entity);
    auto mesh = GetMesh(meshComp.meshID);



    if (_viewportMode == ViewportMode::Solid) {
        solidShader->Use();
        solidShader->setMat4f("view", view);
        solidShader->setMat4f("projection", proj);
        mathpp::mat4f model = _transformSystem->GetWorldTransform(entity);
        solidShader->setMat4f("model", model );
        mathpp::mat3f normalMat = mathpp::normal_matrix(view * model);
        solidShader->setMat3f("normalMatrix", normalMat);
        matCapTexture->Bind(0);
        solidShader->setInt("matCap",0);
    } else if (_viewportMode == ViewportMode::Rendered) {
        auto shader = GetShader(matComp.shaderID);
        shader->Use();
        shader->setMat4f("view", view);
        shader->setMat4f("projection", proj);
        shader->setMat4f("model", _transformSystem->GetWorldTransform(entity));
        shader->setVec3f("albedo", mathpp::vec3f{1.0f,1.0f,1.0f});
        shader->setVec3f("viewVec",viewVec);
        shader->setVec3f("lightDir",mathpp::vec3f{0.8f,0.2f,0.0f});
    }
    else {
        textureShader->Use();
        const Texture* tex = HasTexture(matComp.textureID)? GetTexture(matComp.textureID): fallBackTexture.get();
        tex->Bind(0);
        textureShader->setInt("meshTexture", 0);
        textureShader->setMat4f("view", view);
        textureShader->setMat4f("projection", proj);
        textureShader->setMat4f("model", _transformSystem->GetWorldTransform(entity));

    }

    mesh->Draw();
}

Renderer::~Renderer() = default;


bool Renderer::HasTexture(AssetID id) const {
    return textureManager.Has(id);
}

bool Renderer::HasShader(AssetID id) const {
    return shaderManager.Has(id);
}
bool Renderer::HasMesh(AssetID id) const {
    return meshManager.Has(id);
}
const Mesh *Renderer::GetMesh(AssetID id) const {
    return meshManager.Get(id);
}
const Texture *Renderer::GetTexture(AssetID id) const {
    return textureManager.Get(id);
}
const Shader *Renderer::GetShader(AssetID id) const {
    return shaderManager.Get(id);
}