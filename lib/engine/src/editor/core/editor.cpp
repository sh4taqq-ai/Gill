#include "editor/core/editor.hpp"
#include "editor/selector/selector.hpp"
#include "render/shader/shader.hpp"
#include "editor/grid/grid.hpp"
#include "core/input/input.hpp"
#include "editor/ui/ui.hpp"
#include "core/component/camera/camera.hpp"
#include "core/system/asset/asset.hpp"
#include "core/system/hierarchy/hierarchy.hpp"
#include "editor/gizmo/gizmoController.hpp"
#include "core/system/transform/transform.hpp"
#include "scene/scene.hpp"



void Editor::Init(float Width, float Height,Window* window,Scene* scene,const mathpp::mat4f& projection, Camera* camera,TransformSystem* transformSystem,Hierarchy* hierarchy) {
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
    gizmoController = std::make_unique<GizmoController>();
    _hierarchy = hierarchy;
    _transformSystem = transformSystem;
    input = std::make_unique<Input>(_window);
    ui = std::make_unique<UIManager>();
    gizmo = std::make_unique<Gizmo>();
    gridRenderer->Init(100);
    gizmo->Init(wdth,hght,&gizmoData);
    ui->Init(window,shaderID);
    selector->Init(wdth,hght);
    gizmoController->Init(wdth,hght,&gizmoData);
    auto handle1 = input->mouseDown.Subscribe([this](int mx, int my) { OnMouseDown(mx, my); });
    auto handle2 = input->mouseUp.Subscribe([this](int mx, int my) {OnMouseUp(mx,my); });

    _handles.push_back(std::make_pair(&input->mouseDown,handle1));
    _handles.push_back(std::make_pair(&input->mouseUp,handle2));






}

void Editor::Run(float deltaT) {
    float currentTime = glfwGetTime();
    input->Update();
    deltaTime = deltaT;
    _camera->Update(input.get(),deltaTime,{0.0f,0.0f,0.0f});
    gridRenderer->Render(_camera->GetViewMatrix(),proj,_camera->GetPosition());
    mathpp::vec2f pos;
    input->GetCursorPos(pos);
    if (_scene->GetSelected().has_value()) {
        gizmo->Render(_scene,_camera->GetViewMatrix(),proj,_transformSystem->GetTransform(_scene->GetSelected().value()).position,_camera->GetPosition());
        gizmo->RenderIDs(_camera->GetViewMatrix(),proj,_transformSystem->GetTransform(_scene->GetSelected().value()).position,_camera->GetPosition());
        gizmo->UpdateHighlight(static_cast<int>(pos.x),static_cast<int>(pos.y),gizmoController->GetActiveAxis(),gizmoController->IsDragging());
    }

    if (gizmoController->IsDragging()) {
        gizmoController->Apply(_camera->GetViewMatrix(), proj, pos.x, pos.y, _transformSystem,_scene->GetSelected().value());
    }
    selector->RenderScene(_scene,_camera->GetViewMatrix(),proj,_transformSystem);
    ui->BeginFrame();
    ui->RenderProperties(_scene,_transformSystem);
    ui->RenderAddMenu(_scene,_transformSystem);
    ui->RenderHierarchy(_scene,_hierarchy);
    ui->RenderGizmoControls(&gizmoData);
    ui->EndFrame();



}

void Editor::ShutDown() {
    ui->Shutdown();
    for (auto& i : _handles) {
        i.first->Unsubscribe(i.second);
    }
}

Editor::Editor() = default;
Editor::~Editor() = default;

void Editor::TrySelect(int mx, int my) {
   _scene->SetSelected(selector->ReadEntityAt(mx,my));
}

void Editor::OnMouseDown(int mx, int my) {
    if (!ui->WantCaptureMouse()) {
        if (ui->WantCaptureMouse()) return;

        if (_scene->GetSelected().has_value()) {
            GizmoAxis pickedAxis = gizmo->ReadAxisAt(mx, my);
            if (pickedAxis != GizmoAxis::None) {
                gizmoData.axis = pickedAxis;
                TransformComponent transform = _transformSystem->GetTransform(_scene->GetSelected().value());
                gizmoController->Begin(_camera->GetViewMatrix(), proj, mx, my, transform.position, transform.rotation, transform.scale);
                return;
            }
        }

        TrySelect(mx, my);
    }
}


void Editor::OnMouseUp(int mx, int my) {
    gizmoController->End();
}
