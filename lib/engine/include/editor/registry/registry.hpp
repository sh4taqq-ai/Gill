#pragma once
#include <functional>
#include <vector>
#include <cstdint>
#include "core/system/mesh/mesh.hpp"
#include "core/system/material/material.hpp"
class Scene;

using Entity = uint32_t;

struct EditorContext{
    MeshSystem* meshSystem;
    MaterialSystem* materialSystem;
};

struct ComponentTypeInfo {
    const char* name;
    void (*Add)(Scene* s, Entity e,EditorContext&);
    void (*Remove)(Scene* s, Entity e);
    void (*DrawInspector)(Scene* s, Entity e,EditorContext&);
    bool (*Has)(Scene* s, Entity e);
};

const std::vector<ComponentTypeInfo>& GetComponentRegistry();