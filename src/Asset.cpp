#include "Asset.h"

#include <iostream>

bool Asset::loadAsset(std::string assetName) {
    std::string warn;
    std::string err;

    std::string path = "./models/" + assetName + "/" + assetName + ".obj";

    bool successfullyLoaded = tinyobj::LoadObj(
        &this->attrib,
        &this->shapes,
        &this->materials,
        &warn,
        &err,
        "./models/Rock/Rock_6.OBJ"
    );

    if (!warn.empty()) {
        std::cerr << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    return successfullyLoaded;
}

std::vector<GLfloat> Asset::getVertices() {
    std::vector<tinyobj::real_t> model;

    // Loop over shapes
    for (auto &shape : shapes) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];

                model.push_back(vx);
                model.push_back(vy);
                model.push_back(vz);
                model.push_back(-1.0f);
            }
            index_offset += fv;

            // per-face material
            shape.mesh.material_ids[f];
        }
    }

    return model;
}
