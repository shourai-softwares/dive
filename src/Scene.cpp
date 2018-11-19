#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "FileManager.h"
#include "Scene.h"

void Scene::load() {
    FileManager fm;
    fm.openDir("../");
    objectTotal = fm.objects.size();

    glCreateVertexArrays(1, &vertexObject);
    glBindVertexArray(vertexObject);

    std::vector<GLfloat> baseTriangle = {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.2f, 0.0f, 0.0f, 1.0f,
        0.0f,  0.2f, 0.0f, 1.0f,
        0.2f, 0.0f, 0.0f, 1.0f,
        0.0f,  0.2f, 0.0f, 1.0f,
        0.2f,  0.2f, 0.0f, 1.0f,
    };

    numCoords = int(baseTriangle.size());

    std::vector<GLfloat> triangles;
    for(int i = 0; i < objectTotal; i++) {
        int coord = 0;
        for (GLfloat value : baseTriangle) {
            if((coord % 4) == 0) {
                value += (0.25f * i) - 1.0f;
            }

            triangles.push_back(value);
            coord++;
        }
    }

    auto triangleAllocSize = sizeof(GLfloat) * objectTotal * baseTriangle.size();

    glCreateBuffers(1, &vertexBuffer);
    glNamedBufferStorage(vertexBuffer, triangleAllocSize, triangles.data(), GL_MAP_WRITE_BIT);

    glEnableVertexAttribArray(0);
}

void Scene::unload() {

}

void Scene::render() {
    glVertexArrayVertexBuffer(vertexObject, 0, vertexBuffer, 0, 4*sizeof(GLfloat));
    glVertexArrayAttribFormat(vertexObject, 0, 4, GL_FLOAT, GL_FALSE, 0);

    int totalVertexes = int(objectTotal * (numCoords / 4));
    glDrawArrays(GL_TRIANGLES, 0, totalVertexes);
}

GLfloat* Scene::getBackgroundColor() {
    static GLfloat RGBColor[] = { 5, 39, 94 };
    static GLfloat backGroundColor[] = { RGBColor[0]/255, RGBColor[1]/255, RGBColor[2]/255, 1.0f };

    return backGroundColor;
}
