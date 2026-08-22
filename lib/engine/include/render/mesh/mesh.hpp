#pragma once
#include <vector>
#include "../data/vertex.hpp"


class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void Draw() const;
    // delete copy . GL handles can't be safely duplicated
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // allow move . transfer ownership of the handles, null out the source
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    ~Mesh();
    private:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    unsigned int indexCount;
};