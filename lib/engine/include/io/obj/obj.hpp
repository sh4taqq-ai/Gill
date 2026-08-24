#pragma once
#include <functional>
#include <string>
#include <utility>

#include "render/mesh/mesh.hpp"

struct ObjIndexKey {
    int posIdx, uvIdx, normalIdx;
    bool operator==(const ObjIndexKey& other) const {
        return posIdx == other.posIdx && uvIdx == other.uvIdx && normalIdx == other.normalIdx;
    }
};

struct ObjIndexKeyHash {
    size_t operator()(const ObjIndexKey& k) const {
        return std::hash<int>()(k.posIdx) ^ (std::hash<int>()(k.uvIdx) << 1) ^ (std::hash<int>()(k.normalIdx) << 2);
    }
};

Mesh LoadOBJ(const std::string& path);