#pragma once
#include <cstdint>
struct AssetHandle {
    uint32_t index = UINT32_MAX;
    uint32_t generation = 0;

    bool operator==(const AssetHandle&) const = default;
};

