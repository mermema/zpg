#pragma once
#include "ShaderProgram.h"
#include "Model.h"
#include "ShapeBuilder.h"
#include <GLFW/glfw3.h>

using namespace std;

class App {
public:
    App() : window(nullptr) {}
    void init();
    void createShaders();
    void createModels();
    void run();

private:
    GLFWwindow* window;

    ShaderProgram redShader;
    ShaderProgram blueShader; // napø. další shader

    // Modely
    Model squareModel;
    Model triangleModel;
};