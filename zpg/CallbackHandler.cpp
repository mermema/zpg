#include "CallbackHandler.h"

static bool rightMousePressed = false;
static bool firstMouse = true;
static double lastMouseX = 400.0;
static double lastMouseY = 300.0;

void CallbackHandler::error(int error, const char* description) {
    fputs(description, stderr);
}

void CallbackHandler::framebufferSize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    App* app = (App*)glfwGetWindowUserPointer(window);
    if (!app) return;

    auto scene = app->getScene(app->GetCurrentSceneIndex());
    if (!scene) return;

    Camera* camera = scene->getCamera();
    if (camera) {
        camera->setAspectRatio((float)width / (float)height);
    }
}

void CallbackHandler::key(GLFWwindow* window, int key, int scancode, int action, int mods) {
    App* app = (App*)glfwGetWindowUserPointer(window);
    if (!app) return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
    
    auto scene = app->getScene(app->GetCurrentSceneIndex());
    if (!scene) return;

    Camera* camera = scene->getCamera();

    if (action == GLFW_PRESS) {
        if (key >= 321 && key <= 326) {
            int index = key - 321;
            app->setCurrentScene(index);
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            CallbackHandler::framebufferSize(window, width, height);
        }

        if (key >= 49 && key <= 57) {
            int index = key - 49;
            app->setCurrentScene(index);
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            CallbackHandler::framebufferSize(window, width, height);
        }
    }

    if (camera) {
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            scene->toggleFlashlight();

        float speed = 0.1f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->forward(speed);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->backward(speed);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->toLeft(speed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->toRight(speed);
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) scene->handlePKey();

    }
}

void CallbackHandler::mouseButton(GLFWwindow* window, int button, int action, int mods) {
    App* app = (App*)glfwGetWindowUserPointer(window);
    if (!app) return;

    auto scene = app->getScene(app->GetCurrentSceneIndex());
    if (!scene) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        scene->handleMouseClick(x, y);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        scene->handleMouseRelease(x, y);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        rightMousePressed = true;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        rightMousePressed = false;

    }
}

void CallbackHandler::cursorPos(GLFWwindow* window, double xpos, double ypos) {
    App* app = (App*)glfwGetWindowUserPointer(window);
    if (!app) return;

    auto scene = app->getScene(app->GetCurrentSceneIndex());
    if (!scene) return;

    Camera* camera = scene->getCamera();
    if (!camera) return;

    if (rightMousePressed) {
        if (firstMouse) {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastMouseX;
        float yoffset = lastMouseY - ypos;

        lastMouseX = xpos;
        lastMouseY = ypos;

        float sensitivity = 0.05f;
        camera->changeYaw(xoffset * sensitivity);
        camera->changePitch(yoffset * sensitivity);
    }
    else {
        firstMouse = true;
    }
}
