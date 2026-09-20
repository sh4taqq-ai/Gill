#include "core/system/mesh/mesh.hpp"
#include "scene/scene.hpp"

const Mesh *MeshSystem::GetMesh(AssetHandle id) const {
        return m_meshManager.Get(id);
}

bool MeshSystem::HasMesh(AssetHandle id) const {
        return m_meshManager.Has(id);
}

AssetHandle MeshSystem::RegisterMesh(Mesh &&mesh) {
        return m_meshManager.Load(std::move(mesh));
}


AssetHandle MeshSystem::AddPrimitive(PrimitiveType type,unsigned int rings, unsigned int segments, float radius, float height) {
        Mesh mesh = [&] {

                switch (type) {
                        case PrimitiveType::Cube:   return CreateCube();
                        case PrimitiveType::Sphere: return CreateSphere(rings, segments, radius);
                        case PrimitiveType::Plane:  return CreatePlane();
                        case PrimitiveType::Cylinder: return CreateCylinder(segments,radius,height);
                        case PrimitiveType::Cone : return CreateCone(segments,radius,height);
                }
        }();


        AssetHandle meshID = RegisterMesh(std::move(mesh));
        return meshID;


}

MeshSystem::~MeshSystem() = default;

void MeshSystem::RemoveMesh(AssetHandle id) {
        m_meshManager.Unload(id);
}
