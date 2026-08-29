#include "scene/scene.hpp"



Entity Scene::CreateEntity() {
    Entity entity = entityManager.CreateEntity();
    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    entityManager.DestroyEntity(entity);
}

Scene::~Scene() = default;


const Mesh *Scene::GetMesh(AssetID id) const {
    return meshManager.Get(id);
}

const Shader *Scene::GetShader(AssetID id) const {
    return shaderManager.Get(id);
}





std::optional<Entity> Scene::GetSelected() const {
    return selected;
}

void Scene::SetSelected(const std::optional<Entity> &value) {
    selected = value;
}


uint32_t Scene::GetEntityCount() {
    return entityManager.GetEntityCount();
}

std::vector<Entity> Scene::GetLivingEntities() const {
    return entityManager.GetLivingEntity();
}
