#pragma once
#include <vector>
#include <queue>
#include <cassert>
#include <cstdint>

using AssetID = uint32_t;

template<typename T>
class AssetManager {
public:
    AssetID Load(T asset) {
        AssetID id;
        if (!mAvailableIDs.empty()) {
            id = mAvailableIDs.front();
            mAvailableIDs.pop();
            mAssets[id] = std::move(asset);
        } else {
            id = static_cast<AssetID>(mAssets.size());
            mAssets.push_back(std::move(asset));
        }
        return id;
    }

    T* Get(AssetID id) {
        assert(id < mAssets.size() && "AssetID out of range.");
        return &mAssets[id];
    }

    const T* Get(AssetID id) const {
        assert(id < mAssets.size() && "AssetID out of range.");
        return &mAssets[id];
    }


    void Unload(AssetID id) {
        assert(id < mAssets.size() && "AssetID out of range.");
        mAvailableIDs.push(id);
    }

private:
    std::vector<T> mAssets;
    std::queue<AssetID> mAvailableIDs;
};