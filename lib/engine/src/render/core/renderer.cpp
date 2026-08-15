#include "../include/render/core/renderer.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "scene/scene.hpp"

Renderer::Renderer() {
    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderScene(const Scene *scene) {
    const std::unordered_map<Entity, MeshComponent> & meshes = scene->GetAllMeshes();
    const std::unordered_map<Entity, TransformComponent> & transforms = scene->GetAllTransforms();

    for (auto it = meshes.begin(); it != meshes.end(); ++it) {
         const MeshComponent* mesh = &it->second;
         const TransformComponent* transform = scene->GetComponent<TransformComponent>(it->first);

    }
}
