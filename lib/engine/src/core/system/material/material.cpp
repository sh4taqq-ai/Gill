#include "core/system/material/material.hpp"

const Material *MaterialSystem::GetMaterial(AssetHandle handle) const {
    if (materialManager.Has(handle)) {
        return materialManager.Get(handle);
    }
    return nullptr;
}

bool MaterialSystem::HasMaterial(AssetHandle handle) const {
    return materialManager.Has(handle);
}

AssetHandle MaterialSystem::RegisterMaterial(Material &&material) {
    return materialManager.Load(material);
}

const Shader *MaterialSystem::GetShader(AssetHandle handle) const {
    if (shaderManager.Has(handle)) {
        return shaderManager.Get(handle);
    }
    return nullptr;
}


const Texture *MaterialSystem::GetTexture(AssetHandle handle) const {
    if (textureManager.Has(handle)) {
        return textureManager.Get(handle);
    }
    return nullptr;
}

bool MaterialSystem::HasShader(AssetHandle handle) const {
    return shaderManager.Has(handle);
}

bool MaterialSystem::HasTexture(AssetHandle handle) const {
    return textureManager.Has(handle);
}

void MaterialSystem::RemoveMaterial(AssetHandle handle) {
    materialManager.Unload(handle);
}

void MaterialSystem::RemoveTexture(AssetHandle handle) {
    textureManager.Unload(handle);
}

void MaterialSystem::RemoveShader(AssetHandle handle) {
    shaderManager.Unload(handle);
}

AssetHandle MaterialSystem::LoadTextureFile(const std::string &path) {
    return textureManager.Load(Texture(path));
}

AssetHandle MaterialSystem::LoadShaderFile(const std::string &vertPath, const std::string &fragPath) {
    return shaderManager.Load(Shader(vertPath, fragPath));
}

const Material *MaterialSystem::GetDefaultMaterial() const {
    return &defaultMaterial;
}

