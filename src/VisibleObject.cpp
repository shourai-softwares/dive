#include "VisibleObject.h"

VisibleObject::VisibleObject(struct dirent *entity)  {
    name = entity->d_name;
    x = y = z = 0;
    asset.loadAsset("Rock");
}

std::vector<GLfloat> VisibleObject::getVertices() {
    return asset.getVertices();
}
