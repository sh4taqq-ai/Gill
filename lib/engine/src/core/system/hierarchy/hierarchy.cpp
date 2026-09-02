#include "core/system/hierarchy/hierarchy.hpp"

void Hierarchy::SetParent(Entity child, Entity parent) {
    ParentComponent parentComponent;
    parentComponent.SetParent(parent);
    if (child == parent || IsDescendant(parent, child)) {
        return;
    }

    if (parents.Has(child)) {
        ParentComponent& oldParentComponent = parents.Get(child);
        RemoveChild(oldParentComponent.parentEntity, child);
    }

    parents.Insert(child, parentComponent);
    InsertChild(parent, child);
}

bool Hierarchy::IsDescendant(Entity potentialDescendant, Entity of) {
    std::optional<Entity> current = TryGetParent(potentialDescendant);
    while (current.has_value()) {
        if (*current == of) return true;
        current = TryGetParent(*current);
    }
    return false;
}


void Hierarchy::InsertChild(Entity parent, Entity child) {
    if (!children.Has(parent)) {
        ChildComponent childComp;
        childComp.AddChild(child);
        children.Insert(parent,childComp);
    }
    else {
        ChildComponent& childe = children.Get(parent);
        childe.AddChild(child);
    }
}

std::optional<Entity> Hierarchy::TryGetParent(Entity child) {
    if (parents.Has(child)) {
        return parents.Get(child).parentEntity;
    }
    else {
        return std::nullopt;
    }
}

std::vector<Entity> Hierarchy::GetChild(Entity parent) {
    if (children.Has(parent)) {
        return children.Get(parent).children;
    }
    return {};
}

void Hierarchy::RemoveChild(Entity parent, Entity child) {
    if (children.Has(parent)) {
        ChildComponent& childComponent = children.Get(parent);
        childComponent.RemoveChild(child);
        RemoveParent(child);
        if (childComponent.IsEmpty()) {
            children.Remove(parent);
        }
    }
}

void Hierarchy::RemoveAllChildren(Entity parent) {
    if (children.Has(parent)) {
        ChildComponent& childComponent = children.Get(parent);
        for (auto child : childComponent.children) {
            RemoveParent(child);
        }
        childComponent.RemoveAllChildren();
        children.Remove(parent);
    }
    //same with here
}

void Hierarchy::RemoveParent(Entity child) {
    if (parents.Has(child)) {
        parents.Remove(child);
    }
}


Hierarchy::~Hierarchy() = default;


void Hierarchy::Unparent(Entity child) {
    if (parents.Has(child)) {
        Entity oldParent = parents.Get(child).parentEntity;
        RemoveChild(oldParent, child); // already handles children list + parents.Remove internally
    }
}