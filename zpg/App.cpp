#include "App.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShapeBuilder.h"
#include "Shaders.h"
#include "Rotation.h"
#include "Translation.h"
#include "Scale.h"
#include <iostream>
#include "Models/sphere.h"
#include "Models/bushes.h"
#include "Models/suzi_flat.h"


Scene* createscene3();





// --- CALLBACKS ---
static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    App* app = (App*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    cout << key;
    if (action == GLFW_PRESS) {
        if (key >= 321 && key <= 323) {
            std::cout << "stisknuto";

            int index = key - 321;
            cout << index;
            app->currentScene = index;

        }
    }
}

// --- INICIALIZACE ---
void App::init() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "ZPG", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this); //ulozime do okna pointer na this idojwkescp
    glfwSetKeyCallback(window, key_callback);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
}

ShaderProgram blueShader, redShader, shader6v;
Model squareModel, triangleModel, sph1;
DrawableObject* triangleObj;
DrawableObject* squareObj;
// --- SHADERY ---
void App::createShaders() {
    blueShader.load(vertexShader, fragmentShaderBlue);
    redShader.load(vertexShader, fragmentShaderRed);
    shader6v.load(vertexShader6v, fragmentShader6v);
}

// --- MODELY A OBJEKTY ---
void App::createModels() {
    // --- pøiprav modely ---
    auto squareVerts = ShapeBuilder::makeSquare({ 0.3f, 0.3f, 1.0f }, { 0.9f, 0.9f, 1.0f });
    squareModel.upload(squareVerts);

    triangleModel.upload({
        {0.0f, 0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        { -0.5f,  -0.5f, 0.0f}
        });


    triangleObj = new DrawableObject(&triangleModel, &blueShader);
    squareObj = new DrawableObject(&squareModel, &redShader);

    Transformation* triTrans = new Transformation();
    triTrans->add(new Rotation(glm::vec3(0, 0, -0.5f)));       
    triTrans->add(new Translation(glm::vec3( 0, 0, 0)));
    triangleObj->setTransformation(triTrans);

    Transformation* sqTrans = new Transformation();
    sqTrans->add(new Scale(glm::vec3(0.9f)));
    sqTrans->add(new Rotation(glm::vec3(00, 0, -0.5)));
    squareObj->setTransformation(sqTrans);

    Scene* scene = new Scene();
    Scene* scene2 = new Scene();
    Scene* scene3 = new Scene();
    currentScene = 2;

    scene->addObject(triangleObj);
    this->scenes.push_back(scene);

    ////////////////  SCENE 2
    //sph1 model
    size_t vertexCount = sizeof(sphere) / (6 * sizeof(float));
    sph1.upload(sphere, vertexCount);

    //vector3 pro umisteni
    vector<glm::vec3> positions = {
        {-5.0f, 0.0f, 0.0f},   
        { 5.0f, 0.0f, 0.0f},   
        { 0.0f, 5.0f, 0.0f},   
        { 0.0f,-5.0f, 0.0f}    
    };

    for (auto& pos : positions) {
        DrawableObject* sphereObj = new DrawableObject(&sph1, &shader6v);

        Transformation* t = new Transformation();
        t->add(new Scale(glm::vec3(0.1f)));
        t->add(new Translation(pos));
        sphereObj->setTransformation(t);

        scene2->addObject(sphereObj);
    }



    this->scenes.push_back(scene2);

    Scene* complexscene = createscene3();
    this->scenes.push_back(complexscene);

}




void App::run() {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = (float)glfwGetTime();

        // --- update všech objektù ---
        scenes[currentScene]->update();

        // --- draw ---
        scenes[currentScene]->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
      


    glfwDestroyWindow(window);
    glfwTerminate();
}

Model modeltree, suziModel;



Scene* createscene3()
{
    Scene* scene = new Scene();
    size_t vertexCount = sizeof(bushes) / (6 * sizeof(float));
    modeltree.upload(bushes, vertexCount);
    for (int i = 0; i < 16; i++) {
        DrawableObject* bushObj = new DrawableObject(&modeltree, &shader6v);
        Transformation* t = new Transformation();

        t->add(new Scale(glm::vec3(0.5f)));
        t->add(new Translation(glm::vec3(i * 0.2f - 1.5f, -2.0f, 0.0f))); // øada vedle sebe
        bushObj->setTransformation(t);

        scene->addObject(bushObj);
    }
    vertexCount = sizeof(suziFlat) / (6 * sizeof(float));
    suziModel.upload(suziFlat, vertexCount);
    DrawableObject* suziObj = new DrawableObject(&suziModel, &shader6v);
    Transformation* suziTrans = new Transformation();
    suziTrans->add(new Scale(glm::vec3(0.5f)));
    suziTrans->add(new Rotation(glm::vec3(0.0f, 1.0f, 0.0f)));
    suziObj->setTransformation(suziTrans);
    scene->addObject(suziObj);

    Model* triModel = new Model();
    triModel->upload({
        { 0.0f, 0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        {-0.5f, -0.5f, 0.0f}
        });

    for (int i = 0; i < 4; i++) {
        DrawableObject* triObj = new DrawableObject(triModel, &blueShader);
        Transformation* triTrans = new Transformation();
        triTrans->add(new Scale(glm::vec3(0.3f)));
        triTrans->add(new Translation(glm::vec3(-1.5f + i * 1.0f, 2.5f, 0.0f))); 
        triTrans->add(new Rotation(glm::vec3(0.5f, 0.5f, 0)));
        triObj->setTransformation(triTrans);

        scene->addObject(triObj);
    }


    return scene;
}