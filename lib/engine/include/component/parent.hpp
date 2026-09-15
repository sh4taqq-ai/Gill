#pragma once
#include <cstdint>

using Entity = uint32_t;

namespace comp {
    struct ParentComponent {
        Entity parentEntity;

        void SetParent(Entity parentEntity) {
            this->parentEntity = parentEntity;
        }
    };
}