#pragma once
#include "core/system/asset/asset.hpp"
#include "render/mesh/mesh.hpp"
#include "render/mesh/primitive.hpp"
#include <string>
class MeshSystem {
public:
    ~MeshSystem();
    const Mesh* GetMesh(AssetHandle id) const;
    bool HasMesh(AssetHandle id)const;
    AssetHandle RegisterMesh(Mesh&& mesh);
    void RemoveMesh(AssetHandle id);
    AssetHandle ImportMeshFile(const std::string& path);
    AssetHandle AddPrimitive(PrimitiveType type,unsigned int rings = 32,unsigned int segments = 16,float radius = 1.0f, float height = 1.0f);

private:

     Mesh LoadMeshFromFile(const std::string& path);

    AssetManager<Mesh> m_meshManager;
};