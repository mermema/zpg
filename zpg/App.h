#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CallbackHandler.h"

#include <iostream>
#include "BallScene.h"
#include "ForrestScene.h"
#include "ObjectScene.h"
#include "SolarSystemScene.h"
#include "ReflectorTestScene.h"
#include "SkyBoxScene.h"

class App {
public:
    void init();
    void run();
    Scene* getScene(int index) {
        if (index >= 0 && index < scenes.size())
            return scenes[index];
        return nullptr;
    }
    void setCurrentScene(int index);
    void createScenes();
    int GetCurrentSceneIndex() {return currentScene;}

private:
    int currentScene;
    GLFWwindow* window;
    vector<Scene*> scenes;
};
