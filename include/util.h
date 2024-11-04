#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>


bool initializeGLFW();
bool initializeGLAD();
GLFWwindow* createWindow(int width, int height, const char* title);
void cleanup(GLFWwindow* window);
