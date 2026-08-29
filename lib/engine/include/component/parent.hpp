#pragma once
#include <cstdint>


using Entity = uint32_t;

struct ParentComponent {
    Entity parentEntity;

    void SetParent(Entity parentEntity) {
        this->parentEntity = parentEntity;
    }
};