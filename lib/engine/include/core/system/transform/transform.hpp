#pragma once
#include <cstdint>
#include "mathpp.hpp"
#include "component/transform.hpp"
#include "component/worldTransform.hpp"
#include "core/set/sparseset.hpp"

using Entity = uint32_t;
class Hierarchy;


class TransformSystem {
public:
    ~TransformSystem();
    void Init(Hierarchy* hierarchy);
    void AddTransform(Entity entity);
    void RemoveTransform(Entity entity);
    const TransformComponent& GetTransform(Entity entity) const;
    const mathpp::mat4f& GetWorldTransform(Entity entity);
    const mathpp::quatf& GetWorldRotation(Entity entity);
    void SetTransform(Entity entity, const mathpp::vec3f& pos, const mathpp::quatf& rot, const mathpp::vec3f& scale);
    void SetPosition(Entity entity, const mathpp::vec3f& pos);
    void SetRotation(Entity entity, const mathpp::quatf& rot);
    void SetScale(Entity entity, const mathpp::vec3f& scale);



private:
    void MarkDirty(Entity entity);
    void CalculateWorldTransform( Entity entity);
    Hierarchy* _hierarchy;
    SparseSet<TransformComponent> localTransforms;
    SparseSet<WorldTransformComponent> worldTransforms;

};