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
        if (!q_AvailableIndices.empty()) {
            index = q_AvailableIndices.front();
            q_AvailableIndices.pop();
            v_Slots[index].asset = std::move(asset);
            v_Slots[index].alive = true;
        } else {
            index = static_cast<uint32_t>(v_Slots.size());
            v_Slots.push_back(Slot{std::move(asset), 0, true});
        }
        return AssetHandle{index, v_Slots[index].generation};
    }

    T* Get(AssetHandle h)             { return IsValid(h) ? &v_Slots[h.index].asset : nullptr; }
    const T* Get(AssetHandle h) const { return IsValid(h) ? &v_Slots[h.index].asset : nullptr; }

    void Unload(AssetHandle h) {
        if (!IsValid(h)) return;
        v_Slots[h.index].alive = false;
        v_Slots[h.index].generation++;
        q_AvailableIndices.push(h.index);
    }

    bool Has(AssetHandle h) const { return IsValid(h); }

private:
    struct Slot { T asset; uint32_t generation = 0; bool alive = false; };

    bool IsValid(AssetHandle h) const {
        return h.index < v_Slots.size()
            && v_Slots[h.index].alive
            && v_Slots[h.index].generation == h.generation;
    }

    std::vector<Slot> v_Slots;
    std::queue<uint32_t> q_AvailableIndices;
};