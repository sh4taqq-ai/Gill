#include "glad/gl.h"
#include "editor/outline/outline.hpp"
#include "core/system/mesh/mesh.hpp"
#include "core/system/transform/transform.hpp"
#include "editor/selector/selectionManager.hpp"
#include "render/shader/shader.hpp"



void Outline::Init(MeshSystem* meshSystem,TransformSystem* transformSystem,SelectionManager* selectionManager) {
    up_outlineShader = std::make_unique<Shader>("asset/shader/outlineShader/outlineVert.glsl","asset/shader/outlineShader/outlineFrag.glsl");
    p_meshSystem = meshSystem;
    p_transformSystem = transformSystem;
    p_selectionManager = selectionManager;
}

void Outline::Draw(const Scene* scene,const mathpp::mat4f& proj, const mathpp::mat4f& view) const {
    up_outlineShader->Use();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    scene->ForEach<comp::MeshComponent>([this,scene,&view,&proj](Entity entity,const comp::MeshComponent& meshComp) {
        DrawEntity(scene,proj,view,entity,meshComp);
    });
    glCullFace(GL_BACK);
}

void Outline::DrawEntity(const Scene* scene,const mathpp::mat4f &proj, const mathpp::mat4f &view,Entity entity,const comp::MeshComponent& meshComp) const {
    auto selected = p_selectionManager->GetAllSelected();
    if (!selected.contains(entity)) return; //safeguard
    if (entity != p_selectionManager->GetActiveSelected()) return;
    auto mesh = p_meshSystem->GetMesh(meshComp.meshID);
    auto worldTransform = p_transformSystem->GetWorldTransform(entity);
    if (!mesh) return;

    up_outlineShader->setMat4f("model",worldTransform);
    up_outlineShader->setMat4f("view",view);
    up_outlineShader->setMat4f("proj",proj);
    mesh->Draw();
}

Outline::~Outline() = default;