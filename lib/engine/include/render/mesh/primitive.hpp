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
/*
 * UV Sphere parameterization
 *
 * The sphere is described using two parameters:
 *
 *   phi   = latitude angle, moves from bottom → top
 *   theta = longitude angle, moves around each ring
 *
 * We discretize the continuous surface:
 *
 *   rings    → number of latitude divisions
 *   segments → number of divisions around each ring
 *
 * Angular step:
 *
 *   Δphi   = π / rings
 *   Δtheta = 2π / segments
 *
 * For a particular vertex:
 *
 *   phi   = -π/2 + ring * Δphi
 *   theta =          segment * Δtheta
 *
 * Position:
 *
 *   x = r cos(phi) cos(theta)
 *   y = r sin(phi)
 *   z = r cos(phi) sin(theta)
 */

std::unique_ptr<Mesh>  inline CreateSphere(unsigned int rings, unsigned int segments,float radius) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    //We need segments and rings to simulate real sphere, because computer cannot generate infinite vertices to cover up the surface of the sphere





    for (int ring = 0; ring <= rings; ++ring) {

        //We calculate the vertical angle

        float phi =
            -mathpp::PI<float> / 2.0f +
            mathpp::PI<float> * static_cast<float>(ring) / static_cast<float>(rings);

        for (int segment = 0; segment <= segments; ++segment) {

            float theta =
                2.0f * mathpp::PI<float> * static_cast<float>(segment) / static_cast<float>(segments);
            // We calculate the theta for every single rings and multiply it by segment because segment acts as scalar steps

            // Position
            float x = radius * cos(phi) * cos(theta);
            float y = radius * sin(phi);
            float z = radius * cos(phi) * sin(theta);

            mathpp::vec3f pos{x, y, z};

            // Normal
            mathpp::vec3f normal =
                mathpp::normalize(pos);

            // UV
            float u = static_cast<float>(segment) / segments;
            float v = static_cast<float>(ring) / rings;

            mathpp::vec2f texCoord{u, v};

            vertices.push_back({
                pos,
                normal,
                texCoord
            });
        }
    }
    for (int ring = 0; ring < rings; ++ring) {
        for (int segment = 0; segment < segments; ++segment) {

            unsigned int a =
                ring * (segments + 1) + segment;

            unsigned int b = a + 1;

            unsigned int c =
                (ring + 1) * (segments + 1) + segment;

            unsigned int d = c + 1;

            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(b);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(vertices, indices);
    return  mesh;
}

std::unique_ptr<Mesh> inline CreateCylinder(unsigned int segments,float radius, float height) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    // Top vertices
    for (unsigned int segment = 0; segment < segments; ++segment) {
        float theta = 2.0f * mathpp::PI<float> * static_cast<float>(segment) / static_cast<float>(segments);
        float x = radius * cosf(theta);
        float y = height/2;
        float z = radius * sinf(theta);

        mathpp::vec3f pos{x, y, z};
        float normX = cosf(theta);
        float normY = 0;
        float normZ = sinf(theta);
        mathpp::vec3f normal{normX, normY, normZ};
        float u = static_cast<float>(segment) / segments;
        float v = 1;
        mathpp::vec2f texCoord{u, v};
        vertices.push_back({pos,normal,texCoord});
    }
    //bottom vertices
    for (unsigned int segment = 0; segment < segments; ++segment) {
        float theta = 2.0f * mathpp::PI<float> * static_cast<float>(segment) / static_cast<float>(segments);
        float x = radius * cosf(theta);
        float y = -height/2;
        float z = radius * sinf(theta);
        mathpp::vec3f pos{x, y, z};
        float normX = cosf(theta);
        float normY = 0;
        float normZ = sinf(theta);
        mathpp::vec3f normal{normX, normY, normZ};
        float u = static_cast<float>(segment) / segments;
        float v = 0;
        mathpp::vec2f texCoord{u, v};
        vertices.push_back({pos,normal,texCoord});

    }

    for (unsigned int s = 0; s < segments; ++s) {
        unsigned int top0 = s;
        unsigned int top1 = (s + 1) % segments;

        unsigned int bottom0 = segments + s;
        unsigned int bottom1 = segments + (s + 1) % segments;

        indices.push_back(top0);
        indices.push_back(bottom0);
        indices.push_back(top1);

        indices.push_back(top1);
        indices.push_back(bottom0);
        indices.push_back(bottom1);
    }

    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(vertices, indices);
    return  mesh;
}




