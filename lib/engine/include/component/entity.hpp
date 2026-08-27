#pragma once
#include <cstdint>
#include <cassert>
#include <array>
#include <queue>
#include <memory>
#include <algorithm>

using Entity = uint32_t;

const Entity MAX_ENTITIES = 5000;

class EntityManager
{
public:
    EntityManager();


    Entity CreateEntity();

    uint32_t GetEntityCount();

    void DestroyEntity(Entity entity);

    std::vector<Entity> GetLivingEntity() const;


private:
    // Queue of unused entity IDs
    std::queue<Entity> mAvailableEntities{};
    //living entity
    std::vector<Entity> mEntities;

    // Total living entities - used to keep limits on how many exist
    uint32_t mLivingEntityCount{};
};
