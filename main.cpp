#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <util.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
    if (!initializeGLFW()) return -1;

    GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT, "cgc");
    if (!window) {
        return -1;
    }

    if (!initializeGLAD()) {
        cleanup(window);
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}