#ifndef __VISIBLE_OBJECT_H__
#define __VISIBLE_OBJECT_H__

#include <string>
#include <dirent.h>

class VisibleObject {
    public:
        VisibleObject(struct dirent *entity) {
            name = entity->d_name;
            x = y = z = 0;
        }

        std::string name;
        int x, y, z;
};

#endif

