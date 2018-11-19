#ifndef __VISIBLE_OBJECT_H__
#define __VISIBLE_OBJECT_H__

#include <string>
#include <dirent.h>
#include <GL/glew.h>
#include "Asset.h"

class VisibleObject {
    public:
        explicit VisibleObject(struct dirent *entity);
        std::vector<GLfloat> getVertices();

        std::string name;
        int x, y, z;

    private:
        Asset asset;
};

#endif

