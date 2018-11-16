#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

class VisibleObject;

#include <string>
#include <list>

class FileManager {
    public:
        ~FileManager();

        void openDir(std::string dirName);

        static std::string* readFile(std::string filePath);

        std::list<VisibleObject*> objects;
    private:
};

#endif

