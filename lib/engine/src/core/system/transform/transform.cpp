#include "core/system/transform/transform.hpp"

#include "core/system/hierarchy/hierarchy.hpp"


void TransformSystem::Init(Hierarchy *hierarchy) {
    p_hierarchy = hierarchy;
}
void TransformSystem::SetTransform(Entity entity, const mathpp::vec3f& pos, const mathpp::quatf& rot, const mathpp::vec3f& scale) {
    auto& t = m_localTransforms.Get(entity);
    t.position = pos; t.rotation = rot; t.scale = scale;
    MarkDirty(entity); // once
}

void TransformSystem::CalculateWorldTransform(Entity entity) {
    auto& wt = m_worldTransforms.Get(entity);
    if (!wt.dirty) return; // already correct, nothing to do

    auto& lt = m_localTransforms.Get(entity);
    mathpp::mat4f parentWorld; // identity if no parent
    mathpp::quatf parentRotation;

    if (auto parent = p_hierarchy->TryGetParent(entity)) {
        CalculateWorldTransform(*parent);
        parentWorld = m_worldTransforms.Get(*parent).world;
        parentRotation = m_worldTransforms.Get(*parent).rotation;
    }

    wt.world = parentWorld * lt.getMatrix();
    wt.rotation = parentRotation * lt.rotation;
    wt.dirty = false;
}

void TransformSystem::MarkDirty(Entity entity) {
    auto& wt = m_worldTransforms.Get(entity);
    if (wt.dirty) {
        return;
    }
    wt.dirty = true;
    for (Entity child : p_hierarchy->GetChild(entity)) {
        MarkDirty(child);
    }
}

const mathpp::mat4f& TransformSystem::GetWorldTransform(Entity entity) {
    CalculateWorldTransform(entity);
    return m_worldTransforms.Get(entity).world;
}


void TransformSystem::AddTransform(Entity entity) {
    comp::TransformComponent transformComponent;
    comp::WorldTransformComponent worldTransformComponent;
    m_localTransforms.Insert(entity,transformComponent);
    m_worldTransforms.Insert(entity,worldTransformComponent);

}

const comp::TransformComponent &TransformSystem::GetTransform(Entity entity) const {
    return m_localTransforms.Get(entity);
}

void TransformSystem::RemoveTransform(Entity entity) {
    m_localTransforms.Remove(entity);
    m_worldTransforms.Remove(entity);
}

void TransformSystem::SetPosition(Entity entity, const mathpp::vec3f& position) {
    m_localTransforms.Get(entity).position = position;
    MarkDirty(entity);
}

void TransformSystem::SetRotation(Entity entity, const mathpp::quatf& rotation) {
    m_localTransforms.Get(entity).rotation = rotation;
    MarkDirty(entity);
}

void TransformSystem::SetScale(Entity entity, const mathpp::vec3f& scale) {
    m_localTransforms.Get(entity).scale = scale;
    MarkDirty(entity);
}

TransformSystem::~TransformSystem() = default;

const mathpp::quatf& TransformSystem::GetWorldRotation(Entity entity) {
    CalculateWorldTransform(entity);
    return m_worldTransforms.Get(entity).rotation;
}

mathpp::mat4f TransformSystem::GetParentWorldTransform(Entity entity) {
    if (auto parent = p_hierarchy->TryGetParent(entity)) {
        return GetWorldTransform(*parent);
    }
    return mathpp::mat4f{}; // identity
}

mathpp::quatf TransformSystem::GetParentWorldRotation(Entity entity) {
    if (auto parent = p_hierarchy->TryGetParent(entity)) {
        return GetWorldRotation(*parent);
    }
    return mathpp::quatf{};
}