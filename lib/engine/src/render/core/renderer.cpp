#include "render/core/renderer.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "scene/scene.hpp"

void Renderer::Init() {
    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderScene(const Scene* scene, const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& viewVec) {
    scene->ForEach<MeshComponent>([this, scene, &view, &projection](Entity entity,const MeshComponent& meshComp) {
        DrawEntity(scene, entity, meshComp, view, projection);
    });
}




void Renderer::DrawEntity(const Scene* scene, Entity entity, const MeshComponent& meshComp, const mathpp::mat4f& view, const mathpp::mat4f& proj) {
    auto& transformComp = scene->GetComponent<TransformComponent>(entity);
    auto mesh = scene->GetMesh(meshComp.meshID);
    auto shader = scene->GetShader(meshComp.shaderID);

    shader->Use();
    shader->setMat4f("view", view);
    shader->setMat4f("projection", proj);
    shader->setMat4f("model", transformComp.getMatrix());

    mesh->Draw();
}

Renderer::~Renderer() = default;