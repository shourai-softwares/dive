#include <chrono>
#include <thread>
#include <iostream>
#include <exception>
#include <glm/gtc/matrix_transform.hpp>
#include "FileManager.h"
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
    const GLfloat red[] = { 1.0f, 0.3f, 0.3f, 1.0f };
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float initialFoV = 45.0f;

    float speed = 3.0f; // 3 units / second
    float mouseSpeed = 5.0f;

    GLuint vertexObject;
    glCreateVertexArrays(1, &vertexObject);
    glBindVertexArray(vertexObject);

    FileManager fm;
    fm.openDir("../");

    static const GLfloat baseTriangle[] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.2f, 0.0f, 0.0f, 1.0f,
        0.0f,  0.2f, 0.0f, 1.0f,
        0.2f, 0.0f, 0.0f, 1.0f,
        0.0f,  0.2f, 0.0f, 1.0f,
        0.2f,  0.2f, 0.0f, 1.0f,
    };

    auto objectTotal = fm.objects.size();
    auto triangleAllocSize = sizeof(GLfloat) * 6 * 4 * objectTotal;
    auto triangles = (GLfloat*) malloc(triangleAllocSize);

    for(int i = 0; i < objectTotal; i++) {
        for(int point = 0; point < 6; point++) {
            for(int coord = 0; coord < 4; coord++) {
                int arrayPos = (((i * 6) + point) * 4) + coord;
                GLfloat value = baseTriangle[(point * 4) + coord];
                if(coord == 0) {
                    value += (0.25f * i) - 1.0f;
                }
                triangles[arrayPos] = value;
            }
        }
    }

    GLuint vertexBuffer;
    glCreateBuffers(1, &vertexBuffer);
    glNamedBufferStorage(vertexBuffer, triangleAllocSize, triangles, GL_MAP_WRITE_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(0);
    glm::mat4 Model = glm::mat4(1.0f);

    GLint MatrixID = glGetUniformLocation(program, "MVP");
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    double lastTime = glfwGetTime();
    double lastX = 0, lastY = 0;
    int maxFPS = 144;
    double waitTime = 1.0 / (maxFPS);
    while(!glfwWindowShouldClose(this->window)) {
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        double x, y;
        glfwGetCursorPos(this->window, &x, &y);

        horizontalAngle += mouseSpeed * deltaTime * float(lastX - x);
        verticalAngle   += mouseSpeed * deltaTime * float(lastY - y);

        if(deltaTime < waitTime) {
            this_thread::sleep_for(chrono::milliseconds(1000 * (int) (waitTime - deltaTime)));
        }

        lastTime = currentTime;
        lastX = x;
        lastY = y;

        glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );
        glm::vec3 right = glm::vec3(
                sin(horizontalAngle - 3.14f/2.0f),
                0,
                cos(horizontalAngle - 3.14f/2.0f)
        );
        glm::vec3 up = glm::cross( right, direction );

        if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS){
            position += direction * deltaTime * speed;
        }
        if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
        }
        if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }

        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(position, position + direction, up);
        glm::mat4 mvp = Projection * View * Model;

        glVertexArrayVertexBuffer(vertexObject, 0, vertexBuffer, 0, 4*sizeof(GLfloat));
        glVertexArrayAttribFormat(vertexObject, 0, 4, GL_FLOAT, GL_FALSE, 0);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, (int) objectTotal * 6);

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
    this->window = glfwCreateWindow( 1024, 768, "Playground", nullptr, nullptr);

    if(this->window == nullptr) {
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
