#ifndef __ASSET_H__
#define __ASSET_H__

#include <string>
#include <tiny_obj_loader.h>
#include <GL/glew.h>

class Asset {
    public:
        bool loadAsset(std::string assetName);
        std::vector<GLfloat> getVertices();

    private:
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
};


#endif //DIVE_ASSET_H
