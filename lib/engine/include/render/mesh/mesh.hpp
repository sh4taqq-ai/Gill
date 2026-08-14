#pragma once
#include <vector>
#include "../data/vertex.hpp"


class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void Draw();
    ~Mesh();
    private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
};