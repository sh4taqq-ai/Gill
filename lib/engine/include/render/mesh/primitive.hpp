#pragma once
#include <memory>
#include "mesh.hpp"



std::unique_ptr<Mesh> inline CreatePlane() {
    Vertex vert1({-1.0f,0.0f,-1.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f});
    Vertex vert2({1.0f,0.0f,-1.0f},{0.0f,1.0f,0.0f},{1.0f,0.0f});
    Vertex vert3({-1.0f,0.0f,1.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f});
    Vertex vert4({1.0f,0.0f,1.0f},{0.0f,1.0f,0.0f},{1.0f,1.0f});
    std::vector<Vertex> planeVert = {vert1,vert2,vert3,vert4};
    std::vector<unsigned int> planeIndices = {0,1,2,1,2,3};

    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(planeVert,planeIndices);

    return  mesh;
}

inline std::unique_ptr<Mesh> CreateCube() {
    std::vector<Vertex> verts = {
        // Front face (+z), normal (0,0,1)
        Vertex({-1,-1, 1}, {0,0,1}, {0,0}),
        Vertex({ 1,-1, 1}, {0,0,1}, {1,0}),
        Vertex({ 1, 1, 1}, {0,0,1}, {1,1}),
        Vertex({-1, 1, 1}, {0,0,1}, {0,1}),

        // Back face (-z), normal (0,0,-1)
        Vertex({ 1,-1,-1}, {0,0,-1}, {0,0}),
        Vertex({-1,-1,-1}, {0,0,-1}, {1,0}),
        Vertex({-1, 1,-1}, {0,0,-1}, {1,1}),
        Vertex({ 1, 1,-1}, {0,0,-1}, {0,1}),

        // Right face (+x), normal (1,0,0)
        Vertex({ 1,-1, 1}, {1,0,0}, {0,0}),
        Vertex({ 1,-1,-1}, {1,0,0}, {1,0}),
        Vertex({ 1, 1,-1}, {1,0,0}, {1,1}),
        Vertex({ 1, 1, 1}, {1,0,0}, {0,1}),

        // Left face (-x), normal (-1,0,0)
        Vertex({-1,-1,-1}, {-1,0,0}, {0,0}),
        Vertex({-1,-1, 1}, {-1,0,0}, {1,0}),
        Vertex({-1, 1, 1}, {-1,0,0}, {1,1}),
        Vertex({-1, 1,-1}, {-1,0,0}, {0,1}),

        // Top face (+y), normal (0,1,0)
        Vertex({-1, 1, 1}, {0,1,0}, {0,0}),
        Vertex({ 1, 1, 1}, {0,1,0}, {1,0}),
        Vertex({ 1, 1,-1}, {0,1,0}, {1,1}),
        Vertex({-1, 1,-1}, {0,1,0}, {0,1}),

        // Bottom face (-y), normal (0,-1,0)
        Vertex({-1,-1,-1}, {0,-1,0}, {0,0}),
        Vertex({ 1,-1,-1}, {0,-1,0}, {1,0}),
        Vertex({ 1,-1, 1}, {0,-1,0}, {1,1}),
        Vertex({-1,-1, 1}, {0,-1,0}, {0,1}),
    };

    std::vector<unsigned int> indices;
    for (unsigned int face = 0; face < 6; face++) {
        unsigned int base = face * 4;
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 0);
    }

    return std::make_unique<Mesh>(verts, indices);
}






