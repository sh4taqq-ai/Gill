#pragma once
#include <memory>
#include "mesh.hpp"



std::unique_ptr<Mesh> CreatePlane() {
    Vertex vert1({-1.0f,0.0f,-1.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f});
    Vertex vert2({1.0f,0.0f,-1.0f},{0.0f,1.0f,0.0f},{1.0f,0.0f});
    Vertex vert3({-1.0f,0.0f,1.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f});
    Vertex vert4({1.0f,0.0f,1.0f},{0.0f,1.0f,0.0f},{1.0f,1.0f});
    std::vector<Vertex> planeVert = {vert1,vert2,vert3,vert4};
    std::vector<unsigned int> planeIndices = {0,1,2,1,2,3};

    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(planeVert,planeIndices);

    return  mesh;
}







