#include "FileManager.h"

#include <fstream>
#include <dirent.h>
#include "VisibleObject.h"
#include "FileObject.h"
#include "DirectoryObject.h"

FileManager::~FileManager() {
    VisibleObject* reader;

    while(!this->objects.empty()) {
        reader = this->objects.front();
        delete reader;
        this->objects.pop_front();
    }
}

void FileManager::openDir(std::string dirName) {
    DIR *directory;
    struct dirent *object;

    directory = opendir(dirName.c_str());
    if(directory == NULL) {
        throw 20;
    }

    while((object = readdir(directory)) != NULL) {
        switch(object->d_type) {
            case DT_REG:
                this->objects.push_back(new FileObject(object));
                break;
            case DT_DIR:
                this->objects.push_back(new DirectoryObject(object));
                break;
            default:
                break;
        }
    }
}

std::string* FileManager::readFile(std::string filePath) {
    std::ifstream file(filePath);
    std::string* content = new std::string(
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>())
    );
    file.close();

    return content;
}
