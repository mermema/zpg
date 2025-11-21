#include "ObjModel.h"
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

ObjModel::ObjModel(const char* objFileName) : Model() {
    loadOBJ(objFileName);
}

void ObjModel::loadOBJ(const char* objFileName) {
    std::string inputfile = objFileName;
    std::string path = inputfile.substr(0, inputfile.find_last_of("/\\") + 1);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), path.c_str());
    std::cout << "=== DEBUG OBJ LOADING ===" << std::endl;
    std::cout << "File: " << objFileName << std::endl;
    std::cout << "Shapes: " << shapes.size() << std::endl;
    std::cout << "Vertices in attrib: " << attrib.vertices.size() / 3 << std::endl;
    std::cout << "Normals in attrib: " << attrib.normals.size() / 3 << std::endl;

    if (!warn.empty()) std::cout << "Warn: " << warn << std::endl;
    if (!err.empty()) std::cerr << "Err: " << err << std::endl;
    if (!ret) throw std::runtime_error("Failed to load OBJ file!");

    std::vector<float> vertices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            //position
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            //normmals (if exists)
            if (index.normal_index >= 0) {
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }



            if (index.texcoord_index >= 0) { //texture cords UV
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

        }
    }

    
    uploadWithUV(vertices.data(), vertices.size() / 8);
}