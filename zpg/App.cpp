#include "App.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShapeBuilder.h"
#include "Shaders.h"
#include "Rotation.h"
#include "Translation.h"
#include "DynamicRotation.h"
#include "Scale.h"
#include <iostream>
#include "Models/sphere.h"
#include "Models/bushes.h"
#include "Models/suzi_flat.h"
#include "Models/tree.h"

Scene* createscene3();
Scene* createForrest();
bool rightMousePressed = false;
double lastX = 400.0;
double lastY = 300.0;
bool firstMouse = true;




// --- CALLBACKS ---
static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    App* app = (App*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if ( action == GLFW_PRESS) {
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
    glfwSetWindowUserPointer(window, this); //ulozime do okna pointer na this
    glfwSetKeyCallback(window, key_callback);
    //glfwSetMouseButtonCallback(window, mouse_button_callback); 
    //glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
    GLint major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "OpenGL version: " << major << "." << minor << std::endl;
}

ShaderProgram* blueShader = nullptr;
ShaderProgram* redShader = nullptr;
ShaderProgram* shader6v = nullptr;
Model squareModel, triangleModel, sph1;
DrawableObject* triangleObj;
DrawableObject* squareObj;
// --- SHADERY ---
void App::createShaders() {
    /*

    auto vertShaC = make_shared<Shader>(vertexShader, GL_VERTEX_SHADER);
    auto vertSha6vC = make_shared<Shader>(vertexShader6v, GL_VERTEX_SHADER);

    auto fragBLueC = make_shared<Shader>(fragmentShaderBlue, GL_FRAGMENT_SHADER);
    auto fragRedC = make_shared<Shader>(fragmentShaderRed, GL_FRAGMENT_SHADER);
    auto fragSha6vC = make_shared<Shader>(fragmentShader6v, GL_FRAGMENT_SHADER);


    std::vector<std::shared_ptr<Shader>> shaders = { vertShaC, fragBLueC };
    blueShader = new ShaderProgram(shaders);

    shaders = { vertShaC, fragRedC };
    redShader = new ShaderProgram(shaders);

    shaders = { vertSha6vC, fragSha6vC };
    shader6v = new ShaderProgram(shaders);
    */
    /*
        Shader *vertShaC = new Shader(vertexShader, GL_VERTEX_SHADER);
    Shader *vertSha6vC = new Shader(vertexShader6v, GL_VERTEX_SHADER);
    Shader* fragBLueC = new Shader(fragmentShaderBlue, GL_FRAGMENT_SHADER);
    Shader *fragRedC = new Shader(fragmentShaderRed, GL_FRAGMENT_SHADER);
    Shader *fragSha6vC = new Shader(fragmentShader6v, GL_FRAGMENT_SHADER);

    // Použijeme vector pointerů na Shader
    std::vector<Shader*> shaders = { vertShaC, fragBLueC };
    blueShader = new ShaderProgram(shaders);

    shaders = { vertShaC, fragRedC };
    redShader = new ShaderProgram(shaders);

    shaders = { vertSha6vC, fragSha6vC };
    shader6v = new ShaderProgram(shaders);
    */


    // Vytvoříme shadery jako static - přežijí celý běh programu
    static Shader vertShaC(vertexShader, GL_VERTEX_SHADER);
    static Shader vertSha6vC(vertexShaderCam, GL_VERTEX_SHADER);
    static Shader fragBLueC(fragmentShaderBlue, GL_FRAGMENT_SHADER);
    static Shader fragRedC(fragmentShaderRed, GL_FRAGMENT_SHADER);
    static Shader fragSha6vC(fragmentShader6v, GL_FRAGMENT_SHADER);

    // Použijeme vector pointerů na Shader
    std::vector<Shader*> shaders = { &vertShaC, &fragBLueC };
    blueShader = new ShaderProgram(shaders);

    shaders = { &vertShaC, &fragRedC };
    redShader = new ShaderProgram(shaders);

    shaders = { &vertSha6vC, &fragSha6vC };
    shader6v = new ShaderProgram(shaders);
}

// --- MODELY A OBJEKTY ---
void App::createModels() {
    // --- připrav modely ---
    auto squareVerts = ShapeBuilder::makeSquare({ 0.3f, 0.3f, 1.0f }, { 0.9f, 0.9f, 1.0f });
    squareModel.upload(squareVerts);

    triangleModel.upload({
        {0.0f, 0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        { -0.5f,  -0.5f, 0.0f}
        });


    triangleObj = new DrawableObject(&triangleModel, blueShader);
    squareObj = new DrawableObject(&squareModel, redShader);

    CompositeTransformation* triTrans = new CompositeTransformation();
    triTrans->add(new DynamicRotation(glm::vec3(0, 0, -0.9f)));
    triTrans->add(new Translation(glm::vec3(0, 0, 0)));
    triangleObj->setTransformation(triTrans);

    CompositeTransformation* sqTrans = new CompositeTransformation();
    sqTrans->add(new Scale(glm::vec3(0.9f)));
    sqTrans->add(new Rotation(0.5f, glm::vec3(00, 0, -0.5)));
    squareObj->setTransformation(sqTrans);

    Scene* scene = new Scene();
    Scene* scene2 = new Scene();
    Scene* scene3 = new Scene();
    currentScene = 0;

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
        DrawableObject* sphereObj = new DrawableObject(&sph1, shader6v);

        CompositeTransformation* t = new CompositeTransformation();
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

    float cameraAngle = 0.0f;
    float cameraRadius = 8.0f;
    float cameraHeight = 3.0f;
    float cameraSpeed = 2.0f;
    double lastMouseX = 400.0, lastMouseY = 300.0;
    bool firstMouse = true;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = (float)glfwGetTime();

        if (currentScene == 2 && scenes.size() > 2) {
            Camera* camera = scenes[2]->getCamera();
            if (camera) {
                float cameraSpeed = 0.1f;
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                    camera->forward(cameraSpeed);
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    camera->backward(cameraSpeed);
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    camera->toLeft(cameraSpeed);
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    camera->toRight(cameraSpeed);
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                    double xpos, ypos;
                    glfwGetCursorPos(window, &xpos, &ypos);

                    if (firstMouse) {
                        lastMouseX = xpos;
                        lastMouseY = ypos;
                        firstMouse = false;
                    }

                    float xoffset = xpos - lastMouseX;
                    float yoffset = lastMouseY - ypos; // Obráceně, protože y jde shora dolů

                    lastMouseX = xpos;
                    lastMouseY = ypos;

                    float sensitivity = 0.05f;
                    camera->changeYaw(xoffset * sensitivity);
                    camera->changePitch(yoffset * sensitivity);
                }
                else {
                    firstMouse = true; // Reset při puštění tlačítka
                }
            
            }
        }

        scenes[currentScene]->update();
        scenes[currentScene]->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

Model modeltree, medelbush;


Scene* createscene3()
{
    Scene* scene = new Scene();

    Camera* camera = new Camera(60.0f, 0.1f, 100.0f);

    camera->setPosition(glm::vec3(0.0f, 0.0f, 8.0f));
    camera->setTarget(glm::vec3(0.0f));

    size_t vertexCount = sizeof(tree) / (6 * sizeof(float));
    modeltree.upload(tree, vertexCount);

    vertexCount = sizeof(bushes) / (6 * sizeof(float));
    medelbush.upload(bushes, vertexCount);

    int gridSize = 9; 
    float spacing = 10.0f; 
    float startOffset = -(gridSize - 1) * spacing * 0.5f; //zarovanni

    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            DrawableObject* bushObj = new DrawableObject(&modeltree, shader6v);
            CompositeTransformation* t = new CompositeTransformation();
            t->add(new Scale(glm::vec3(0.5f)));

            float baseX = startOffset + col * spacing;
            float baseZ = startOffset + row * spacing;

            //randomoffcet
            float randomOffsetX = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 5.f;
            float randomOffsetZ = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 5.0f;


            t->add(new Translation(glm::vec3(
                baseX + randomOffsetX,
                1.0f,
                baseZ + randomOffsetZ
            )));

            bushObj->setTransformation(t);
            scene->addObject(bushObj);
            camera->registerObserver(bushObj->getShader());
        }
    }

    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            DrawableObject* bushObj = new DrawableObject(&medelbush, shader6v);
            CompositeTransformation* t = new CompositeTransformation();
            t->add(new Scale(glm::vec3(0.5f)));

            float baseX = startOffset + col * spacing;
            float baseZ = startOffset + row * spacing;

            float randomOffsetX = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 5.f;
            float randomOffsetZ = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 5.0f;

 

            t->add(new Translation(glm::vec3(
                baseX + randomOffsetX,
                1.0f,
                baseZ + randomOffsetZ
            )));

            bushObj->setTransformation(t);
            scene->addObject(bushObj);
            camera->registerObserver(bushObj->getShader());
        }
    }


    scene->setCamera(camera);

    camera->notifyObservers();
    return scene;
}