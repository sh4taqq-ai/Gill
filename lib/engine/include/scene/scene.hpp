#pragma once
#include <unordered_map>
#include "../component/entity.hpp"
#include "../component/transform.hpp"
#include "../component/mesh.hpp"
#include "../component/sunlight.hpp"
#include "../core/component/raycast/raycast.hpp"
#include "../include/core/system/asset/asset.hpp"
#include <optional>
class Scene {
public:


    Entity CreateEntity();
    void DestroyEntity(Entity entity);



    template<typename T>
   const T* GetComponent(Entity entity) const {
        if constexpr (std::is_same_v<T, TransformComponent>) {
            auto it = transforms.find(entity);
            return it != transforms.end() ? &it->second : nullptr;
        } else if constexpr (std::is_same_v<T, MeshComponent>) {
            auto it = meshes.find(entity);
            return it != meshes.end() ? &it->second : nullptr;
        } else if constexpr (std::is_same_v<T, SunlightComponent>) {
            auto it = sunlights.find(entity);
            return it != sunlights.end() ? &it->second : nullptr;
        }

        return nullptr;
    }//the function have to be a const to be able to call the function on a const object

    template<typename T>
T* GetComponent(Entity entity) {
        if constexpr (std::is_same_v<T, TransformComponent>) {
            auto it = transforms.find(entity);
            return it != transforms.end() ? &it->second : nullptr;
        }
        else if constexpr (std::is_same_v<T, MeshComponent>) {
            auto it = meshes.find(entity);
            return it != meshes.end() ? &it->second : nullptr;
        } else if constexpr (std::is_same_v<T, SunlightComponent>) {
            auto it = sunlights.find(entity);
            return it != sunlights.end() ? &it->second : nullptr;
        }

        return nullptr;


    }


    const std::unordered_map<Entity, MeshComponent>& GetAllMeshes() const;
    const std::unordered_map<Entity, TransformComponent>& GetAllTransforms() const;
    const std::unordered_map<Entity, SunlightComponent>& GetAllSunlights() const;

    void AddTransform(Entity entity, const TransformComponent& transform);
    void AddMesh(Entity entity, const MeshComponent& mesh);
    void AddSunlight(Entity entity, const SunlightComponent& sun);

    AssetID LoadMesh(Mesh mesh) { return meshManager.Load(std::move(mesh)); }
    AssetID LoadShader(Shader shader) { return shaderManager.Load(std::move(shader)); }

    const Mesh* GetMesh(AssetID id) const;
    const Shader* GetShader(AssetID id) const;

    std::optional<Entity> SelectEntity(const Ray&  ray,float radius) const;


    std::optional<Entity> GetSelected() const;
    void SetSelected(const std::optional<Entity>& value);



    private:
    std::optional<Entity> selected;
    EntityManager entityManager;
    AssetManager<Mesh> meshManager;
    AssetManager<Shader> shaderManager;
    std::unordered_map<Entity, TransformComponent> transforms;
    std::unordered_map<Entity, MeshComponent> meshes;
    std::unordered_map<Entity, SunlightComponent> sunlights;

};