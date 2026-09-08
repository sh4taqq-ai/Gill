#pragma once
#include "mat4.hpp"
#include <cstdint>
#include <memory>
#include "core/set/sparseset.hpp"
#include "core/system/asset/asset.hpp"
#include "render/shader/shader.hpp"
#include "render/texture/texture.hpp"

class TransformSystem;
class Scene;
class Mesh;

using Entity = uint32_t;

struct MeshComponent;


enum class ViewportMode : int {
    Solid = 1,
    Textured = 2,
    Rendered = 3
};

class Renderer {
    public:
    ~Renderer();
    void Init(TransformSystem* transformSystem);
    AssetID LoadShader(Shader shader) { return shaderManager.Load(std::move(shader)); }
    AssetID LoadTexture(Texture texture) {return textureManager.Load(std::move(texture)); }
    void renderScene(const Scene* scene,const mathpp::mat4f& view,const mathpp::mat4f& projection,const mathpp::vec3f& viewVec);

    const Shader* GetShader(AssetID id) const;
    const Texture* GetTexture(AssetID id) const;
    const Mesh* GetMesh(AssetID id) const;
    bool HasMesh(AssetID id)const;
    bool HasTexture(AssetID id)const;
    bool HasShader(AssetID id)const;
private:
    void SetViewportMode(ViewportMode mode);
    ViewportMode GetViewportMode();
    void DrawEntity(const Scene* scene, Entity entity, const mathpp::mat4f& view, const mathpp::mat4f& proj,const mathpp::vec3f& viewVec);
    TransformSystem* _transformSystem;
    std::unique_ptr<Shader> solidShader;
    std::unique_ptr<Shader> textureShader;
    std::unique_ptr<Texture> matCapTexture;
    std::unique_ptr<Texture> fallBackTexture;

    AssetManager<Mesh> meshManager;
    AssetManager<Shader> shaderManager;
    AssetManager<Texture> textureManager;

    ViewportMode _viewportMode;
};