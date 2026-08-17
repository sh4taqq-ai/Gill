#pragma once
#include <unordered_map>
#include "../component/entity.hpp"
#include "../component/transform.hpp"
#include "../component/mesh.hpp"
#include "../core/component/raycast/raycast.hpp"
#include <optional>
class Scene {
public:

    Scene();
    ~Scene();
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
        }
        return nullptr;
    }//the function have to be a const to be able to call the function on a const object

    std::unordered_map<Entity, MeshComponent> GetAllMeshes() const;
    std::unordered_map<Entity, TransformComponent> GetAllTransforms() const;

    void AddTransform(Entity entity, const TransformComponent& transform);
    void AddMesh(Entity entity, const MeshComponent& mesh);

    std::optional<Entity> SelectEntity(const Ray&  ray,float radius) const;

    private:
    EntityManager entityManager;
    std::unordered_map<Entity, TransformComponent> transforms;
    std::unordered_map<Entity, MeshComponent> meshes;
};