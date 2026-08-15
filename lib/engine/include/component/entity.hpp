#pragma once
#include <cstdint>
#include <assert.h>
#include <array>
#include <queue>
#include <memory>

using Entity = uint32_t;

const Entity MAX_ENTITIES = 5000;

class EntityManager
{
public:
    EntityManager();


    Entity CreateEntity();


    void DestroyEntity(Entity entity);



private:
    // Queue of unused entity IDs
    std::queue<Entity> mAvailableEntities{};

    // Total living entities - used to keep limits on how many exist
    uint32_t mLivingEntityCount{};
};
