#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <util.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
    if (!initializeGLFW()) {
        return -1;
    }

    GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT, "GLTF Model");
    if (!window) {
        return -1;
    }

    if (!initializeGLAD()) {
        cleanup(window);
        return -1;
    }
}