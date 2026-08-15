#pragma once
#include <vector>
class Scene;


class Renderer {
    public:
    Renderer();
    ~Renderer();
    void renderScene(const Scene* scene);

private:
};