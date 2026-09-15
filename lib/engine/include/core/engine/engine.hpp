#pragma once
#include <string>
#include <memory>
#include "mathpp.hpp"



class Renderer ;
class Camera;
class Scene;
class TransformSystem;
class Hierarchy;
class MeshSystem;
class MaterialSystem;


class Engine {

public:
    Engine(const mathpp::mat4f& projection);
    ~Engine();
    void Init(unsigned int width, unsigned int height,Camera* cam,Scene* scene);
    void Run();
    void Shutdown();
    TransformSystem* GetTransformSystem();
    Hierarchy* GetHierarchy();
    MeshSystem* GetMeshSystem();
    MaterialSystem* GetMaterialSystem();
    Renderer* GetRenderer();

private:
    unsigned int wdth,hght;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<TransformSystem> transformSystem;
    std::unique_ptr<Hierarchy> hierarchy;
    std::unique_ptr<MaterialSystem> materialSystem;
    std::unique_ptr<MeshSystem> meshSystem;
    Camera* _cam;
    Scene* _scene;

    mathpp::mat4f& proj;

};
