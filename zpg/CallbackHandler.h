#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "App.h"

class CallbackHandler {
public:
    static void error(int error, const char* description);
    static void framebufferSize(GLFWwindow* window, int width, int height);
    static void key(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButton(GLFWwindow* window, int button, int action, int mods);
    static void cursorPos(GLFWwindow* window, double xpos, double ypos);
};
