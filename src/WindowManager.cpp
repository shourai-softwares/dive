#include <chrono>
#include <thread>
#include <iostream>
#include <exception>
#include <glm/gtc/matrix_transform.hpp>
#include <tiny_obj_loader.h>
#include "Scene.h"
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
    Scene scene;

    glUseProgram(program);

    glm::vec3 position = glm::vec3( 0, 0, 5 );
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;

    float speed = 3.0f;
    float mouseSpeed = 3.0f;
    glm::mat4 Model = glm::mat4(1.0f);

    GLint MatrixID = glGetUniformLocation(program, "MVP");
    double lastX = 0, lastY = 0;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    scene.load();

    int maxFPS = 144;
    double waitTime = 1.0 / (maxFPS);

    double lastRenderTime = glfwGetTime();

    while(!glfwWindowShouldClose(window)) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        double currentTime = glfwGetTime();
        auto deltaTime = float(currentTime - lastRenderTime);

        horizontalAngle += mouseSpeed * deltaTime * float(lastX - x);
        verticalAngle   += mouseSpeed * deltaTime * float(lastY - y);

        if(deltaTime < waitTime) {
            this_thread::sleep_for(chrono::milliseconds(1000 * (int) (waitTime - deltaTime)));
        }

        lastRenderTime = currentTime;
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

        if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS){
            position += direction * deltaTime * speed;
        }
        if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
        }
        if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }

        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(position, position + direction, up);
        glm::mat4 mvp = Projection * View * Model;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        scene.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, scene.getBackgroundColor());
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    scene.unload();

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
    this->window = glfwCreateWindow(1024, 768, "Dive", nullptr, nullptr);

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
