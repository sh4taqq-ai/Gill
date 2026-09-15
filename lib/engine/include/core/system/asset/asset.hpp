#pragma once
#include <vector>
#include <queue>
#include <cassert>
#include <cstdint>
#include "assetID.hpp"



template<typename T>
class AssetManager {
public:
    AssetHandle Load(T asset) {
        uint32_t index;
        if (!mAvailableIndices.empty()) {
            index = mAvailableIndices.front();
            mAvailableIndices.pop();
            mSlots[index].asset = std::move(asset);
            mSlots[index].alive = true;
        } else {
            index = static_cast<uint32_t>(mSlots.size());
            mSlots.push_back(Slot{std::move(asset), 0, true});
        }
        return AssetHandle{index, mSlots[index].generation};
    }

    T* Get(AssetHandle h)             { return IsValid(h) ? &mSlots[h.index].asset : nullptr; }
    const T* Get(AssetHandle h) const { return IsValid(h) ? &mSlots[h.index].asset : nullptr; }

    void Unload(AssetHandle h) {
        if (!IsValid(h)) return;
        mSlots[h.index].alive = false;
        mSlots[h.index].generation++;
        mAvailableIndices.push(h.index);
    }

    bool Has(AssetHandle h) const { return IsValid(h); }

private:
    struct Slot { T asset; uint32_t generation = 0; bool alive = false; };

    bool IsValid(AssetHandle h) const {
        return h.index < mSlots.size()
            && mSlots[h.index].alive
            && mSlots[h.index].generation == h.generation;
    }

    std::vector<Slot> mSlots;
    std::queue<uint32_t> mAvailableIndices;
};