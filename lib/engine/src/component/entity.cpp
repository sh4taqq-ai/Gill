#include "component/entity.hpp"

#include <memory>

EntityManager::EntityManager()
{
    // Initialize the queue with all possible entity IDs
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        mAvailableEntities.push(entity);
    }
}

Entity EntityManager::CreateEntity()   {
    assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

    // Take an ID from the front of the queue
    Entity id = mAvailableEntities.front();
    mAvailableEntities.pop();
    ++mLivingEntityCount;

    return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");


    // Put the destroyed ID at the back of the queue
    mAvailableEntities.push(entity);
    --mLivingEntityCount;
}


