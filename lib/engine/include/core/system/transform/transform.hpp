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
    const comp::TransformComponent& GetTransform(Entity entity) const;
    const mathpp::mat4f& GetWorldTransform(Entity entity);
    mathpp::mat4f GetParentWorldTransform(Entity entity);
    mathpp::quatf GetParentWorldRotation(Entity entity);
    const mathpp::quatf& GetWorldRotation(Entity entity);
    void SetTransform(Entity entity, const mathpp::vec3f& pos, const mathpp::quatf& rot, const mathpp::vec3f& scale);
    void SetPosition(Entity entity, const mathpp::vec3f& pos);
    void SetRotation(Entity entity, const mathpp::quatf& rot);
    void SetScale(Entity entity, const mathpp::vec3f& scale);
    void MarkDirty(Entity entity);



private:

    void CalculateWorldTransform( Entity entity);
    Hierarchy* p_hierarchy;
    SparseSet<comp::TransformComponent> m_localTransforms;
    SparseSet<comp::WorldTransformComponent> m_worldTransforms;

};