#include "io/obj/obj.hpp"
#include "mathpp.hpp"
#include "render/mesh/mesh.hpp"
#include "render/data/vertex.hpp"
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>


Mesh LoadOBJ(const std::string& path) {
    std::vector<mathpp::vec3f> positions;
    std::vector<mathpp::vec2f> uvs;
    std::vector<mathpp::vec3f> normals;

    std::vector<Vertex> outVertices;
    std::vector<unsigned int> outIndices;
    std::unordered_map<ObjIndexKey, unsigned int, ObjIndexKeyHash> indexMap;

    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            mathpp::vec3f p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        } else if (prefix == "vt") {
            mathpp::vec2f uv;
            ss >> uv.x >> uv.y;
            uvs.push_back(uv);
        } else if (prefix == "vn") {
            mathpp::vec3f n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(mathpp::normalize(n));
        } else if (prefix == "f") {
            for (int i = 0; i < 3; ++i) {
                std::string vertStr;
                ss >> vertStr; // e.g. "12/5/3"

                int posIdx, uvIdx, normalIdx;
                sscanf(vertStr.c_str(), "%d/%d/%d", &posIdx, &uvIdx, &normalIdx);

                ObjIndexKey key{posIdx, uvIdx, normalIdx};
                auto it = indexMap.find(key);
                if (it != indexMap.end()) {
                    outIndices.push_back(it->second);
                } else {
                    Vertex v;
                    v.position = positions[posIdx - 1]; // OBJ is 1-indexed
                    v.uv = uvs[uvIdx - 1];
                    v.normal = normals[normalIdx - 1];
                    unsigned int newIndex = static_cast<unsigned int>(outVertices.size());
                    outVertices.push_back(v);
                    outIndices.push_back(newIndex);
                    indexMap[key] = newIndex;
                }
            }
        }
    }
    Mesh mesh(outVertices, outIndices);
    return mesh;
}