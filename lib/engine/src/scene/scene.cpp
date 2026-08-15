#include "../include/scene/scene.hpp"

Entity Scene::CreateEntity() {
    Entity entity = entityManager.CreateEntity();
    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    entityManager.DestroyEntity(entity);
}

Scene::Scene() {
//idk what to put here
}

Scene::~Scene() {
    //here too
}

std::unordered_map<Entity, MeshComponent> Scene::GetAllMeshes() const {
    return meshes;
}

std::unordered_map<Entity, TransformComponent> Scene::GetAllTransforms() const {
    return transforms;
}


