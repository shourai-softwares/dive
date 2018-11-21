#ifndef __SCENE_H__
#define __SCENE_H__

#include <list>
#include "VisibleObject.h"

class Scene {
    public:
        void load();
        void unload();
        void render();
        GLfloat* getBackgroundColor();

    private:
        int numCoords;
        int objectTotal;
        GLuint vertexObject;
        GLuint vertexBuffer;
};


#endif //DIVE_SCENE_H
