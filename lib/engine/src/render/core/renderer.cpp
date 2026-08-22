#include "../include/render/core/renderer.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "scene/scene.hpp"

void Renderer::Init() {
    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderScene(const Scene *scene, const mathpp::mat4f& view, const mathpp::mat4f& projection,const mathpp::vec3f& viewVec) {
    const std::unordered_map<Entity, MeshComponent> & meshes = scene->GetAllMeshes();

    mathpp::vec3f lightDir = {0.0f, -1.0f, 0.0f};
    mathpp::vec3f sunColor = {1.0f, 1.0f, 1.0f};
    float sunIntensity = 1.0f;

    const auto& sunLights = scene->GetAllSunlights();
    if (!sunLights.empty()) {
        Entity sunEntity = sunLights.begin()->first;
        const TransformComponent* sunTransform = scene->GetComponent<TransformComponent>(sunEntity);
        const SunlightComponent* sunComp = scene->GetComponent<SunlightComponent>(sunEntity);

        if (sunTransform) {
            mathpp::vec3f baseForward = {0.0f, 0.0f, -1.0f};
            mathpp::quat<float> q = mathpp::QuatFromEulerAngles(sunTransform->rotation);
            lightDir = -mathpp::RotateVector(q, baseForward);
        }
        if (sunComp) {
            sunColor = sunComp->color;
            sunIntensity = sunComp->intensity;
        }
    }

    for (auto it = meshes.begin(); it != meshes.end(); ++it) {
         const MeshComponent* mesh = &it->second;
         const TransformComponent* transform = scene->GetComponent<TransformComponent>(it->first);
        scene->GetShader(mesh->shaderID)->Use();
        if (transform != nullptr) {
            mathpp::mat4f mat = transform->getMatrix();
            scene->GetShader(mesh->shaderID)->setMat4f("model",mat);
            scene->GetShader(mesh->shaderID)->setMat4f("view",view);
            scene->GetShader(mesh->shaderID)->setVec3f("viewVec",viewVec);
            scene->GetShader(mesh->shaderID)->setVec3f("lightDir", lightDir);
            scene->GetShader(mesh->shaderID)->setVec3f("lightColor", sunColor);
            scene->GetShader(mesh->shaderID)->setFloat("lightIntensity", sunIntensity);
            scene->GetShader(mesh->shaderID)->setVec3f("albedo", mathpp::vec3f{0.8f, 0.8f, 0.8f});
            scene->GetShader(mesh->shaderID)->setMat4f("projection",projection);
        }
        scene->GetMesh(mesh->meshID)->Draw();

    }
}
