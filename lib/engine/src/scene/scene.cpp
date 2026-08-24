#include "scene/scene.hpp"



Entity Scene::CreateEntity() {
    Entity entity = entityManager.CreateEntity();
    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    entityManager.DestroyEntity(entity);
}





const Mesh *Scene::GetMesh(AssetID id) const {
    return meshManager.Get(id);
}

const Shader *Scene::GetShader(AssetID id) const {
    return shaderManager.Get(id);
}

const std::unordered_map<Entity, MeshComponent>& Scene::GetAllMeshes() const {
    return meshes;
}

const std::unordered_map<Entity, TransformComponent>& Scene::GetAllTransforms() const {
    return transforms;
}

void Scene::AddTransform(Entity entity, const TransformComponent& transform) {
    transforms.insert({entity, transform});
}

void Scene::AddMesh(Entity entity, const MeshComponent &mesh) {
    meshes.insert({entity, mesh});
}

void Scene::AddSunlight(Entity entity, const SunlightComponent &sun) {
    sunlights.insert({entity, sun});
}

std::optional<Entity> Scene::SelectEntity(const Ray& ray, float radius) const {
    for (const auto& [entity, meshComp] : meshes) {           // loop over ENTITIES
        const TransformComponent* t = GetComponent<TransformComponent>(entity);
        if (t && RaySphereIntersect(ray, t->position, radius)) {  // ONE check per entity, no inner loop
            return entity;  // found a hit
        }
    }
    return std::nullopt;
}


std::optional<Entity> Scene::GetSelected() const {
    return selected;
}

void Scene::SetSelected(const std::optional<Entity> &value) {
    selected = value;
}

const std::unordered_map<Entity, SunlightComponent> &Scene::GetAllSunlights() const {
    return  sunlights;
}

