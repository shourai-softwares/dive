#include <iostream>
#include <exception>

#include "WindowManager.h"

using namespace std;

WindowManager::WindowManager() {
    try {
        this->initializeWindow();
    } catch(exception& e) {
        cout << e.what() << '\n';
    }
}

WindowManager::~WindowManager() {
    glfwTerminate();
}

void WindowManager::run(GLuint program) {
    const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    GLuint vertexObject;
    glCreateVertexArrays(1, &vertexObject);
    glBindVertexArray(vertexObject);

    static const GLfloat triangle[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f,
        0.0f,  1.0f, 0.0f, 1.0f,
    };

    GLuint vertexbuffer;
    glCreateBuffers(1, &vertexbuffer);
    glNamedBufferStorage(vertexbuffer, sizeof(triangle), triangle, GL_MAP_WRITE_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(0);
    while(!glfwWindowShouldClose(this->window)) {
        glVertexArrayVertexBuffer(vertexObject, 0, vertexbuffer, 0, 4*sizeof(GLfloat));
        glVertexArrayAttribFormat(vertexObject, 0, 4, GL_FLOAT, GL_FALSE, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(this->window);
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, red);
    }
    glDisableVertexAttribArray(0);
}

void WindowManager::initializeWindow() {
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    this->window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);

    if( this->window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
    }
    glfwMakeContextCurrent(this->window);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}
