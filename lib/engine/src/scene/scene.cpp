#include "scene/scene.hpp"





Entity Scene::CreateEntity() {
    Entity entity = entityManager.CreateEntity();
    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    entityManager.DestroyEntity(entity);
}

Scene::~Scene() = default;










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




