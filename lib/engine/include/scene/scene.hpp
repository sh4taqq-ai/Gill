#pragma once
#include <vector>
#include "component/entity.hpp"
#include "component/mesh.hpp"
#include "component/sunlight.hpp"
#include <functional>
#include "core/set/sparseset.hpp"
#include "core/system/asset/asset.hpp"
#include "component/material.hpp"
#include <optional>
#include "render/mesh/mesh.hpp"

class Scene {
public:

    ~Scene();


    Entity CreateEntity();
    void DestroyEntity(Entity entity);


    template <typename T>
    void InsertComponent(Entity entity,const T& component);

    template <typename T>
    void RemoveComponent(Entity entity);

    template <typename T>
    void ForEach(std::function<void(Entity entity, const T& component)> func) const;

    template <typename T>
    void ForEach(std::function<void(Entity entity, T& component)> func);


    template<typename T>
   const T* TryGetComponent(Entity entity) const;





    template<typename T>
T* TryGetComponent(Entity entity);

    template <typename T>
    T& GetComponent(Entity entity);

    template<typename T>
    const T& GetComponent(Entity entity) const;







    AssetHandle LoadMesh(Mesh mesh) { return meshManager.Load(std::move(mesh)); }











    std::optional<Entity> GetSelected() const;
    void SetSelected(const std::optional<Entity>& value);

    std::vector<Entity> GetLivingEntities() const;

    uint32_t GetEntityCount();



    private:
    std::optional<Entity> selected;
    EntityManager entityManager;
    AssetManager<Mesh> meshManager;
    SparseSet<comp::MeshComponent> meshes;
    SparseSet<comp::SunlightComponent> sunlights;
    SparseSet<comp::MaterialComponent> materials;
};




    template<>
    inline comp::SunlightComponent& Scene::GetComponent<comp::SunlightComponent>(Entity entity) {
        return sunlights.Get(entity);
    }
    template<>
    inline const comp::SunlightComponent& Scene::GetComponent<comp::SunlightComponent>(Entity entity) const {
        return sunlights.Get(entity);
    }
    template<>
    inline comp::MeshComponent& Scene::GetComponent<comp::MeshComponent>(Entity entity) {
        return meshes.Get(entity);
    }
    template<>
    inline const comp::MeshComponent& Scene::GetComponent<comp::MeshComponent>(Entity entity) const {
        return meshes.Get(entity);
    }
    template<>
    inline comp::MaterialComponent& Scene::GetComponent<comp::MaterialComponent>(Entity entity) {
        return materials.Get(entity);
    }
    template<>
    inline const comp::MaterialComponent& Scene::GetComponent<comp::MaterialComponent>(Entity entity) const {
        return materials.Get(entity);
    }

    template<>
    inline comp::MeshComponent* Scene::TryGetComponent<comp::MeshComponent>(Entity entity) {
        if (meshes.Has(entity)) {
            return &meshes.Get(entity);
        }
        return nullptr;
    }
    template<>
    inline const comp::MeshComponent* Scene::TryGetComponent<comp::MeshComponent>(Entity entity) const {
        if (meshes.Has(entity)) {
            return &meshes.Get(entity);
        }
        return nullptr;
    }
    template<>
    inline comp::SunlightComponent* Scene::TryGetComponent<comp::SunlightComponent>(Entity entity) {
        if (sunlights.Has(entity)) {
            return &sunlights.Get(entity);
        }
        return nullptr;
    }
    template<>
    inline const comp::SunlightComponent* Scene::TryGetComponent<comp::SunlightComponent>(Entity entity) const {
        if (sunlights.Has(entity)) {
            return &sunlights.Get(entity);
        }
        return nullptr;
    }
    template<>
    inline const comp::MaterialComponent* Scene::TryGetComponent<comp::MaterialComponent>(Entity entity) const {
        if (materials.Has(entity)) {
            return &materials.Get(entity);
        }
        return nullptr;
    }

    template<>
    inline comp::MaterialComponent*  Scene::TryGetComponent<comp::MaterialComponent>(Entity entity) {
        if (materials.Has(entity)) {
            return &materials.Get(entity);
        }
        return nullptr;
    }
    template<>
    inline void Scene::InsertComponent<comp::MeshComponent>(Entity entity,const comp::MeshComponent& component) {
        meshes.Insert(entity, component);
    }
    template<>
    inline void Scene::InsertComponent<comp::MaterialComponent>(Entity entity,const comp::MaterialComponent& component) {
        materials.Insert(entity,component);
    }
    template<>
    inline void Scene::RemoveComponent<comp::MeshComponent>(Entity entity) {
        meshes.Remove(entity);
    }
    template<>
    inline void Scene::InsertComponent<comp::SunlightComponent>(Entity entity,const comp::SunlightComponent& component) {
        sunlights.Insert(entity, component);
    }
    template<>
    inline void Scene::RemoveComponent<comp::SunlightComponent>(Entity entity) {
        sunlights.Remove(entity);
    }
    template<>
    inline void Scene::RemoveComponent<comp::MaterialComponent>(Entity entity) {
        materials.Remove(entity);
    }

    template<>
    inline void Scene::ForEach<comp::MeshComponent>(std::function<void(Entity entity,const  comp::MeshComponent& component)> func) const{
        for (size_t it = 0; it<meshes.Size();it++) {
            func(meshes.GetEntity(it), meshes[it]);
        }
    }

    template<>
    inline void Scene::ForEach<comp::SunlightComponent>(std::function<void(Entity entity, const comp::SunlightComponent& component)> func) const {
        for (size_t it = 0; it<sunlights.Size();it++) {
            func(sunlights.GetEntity(it), sunlights[it]);
        }
    }
    template<>
    inline void Scene::ForEach<comp::MaterialComponent>(std::function<void(Entity entity, const comp::MaterialComponent& component)> func) const {
        for (size_t it = 0; it<materials.Size();it++) {
            func(materials.GetEntity(it), materials[it]);
        }
    }

