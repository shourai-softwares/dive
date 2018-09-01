#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowManager {
    public:
        WindowManager();
        ~WindowManager();
        void run(GLuint program);

    private:
        GLFWwindow* window;

        void initializeWindow();
};
