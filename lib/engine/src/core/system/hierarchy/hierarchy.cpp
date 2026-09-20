#include "core/system/hierarchy/hierarchy.hpp"

void Hierarchy::SetParent(Entity child, Entity parent) {
    comp::ParentComponent parentComponent;
    parentComponent.SetParent(parent);
    if (child == parent || IsDescendant(parent, child)) {
        return;
    }

    if (m_parents.Has(child)) {
        comp::ParentComponent& oldParentComponent = m_parents.Get(child);
        RemoveChild(oldParentComponent.parentEntity, child);
    }

    m_parents.Insert(child, parentComponent);
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
    if (!m_children.Has(parent)) {
        comp::ChildComponent childComp;
        childComp.AddChild(child);
        m_children.Insert(parent,childComp);
    }
    else {
        comp::ChildComponent& childe = m_children.Get(parent);
        childe.AddChild(child);
    }
}

std::optional<Entity> Hierarchy::TryGetParent(Entity child) {
    if (m_parents.Has(child)) {
        return m_parents.Get(child).parentEntity;
    }
    else {
        return std::nullopt;
    }
}

std::vector<Entity> Hierarchy::GetChild(Entity parent) {
    if (m_children.Has(parent)) {
        return m_children.Get(parent).children;
    }
    return {};
}

void Hierarchy::RemoveChild(Entity parent, Entity child) {
    if (m_children.Has(parent)) {
        comp::ChildComponent& childComponent = m_children.Get(parent);
        childComponent.RemoveChild(child);
        RemoveParent(child);
        if (childComponent.IsEmpty()) {
            m_children.Remove(parent);
        }
    }
}

void Hierarchy::RemoveAllChildren(Entity parent) {
    if (m_children.Has(parent)) {
        comp::ChildComponent& childComponent = m_children.Get(parent);
        for (auto child : childComponent.children) {
            RemoveParent(child);
        }
        childComponent.RemoveAllChildren();
        m_children.Remove(parent);
    }
    //same with here
}

void Hierarchy::RemoveParent(Entity child) {
    if (m_parents.Has(child)) {
        m_parents.Remove(child);
    }
}


Hierarchy::~Hierarchy() = default;


void Hierarchy::Unparent(Entity child) {
    if (m_parents.Has(child)) {
        Entity oldParent = m_parents.Get(child).parentEntity;
        RemoveChild(oldParent, child); // already handles children list + parents.Remove internally
    }
}