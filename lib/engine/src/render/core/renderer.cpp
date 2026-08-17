#include "../include/render/core/renderer.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "scene/scene.hpp"

void Renderer::Init() {
    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderScene(const Scene *scene, const mathpp::mat4f& view, const mathpp::mat4f& projection) {
    const std::unordered_map<Entity, MeshComponent> & meshes = scene->GetAllMeshes();

    for (auto it = meshes.begin(); it != meshes.end(); ++it) {
         const MeshComponent* mesh = &it->second;
         const TransformComponent* transform = scene->GetComponent<TransformComponent>(it->first);
        mesh->shader->Use();
        if (transform != nullptr) {
            mathpp::mat4f mat = transform->getMatrix();
            mesh->shader->setMat4f("model",mat);
            mesh->shader->setMat4f("view",view);
            mesh->shader->setMat4f("projection",projection);
        }
        mesh->mesh->Draw();

    }
}
