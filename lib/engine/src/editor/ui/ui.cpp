#include "editor/ui/ui.hpp"







void UIManager::Init(Window* window,uint32_t shaderID) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(),true);
    defaultShaderID = shaderID;
}

void UIManager::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::RenderProperties(Scene* scene) {
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Once);
    ImGui::SetNextWindowPos(
    ImVec2(1,30 ),
    ImGuiCond_Always
);
    ImGui::Begin("Properties",nullptr,ImGuiWindowFlags_NoMove);
    if (scene->GetSelected().has_value()) {
        ImGui::Text("Entity: %u", scene->GetSelected().value());
        ImGui::NewLine();
         TransformComponent* transform = scene->GetComponent<TransformComponent>(scene->GetSelected().value());
        assert(transform && "Every entity must have a TransformComponent");
        ImGui::InputFloat3(
       "Position",
       const_cast<float *>(&transform->position.x));

        ImGui::InputFloat3(
            "Rotation",
            const_cast<float *>(&transform->rotation.x));

        ImGui::InputFloat3(
            "Scale",
            const_cast<float *>(&transform->scale.x));

        SunlightComponent* sunlight = scene->GetComponent<SunlightComponent>(scene->GetSelected().value());
        if (sunlight) {
            ImGui::ColorEdit3("Sun Color", &sunlight->color.x);
            ImGui::DragFloat("Intensity", &sunlight->intensity, 0.05f, 0.0f, 10.0f);
        }
    }
    ImGui::End();
}

void UIManager::AddPrimitive(Scene* scene, PrimitiveType type) {
    Mesh mesh = [&] {
        switch (type) {
            case PrimitiveType::Cube:   return CreateCube();
            case PrimitiveType::Sphere: return CreateSphere(15, 15, 2.0f);
            case PrimitiveType::Plane:  return CreatePlane();
            case PrimitiveType::Cylinder: return CreateCylinder(15,2.0f,6.0f);
            case PrimitiveType::Cone : return CreateCone(15,1.0f,1.0f);
        }
    }();

    Entity entity = scene->CreateEntity();
    AssetID meshID = scene->LoadMesh(std::move(mesh));

    MeshComponent meComp;
    meComp.meshID = meshID;
    meComp.shaderID = defaultShaderID;

    TransformComponent tComp;

    scene->AddTransform(entity, tComp);
    scene->AddMesh(entity, meComp);
}

void UIManager::RenderAddMenu(Scene* scene) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Add")) {
            if (ImGui::MenuItem("Cube"))   AddPrimitive(scene, PrimitiveType::Cube);
            if (ImGui::MenuItem("Sphere")) AddPrimitive(scene, PrimitiveType::Sphere);
            if (ImGui::MenuItem("Plane"))  AddPrimitive(scene, PrimitiveType::Plane);
            if (ImGui::MenuItem("Cylinder")) AddPrimitive(scene, PrimitiveType::Cylinder);
            if (ImGui::MenuItem("Cone")) AddPrimitive(scene, PrimitiveType::Cone);
            if (ImGui::MenuItem("Sun Light")) AddSunlight(scene);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


void UIManager::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::AddSunlight(Scene* scene) {
    Mesh mesh = CreateSphere(8, 8, 0.3f);

    Entity entity = scene->CreateEntity();
    AssetID meshID = scene->LoadMesh(std::move(mesh));

    MeshComponent meComp;
    meComp.meshID = meshID;
    meComp.shaderID = defaultShaderID;

    TransformComponent tComp;

    SunlightComponent sunComp;


    scene->AddTransform(entity, tComp);
    scene->AddMesh(entity, meComp);
    scene->AddSunlight(entity, sunComp);
}

