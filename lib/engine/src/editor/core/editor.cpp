#include "editor/core/editor.hpp"
#include "editor/selector/selector.hpp"
#include "editor/grid/grid.hpp"
#include "core/input/input.hpp"
#include "editor/ui/ui.hpp"
#include "core/component/camera/camera.hpp"
#include "core/system/asset/asset.hpp"




void Editor::Init(float Width, float Height,Window* window,Scene* scene,const mathpp::mat4f& projection, Camera* camera) {
    _window = window;
    _scene = scene;
    wdth = Width;
    hght = Height;
    proj = projection;
    Shader shader("asset/shader/simpleShader/simpleVert.glsl", "asset/shader/simpleShader/simpleFrag.glsl");
    AssetID shaderID = _scene->LoadShader(std::move(shader));
    _camera = camera;
    selector = std::make_unique<Selector>();
    gridRenderer = std::make_unique<GridRenderer>();
    input = std::make_unique<Input>(_window);
    ui = std::make_unique<UIManager>();
    gizmo = std::make_unique<Gizmo>();
    gridRenderer->Init(100);
    gizmo->Init(wdth,hght);
    ui->Init(window,shaderID);
    selector->Init(wdth,hght);






}

void Editor::Run(float deltaT) {
    float currentTime = glfwGetTime();
    input->Update();
    deltaTime = deltaT;
    _camera->Update(input.get(),deltaTime,{0.0f,0.0f,0.0f});
    gridRenderer->Render(_camera->GetViewMatrix(),proj,_camera->GetPosition());

    if (_scene->GetSelected().has_value()) {
        gizmo->Render(_scene,_camera->GetViewMatrix(),proj,_scene->GetComponent<TransformComponent>(_scene->GetSelected().value())->position,_camera->GetPosition());
        gizmo->RenderIDs(_camera->GetViewMatrix(),proj,_scene->GetComponent<TransformComponent>(_scene->GetSelected().value())->position,_camera->GetPosition());
    }
    selector->RenderScene(_scene,_camera->GetViewMatrix(),proj);
    ui->BeginFrame();
    ui->RenderProperties(_scene);
    ui->RenderAddMenu(_scene);
    ui->EndFrame();



}

void Editor::ShutDown() {
    ui->Shutdown();
}

Editor::Editor() = default;
Editor::~Editor() = default;
