#include "scene/scene.hpp"





Entity Scene::CreateEntity() {
    Entity entity = m_entityManager.CreateEntity();
    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    m_entityManager.DestroyEntity(entity);
}

Scene::~Scene() = default;












uint32_t Scene::GetEntityCount() {
    return m_entityManager.GetEntityCount();
}

std::vector<Entity> Scene::GetLivingEntities() const {
    return m_entityManager.GetLivingEntity();
}




