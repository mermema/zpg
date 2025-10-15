#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Scene.h"

class App {
public:
    void init();
    void createShaders();
    void createModels();
    void run();
    int currentScene;
    Scene* getScene(int index) {
        if (index >= 0 && index < scenes.size())
            return scenes[index];
        return nullptr;
    }


private:
    GLFWwindow* window;
    vector<Scene*> scenes;
};
