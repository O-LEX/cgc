#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "util.h"
#include "Camera.h"
#include "HairFile.h"
#include "PBD.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
Camera camera(glm::vec3(0.0f, 0.0f, 150.0f)); //Initial Position; Direction (0,0,-1)
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(){
    if(!initializeGLFW()) return -1;

    GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT, "CGC");
    if(!window) return -1;

    if(!initializeGLAD()) return -1;

    #ifdef __APPLE__
    glViewport(0, 0, SCR_WIDTH * 2, SCR_HEIGHT * 2);
    #else
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    #endif
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    Shader shader(SOURCE_DIR "/shader/tiny_hair_vertex.glsl", SOURCE_DIR "/shader/tiny_hair_fragment.glsl");

    HairFile hairFile;
    try {
        hairFile.LoadFromFile(SOURCE_DIR "/model/wWavy.hair");
        // hairFile.CreateVAO();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error loading file: " << e.what() << std::endl;
        return 1;
    }

    hairFile.PrintHeader();

    PBD pbd;
    pbd.initialize(hairFile);
    pbd.CreateVAO();

    // while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        pbd.update(deltaTime);

        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", modelMatrix);

        pbd.draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    // }

    glfwTerminate();
    return 0;
}

// Callback function for framebuffer size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback function for mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Callback function for mouse scroll wheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

// Function to process input from keyboard
void processInput(GLFWwindow *window) {
    bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime, shift);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime, shift);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime, shift);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime, shift);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime, shift);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime, shift);
}