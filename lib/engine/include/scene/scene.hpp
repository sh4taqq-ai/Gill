#pragma once
#include <vector>
#include "component/entity.hpp"
#include "component/transform.hpp"
#include "component/mesh.hpp"
#include "component/sunlight.hpp"
#include <functional>
#include "core/set/sparseset.hpp"
#include "core/system/asset/asset.hpp"
#include <optional>
class Scene {
public:

    Scene() = default;
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









    AssetID LoadMesh(Mesh mesh) { return meshManager.Load(std::move(mesh)); }
    AssetID LoadShader(Shader shader) { return shaderManager.Load(std::move(shader)); }

    const Mesh* GetMesh(AssetID id) const;
    const Shader* GetShader(AssetID id) const;



    std::optional<Entity> GetSelected() const;
    void SetSelected(const std::optional<Entity>& value);

    std::vector<Entity> GetLivingEntities() const;

    uint32_t GetEntityCount();


    private:
    std::optional<Entity> selected;
    EntityManager entityManager;
    AssetManager<Mesh> meshManager;
    AssetManager<Shader> shaderManager;
    SparseSet<MeshComponent> meshes;
    SparseSet<SunlightComponent> sunlights;
};




    template<>
    inline SunlightComponent& Scene::GetComponent<SunlightComponent>(Entity entity) {
        return sunlights.Get(entity);
    }
    template<>
    inline const SunlightComponent& Scene::GetComponent<SunlightComponent>(Entity entity) const {
        return sunlights.Get(entity);
    }
    template<>
    inline MeshComponent& Scene::GetComponent<MeshComponent>(Entity entity) {
        return meshes.Get(entity);
    }
    template<>
    inline const MeshComponent& Scene::GetComponent<MeshComponent>(Entity entity) const {
        return meshes.Get(entity);
    }

    template<>
    inline MeshComponent* Scene::TryGetComponent<MeshComponent>(Entity entity) {
        if (meshes.Has(entity)) {
            return &meshes.Get(entity);
        }
        return nullptr;
    }
    template<>
    inline const MeshComponent* Scene::TryGetComponent<MeshComponent>(Entity entity) const {
        if (meshes.Has(entity)) {
            return &meshes.Get(entity);
        }
        return nullptr;
    }
    template<>
    inline SunlightComponent* Scene::TryGetComponent<SunlightComponent>(Entity entity) {
        if (sunlights.Has(entity)) {
            return &sunlights.Get(entity);
        }
        return nullptr;
    }
    template<>
    inline const SunlightComponent* Scene::TryGetComponent<SunlightComponent>(Entity entity) const {
        if (sunlights.Has(entity)) {
            return &sunlights.Get(entity);
        }
        return nullptr;
    }
    template<>
    inline void Scene::InsertComponent<MeshComponent>(Entity entity,const MeshComponent& component) {
        meshes.Insert(entity, component);
    }
    template<>
    inline void Scene::RemoveComponent<MeshComponent>(Entity entity) {
        meshes.Remove(entity);
    }
    template<>
    inline void Scene::InsertComponent<SunlightComponent>(Entity entity,const SunlightComponent& component) {
        sunlights.Insert(entity, component);
    }
    template<>
    inline void Scene::RemoveComponent<SunlightComponent>(Entity entity) {
        sunlights.Remove(entity);
    }

    template<>
    inline void Scene::ForEach<MeshComponent>(std::function<void(Entity entity,const  MeshComponent& component)> func) const{
        for (size_t it = 0; it<meshes.Size();it++) {
            func(meshes.GetEntity(it), meshes[it]);
        }
    }

    template<>
    inline void Scene::ForEach<SunlightComponent>(std::function<void(Entity entity, const SunlightComponent& component)> func) const {
        for (size_t it = 0; it<sunlights.Size();it++) {
            func(sunlights.GetEntity(it), sunlights[it]);
        }
    }