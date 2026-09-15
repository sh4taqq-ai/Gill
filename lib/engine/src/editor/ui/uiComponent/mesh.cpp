#include "imgui.h"
#include "magic_enum.hpp"
#include "editor/registry/registry.hpp"
#include "scene/scene.hpp"
#include "render/mesh/primitive.hpp"
#include "editor/ui/uiComponent/mesh.hpp"

void AddMesh(Scene* s, Entity e,EditorContext& ctx) {
    s->InsertComponent(e,comp::MeshComponent{});
}

void RemoveMesh(Scene* s, Entity e) {
    s->RemoveComponent<comp::MeshComponent>(e);
}
bool HasMesh(Scene* s, Entity e) {
   return s->TryGetComponent<comp::MeshComponent>(e) != nullptr;
}

void DrawMeshInspector(Scene* s, Entity e,EditorContext& ctx) {
    auto* mesh = s->TryGetComponent<comp::MeshComponent>(e);
    if (mesh->meshID == AssetHandle{}) {
        constexpr auto count = magic_enum::enum_count<PrimitiveType>();
        constexpr auto names = magic_enum::enum_names<PrimitiveType>();
        constexpr auto values = magic_enum::enum_values<PrimitiveType>();

        if (ImGui::BeginListBox("Meshes"))
        {
            for (size_t  i = 0; i <count  ; i++)
            {
                if (ImGui::Selectable(names[i].data())) {
                    mesh->meshID= ctx.meshSystem->AddPrimitive(values[i]);
                }
            }
            ImGui::EndListBox();

        }
    }
    else {
        {ImGui::Text("Mesh ID: %u", mesh->meshID.index);}
    }
}