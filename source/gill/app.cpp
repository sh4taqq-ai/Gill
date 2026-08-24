#include "app.hpp"
#include "../../lib/engine/include/core/window/window.hpp"
#include "core/component/camera/camera.hpp"
#include "core/engine/engine.hpp"
#include "editor/core/editor.hpp"
#include "scene/scene.hpp"
#include "core/input/input.hpp"

void App::Init(unsigned int width, unsigned int height) {
    wdth = width;
    hght = height;
    aspect = static_cast<float>(width) / static_cast<float>(height);
    projection = mathpp::perspective(45.0f,aspect,0.01f,300.0f);
    window = std::make_unique<Window>(wdth,hght,title);
    engine = std::make_unique<Engine>();
    editor = std::make_unique<Editor>();
    scene = std::make_unique<Scene>();
    camera = std::make_unique<Camera>();
    input = std::make_unique<Input>(window.get());
    engine->Init(wdth,hght);
    editor->Init(wdth,hght,window.get(),scene.get(),projection,camera.get());
}

void App::Run() {
    while (!window->ShouldClose()) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        window->PollEvents();
        input->Update();

        camera->Update(input.get(),deltaTime,{0.0f,0.0f,0.0f});
        engine->Run();
        editor->Run(deltaTime);



        window->SwapBuffers();
    }
}

void App::Shutdown() {
    editor->ShutDown();
    engine->Shutdown();
    glfwTerminate();
}
