#include <iostream>
#include "FileManager.h"
#include "WindowManager.h"

int main() {
    WindowManager wm;

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;
    GLint test;

    std::string* vertexContent = FileManager::readFile("vertex.glsl");
    std::string* fragmentContent = FileManager::readFile("fragment.glsl");
    const GLchar *vsSource = vertexContent->c_str();
    const GLchar *fsSource = fragmentContent->c_str();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vsSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &test);
    if(test == GL_FALSE)
        std::cout << "NOOOOOOOOO" << std::endl;

    glShaderSource(fragmentShader, 1, &fsSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &test);
    if(test == GL_FALSE)
        std::cout << "NOOOOOOOaOO" << std::endl;

    delete vertexContent;
    delete fragmentContent;

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &test);
    if(test == GL_FALSE)
        std::cout << "NaaaaOOOOOOOaOO" << std::endl;

    wm.run(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);

    return 0;
}
