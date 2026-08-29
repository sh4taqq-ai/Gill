#include "editor/grid/grid.hpp"
#include "glad/gl.h"
#include "render/mesh/primitive.hpp"
#include "render/shader/shader.hpp"

void GridRenderer::Init(float extent) {
    gridMesh = CreateGridPlane(extent);
    ext = extent;
    gridShader = std::make_unique<Shader>(
        "asset/shader/gridShader/gridVert.glsl",
        "asset/shader/gridShader/gridFrag.glsl"
    );
}

void GridRenderer::Render(const mathpp::mat4f& view, const mathpp::mat4f& projection, const mathpp::vec3f& camPos) {
    gridShader->Use();
    gridShader->setVec3f("cameraPos", camPos);
    gridShader->setMat4f("view", view);
    gridShader->setMat4f("projection", projection);
    gridShader->setFloat("cellSize", cellSize);
    gridShader->setFloat("extent", ext);



    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    gridMesh->Draw();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}