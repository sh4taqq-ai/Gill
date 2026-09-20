#include "editor/grid/grid.hpp"
#include "glad/gl.h"
#include "render/mesh/primitive.hpp"
#include "render/shader/shader.hpp"

void GridRenderer::Init(float extent) {
    up_gridMesh = CreateGridPlane(extent);
    m_ext = extent;
    up_gridShader = std::make_unique<Shader>(
        "asset/shader/gridShader/gridVert.glsl",
        "asset/shader/gridShader/gridFrag.glsl"
    );
}

void GridRenderer::Render(const mathpp::mat4f& view, const mathpp::mat4f& projection, const mathpp::vec3f& camPos) {
    up_gridShader->Use();
    up_gridShader->setVec3f("cameraPos", camPos);
    up_gridShader->setMat4f("view", view);
    up_gridShader->setMat4f("projection", projection);
    up_gridShader->setFloat("cellSize", m_cellSize);
    up_gridShader->setFloat("extent", m_ext);



    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    up_gridMesh->Draw();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}