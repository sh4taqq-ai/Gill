#pragma once
#include "core/set/sparseset.hpp"
#include "component/parent.hpp"
#include "component/children.hpp"
#include <optional>


class Hierarchy {
    public:
    ~Hierarchy();
    void SetParent(Entity child, Entity parent);
    void InsertChild(Entity parent, Entity child);
    void RemoveChild( Entity parent,Entity child);
    void RemoveAllChildren(Entity parent);
    void RemoveParent(Entity child);
  bool  IsDescendant(Entity potentialDescendant, Entity of);
   std::optional<Entity>TryGetParent(Entity child);
    std::optional<Entity> TryGetChild(Entity child);
    std::vector<Entity> GetChild(Entity parent);

   void Unparent(Entity child);



private :
    SparseSet<ParentComponent> parents;
    SparseSet<ChildComponent> children;
};
