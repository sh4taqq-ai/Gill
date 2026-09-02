#include "core/system/transform/transform.hpp"

#include "core/system/hierarchy/hierarchy.hpp"


void TransformSystem::Init(Hierarchy *hierarchy) {
    _hierarchy = hierarchy;
}
void TransformSystem::SetTransform(Entity entity, const mathpp::vec3f& pos, const mathpp::quatf& rot, const mathpp::vec3f& scale) {
    auto& t = localTransforms.Get(entity);
    t.position = pos; t.rotation = rot; t.scale = scale;
    MarkDirty(entity); // once
}

void TransformSystem::CalculateWorldTransform(Entity entity) {
    auto& wt = worldTransforms.Get(entity);
    if (!wt.dirty) return; // already correct, nothing to do

    auto& lt = localTransforms.Get(entity);
    mathpp::mat4f parentWorld; // identity if no parent
    mathpp::quatf parentRotation;

    if (auto parent = _hierarchy->TryGetParent(entity)) {
        CalculateWorldTransform(*parent);
        parentWorld = worldTransforms.Get(*parent).world;
        parentRotation = worldTransforms.Get(*parent).rotation;
    }

    wt.world = parentWorld * lt.getMatrix();
    wt.rotation = parentRotation * lt.rotation;
    wt.dirty = false;
}

void TransformSystem::MarkDirty(Entity entity) {
    auto& wt = worldTransforms.Get(entity);
    if (wt.dirty) {
        return;
    }
    wt.dirty = true;
    for (Entity child : _hierarchy->GetChild(entity)) {
        MarkDirty(child);
    }
}

const mathpp::mat4f& TransformSystem::GetWorldTransform(Entity entity) {
    CalculateWorldTransform(entity);
    return worldTransforms.Get(entity).world;
}


void TransformSystem::AddTransform(Entity entity) {
    TransformComponent transformComponent;
    WorldTransformComponent worldTransformComponent;
    localTransforms.Insert(entity,transformComponent);
    worldTransforms.Insert(entity,worldTransformComponent);

}

const TransformComponent &TransformSystem::GetTransform(Entity entity) const {
    return localTransforms.Get(entity);
}

void TransformSystem::RemoveTransform(Entity entity) {
    localTransforms.Remove(entity);
    worldTransforms.Remove(entity);
}

void TransformSystem::SetPosition(Entity entity, const mathpp::vec3f& position) {
    localTransforms.Get(entity).position = position;
    MarkDirty(entity);
}

void TransformSystem::SetRotation(Entity entity, const mathpp::quatf& rotation) {
    localTransforms.Get(entity).rotation = rotation;
    MarkDirty(entity);
}

void TransformSystem::SetScale(Entity entity, const mathpp::vec3f& scale) {
    localTransforms.Get(entity).scale = scale;
    MarkDirty(entity);
}

TransformSystem::~TransformSystem() = default;

const mathpp::quatf& TransformSystem::GetWorldRotation(Entity entity) {
    CalculateWorldTransform(entity);
    return worldTransforms.Get(entity).rotation;
}