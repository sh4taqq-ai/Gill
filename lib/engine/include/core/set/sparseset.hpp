#pragma once

#include <vector>
#include <cstdint>
#include <cassert>
#include <utility>

template<typename T>
class SparseSet
{
public:
    using Entity = uint32_t;
    using Index  = size_t;

public:
    void Insert(Entity entity, const T& component)
    {
        assert(!Has(entity) && "Entity already has this component");

        if (entity >= v_sparse.size())
            v_sparse.resize(entity + 1, INVALID_INDEX);

        Index index = v_components.size();

        v_entities.push_back(entity);
        v_components.push_back(component);

        v_sparse[entity] = index;
    }

    void Insert(Entity entity, T&& component)
    {
        assert(!Has(entity) && "Entity already has this component");

        if (entity >= v_sparse.size())
            v_sparse.resize(entity + 1, INVALID_INDEX);

        Index index = v_components.size();

        v_entities.push_back(entity);
        v_components.push_back(std::move(component));

        v_sparse[entity] = index;
    }

    bool Has(Entity entity) const
    {
        if (entity >= v_sparse.size())
            return false;

        Index index = v_sparse[entity];

        return index != INVALID_INDEX &&
               index < v_entities.size() &&
               v_entities[index] == entity;
    }

    T& Get(Entity entity)
    {
        assert(Has(entity) && "Entity does not have this component");

        return v_components[v_sparse[entity]];
    }

    const T& Get(Entity entity) const
    {
        assert(Has(entity) && "Entity does not have this component");

        return v_components[v_sparse[entity]];
    }

    void Remove(Entity entity)
    {
        assert(Has(entity) && "Entity does not have this component");

        Index index = v_sparse[entity];
        Index last  = v_components.size() - 1;

        // If we're not removing the last element,
        // move the last element into the removed element's position.
        if (index != last)
        {
            v_components[index] = std::move(v_components[last]);

            Entity movedEntity = v_entities[last];
            v_entities[index] = movedEntity;

            v_sparse[movedEntity] = index;
        }

        v_components.pop_back();
        v_entities.pop_back();

        v_sparse[entity] = INVALID_INDEX;
    }

    size_t Size() const
    {
        return v_components.size();
    }

    void Clear()
    {
        v_components.clear();
        v_entities.clear();
        v_sparse.clear();
    }

    // Dense iteration
    T& operator[](size_t index)
    {
        assert(index < v_components.size());
        return v_components[index];
    }

    const T& operator[](size_t index) const
    {
        assert(index < v_components.size());
        return v_components[index];
    }

    Entity GetEntity(size_t index) const
    {
        assert(index < v_entities.size());
        return v_entities[index];
    }

    size_t EntitySize() const {
        return v_entities.size();
    }

private:
    static constexpr Index INVALID_INDEX = static_cast<Index>(-1);

    // Entity -> dense index
    std::vector<Index> v_sparse;

    // Dense index -> Entity
    std::vector<Entity> v_entities;

    // Dense index -> Component
    std::vector<T> v_components;
};