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
//#include "Models/sphere.h"
//#include "Models/bushes.h"
#include "Models/suzi_flat.h"
//#include "Models/tree.h"
//#include "Models/plain.h"
#include "ShaderLoader.h"
#include "Light.h"
#include "Firefly.h"
#include "RandomTranslation.h"
#include "BallScene.h"
#include "ForrestScene.h"
#include "ObjectScene.h"
#include "SolarSystemScene.h"

#include "ObjModel.h"
#include "SkyBox.h"
#include "SkyDome.h"

//Scene* createscene3();

Scene* createSolarSystem();
Scene* createTextureScene();

// --- CALLBACKS ---
static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}


static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    App* app = (App*)glfwGetWindowUserPointer(window);
    if (app && app->getScene(app->currentScene)) {
        Camera* camera = app->getScene(app->currentScene)->getCamera();
        if (camera) {
            camera->setAspectRatio((float)width / (float)height);
        }
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float cameraSpeed = 0.1;
    App* app = (App*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);


        if (action == GLFW_PRESS) {
            if (key >= 321 && key <= 326) {
                std::cout << "stisknuto";
                int index = key - 321;
                cout << index;
                app->setCurrentScene(index);


            }
        }

            Camera* camera = app->getScene(app->currentScene)->getCamera();
            if (camera) {
                if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
                {
                    app->getScene(app->currentScene)->toggleFlashlight();
                }


            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera->forward(cameraSpeed);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera->backward(cameraSpeed);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera->toLeft(cameraSpeed);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera->toRight(cameraSpeed);

       
    }
}
bool rightMousePressed = false;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    App* app = (App*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        app->getScene(app->currentScene)->handleMouseClick(x, y);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
       // app->getScene(app->currentScene)->handleMouseRelease(x, y);
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

double lastMouseX = 400.0, lastMouseY = 300.0;
bool firstMouse = true;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    float cameraSpeed = 0.1;
    App* app = (App*)glfwGetWindowUserPointer(window);
    Camera* camera = app->getScene(app->currentScene)->getCamera();
    if (camera) {
        float cameraSpeed = 0.1f;
        if (rightMousePressed) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

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

}


void App::setCurrentScene(int index) {
    if (index >= 0 && index < scenes.size()) {
        currentScene = index;
        scenes[index]->activate();
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
    glfwSetMouseButtonCallback(window, mouse_button_callback); 
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_STENCIL_TEST);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

   // glEnable(GL_DEPTH_TEST);
    GLint major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "OpenGL version: " << major << "." << minor << std::endl;

}

ShaderProgram* blueShader = nullptr;
ShaderProgram* brownShader = nullptr;
ShaderProgram* shader6v = nullptr;
ShaderProgram* fireflyshader = nullptr;

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
    static ShaderLoader vertSha6vC("../zpg/Shaders/uni.vert", GL_VERTEX_SHADER);
    static ShaderLoader fragBLueC("../zpg/Shaders/constant.frag", GL_FRAGMENT_SHADER);
    static Shader fragBrownC(fragmentShaderBrown, GL_FRAGMENT_SHADER);
    static ShaderLoader fragSha6vC("../zpg/Shaders/phongmulti.frag", GL_FRAGMENT_SHADER);

    //Použijeme vector pointerů na Shader
    std::vector<Shader*> shaders = { &vertShaC, &fragBLueC };
    blueShader = new ShaderProgram(shaders);


    shaders = { &vertSha6vC, &fragSha6vC };
    shader6v = new ShaderProgram(shaders);

    ShaderLoader vertPhong("../zpg/Shaders/uni.vert", GL_VERTEX_SHADER);
    ShaderLoader fragPhong("../zpg/Shaders/coloruni/phongmulti.frag", GL_FRAGMENT_SHADER);

    std::vector<Shader*> phongShaders = { &vertPhong, &fragPhong };
    brownShader = new ShaderProgram(phongShaders);

    static ShaderLoader constantCOlor("../zpg/Shaders/coloruni/constant.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> fireflyShaders = { &vertPhong, &constantCOlor };
    fireflyshader = new ShaderProgram(fireflyShaders);


}



// --- MODELY A OBJEKTY ---
void App::createModels() {
    auto squareVerts = ShapeBuilder::makeSquare({ 0.3f, 0.3f, 1.0f }, { 0.9f, 0.9f, 1.0f });
    squareModel.upload(squareVerts);

    triangleModel.upload({
        {0.0f, 0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        { -0.5f,  -0.5f, 0.0f}
        });


    triangleObj = new DrawableObject(&triangleModel, blueShader);

    CompositeTransformation* triTrans = new CompositeTransformation();
    triTrans->add(new DynamicRotation(glm::vec3(0, 0, -0.9f)));
    triTrans->add(new Translation(glm::vec3(0, 0, 0)));
    triangleObj->setTransformation(triTrans);


    Scene* scene = new Scene();

    scene->addObject(triangleObj);
    this->scenes.push_back(scene);

    //Scene* scene2 = createSolarSystem();
    
    SolarSystemScene* scene2 = new SolarSystemScene();
    scene2->create();
    BallScene* scene3 = new BallScene();
    scene3->create();
    currentScene = 1;


    this->scenes.push_back(scene2);
    this->scenes.push_back(scene3);

    ForrestScene* complexscene = new ForrestScene();
    complexscene->create();
    this->scenes.push_back(complexscene);

/*
    ObjectScene* objscene = new ObjectScene();
    objscene->create();
    this->scenes.push_back(objscene);
    */
    Scene* sceneReflector = createTextureScene();
    this->scenes.push_back(sceneReflector);
}




void App::run() {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        scenes[currentScene]->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}




Scene* createTextureScene() {

    const float plainUV[] = {
        //vrchol, normála, uv souřadnice
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 10.0f,

       -1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 10.0f
    };

    static ShaderLoader vert("../zpg/Shaders/uv.vert", GL_VERTEX_SHADER);
    static ShaderLoader frag("../zpg/Shaders/phongmultexture.frag", GL_FRAGMENT_SHADER);

    std::vector<Shader*> shaders = { &vert, &frag };


    static ShaderProgram textureshader(shaders);

    static ShaderLoader skyboxVert("../zpg/Shaders/skybox.vert", GL_VERTEX_SHADER);
    static ShaderLoader skyboxFrag("../zpg/Shaders/skybox.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> skyboxShaders = { &skyboxVert, &skyboxFrag };
    static ShaderProgram skyboxShader(skyboxShaders);

    /*
    static ShaderLoader skyDomeVert("../zpg/Shaders/skydome.vert", GL_VERTEX_SHADER);
    static ShaderLoader skyDomeFrag("../zpg/Shaders/skydome.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> skyDomeShaders = { &skyDomeVert, &skyDomeFrag };
    static ShaderProgram skyDomeShader(skyDomeShaders);
    */

    Scene* scene = new Scene();
    scene->enableMouseActions();
    Camera* camera = new Camera(60.0f, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0.0f, 2.0f, -3.0f)); 
    camera->setTarget(glm::vec3(0.0f, 0.1f, 0.0f));
    scene->setCamera(camera);

    
    std::vector<std::string> skyboxFaces = {
"../zpg/Objects/sky/posx.jpg",   // +X
"../zpg/Objects/sky/negx.jpg",    // -X
"../zpg/Objects/sky/posy.jpg",    // +Y  
"../zpg/Objects/sky/negy.jpg",  // -Y
"../zpg/Objects/sky/posz.jpg",   // +Z
"../zpg/Objects/sky/negz.jpg",   // -Z
    };

    // Vytvoření skyboxu s předaným shaderem
    SkyBox* skybox = new SkyBox(&skyboxShader, skyboxFaces);

    // Přidání do scény
    scene->setSkyObject(skybox);
    

    //SkyDome* skyDome = new SkyDome(&skyDomeShader,"../zpg/Objects/sky/skydome.obj","../zpg/Objects/sky/skydome.png" );
    //scene->setSkyObject(skyDome);

    auto reflector = new DirectionalLight(glm::vec3(0, -1, -1));
    reflector->setIntensity(1);
    //reflector->setAttenuation(0, 0, 0);
    scene->addLight(reflector);

    Material woodMaterial(
        
        glm::vec3(0.1),     
        glm::vec3(0,1,0),      
        glm::vec3(0.1),     // specular
        32.0f               // shininess
    ) ;

    Material defaultMaterial;


    Texture* woodTexture = new Texture(woodMaterial, "../zpg/Objects/wooden_fence.png");


    static Model planeModel;
    size_t vertexCount = sizeof(plainUV) / (8 * sizeof(float));
    planeModel.uploadWithUV(plainUV, vertexCount);

    DrawableObject* planeObj = new DrawableObject(&planeModel, &textureshader, woodTexture);
    CompositeTransformation* plainTrans = new CompositeTransformation();
    plainTrans->add(new Scale(glm::vec3(10, 1, 10)));
    planeObj->setTransformation(plainTrans);
    planeObj->setObjectColor(glm::vec3(0, 1, 0));
    planeObj->setNotMovable();
    scene->addObject(planeObj);


    Material shrekMaterial;
    shrekMaterial.loadFromMTL("../zpg/Objects/shrek.mtl");
    Texture* shrekTexture = new Texture(shrekMaterial, "../zpg/Objects/shrek.png");


    static ObjModel shrek("../zpg/Objects/shrek.obj");
    DrawableObject* shrekObject = new DrawableObject(&shrek, &textureshader, shrekTexture);
    shrekObject->setObjectColor(glm::vec3(0, 1, 0));
    scene->addObject(shrekObject);

    Texture* fionaTexture = new Texture(defaultMaterial, "../zpg/Objects/fiona.png");


    static ObjModel fiona("../zpg/Objects/fiona.obj");
    DrawableObject* fionaObject = new DrawableObject(&fiona, &textureshader, fionaTexture);
    CompositeTransformation* fionaTrans = new CompositeTransformation();
    fionaTrans->add(new Translation(glm::vec3(2, 0, 0)));
    fionaObject->setTransformation(fionaTrans);
    scene->addObject(fionaObject);

    Texture* toiletTexture = new Texture(defaultMaterial, "../zpg/Objects/toiled.jpg");
    static ObjModel toilet("../zpg/Objects/toiled.obj");
    DrawableObject* toiletObject = new DrawableObject(&toilet, &textureshader, toiletTexture);
    CompositeTransformation* toiletTrans = new CompositeTransformation();
    toiletTrans->add(new Translation(glm::vec3(0, 0, -3)));
    toiletObject->setTransformation(toiletTrans);
    scene->addObject(toiletObject);

    Texture* catTexture = new Texture(defaultMaterial, "../zpg/Objects/cat.jpg");
    static ObjModel cat("../zpg/Objects/cat.obj");
    DrawableObject* catObject = new DrawableObject(&cat, &textureshader, catTexture);
    CompositeTransformation* catTrans = new CompositeTransformation();
    catTrans->add(new Rotation(-3.1415926 / 2, glm::vec3(1, 0, 0)));
    catTrans->add(new Scale(glm::vec3(0.03)));
    catTrans->add(new Translation(glm::vec3(-2, 0, 0)));
    catObject->setTransformation(catTrans);
    scene->addObject(catObject);

    Model* sphereModel = new Model();
    sphereModel->upload(sphere, sizeof(sphere) / (6 * sizeof(float)));
    scene->setClickCreateModel(sphereModel);
    scene->setClickCreateShader(&textureshader);





    camera->registerObserver(&textureshader);
    //icamera->registerObserver(&skyDomeShader);

    reflector->registerObserver(&textureshader);
    camera->notifyObservers();
    reflector->notifyObservers();

    return scene;
}








Scene* createSolarSystem() {
    Scene* scene = new Scene();
    Camera* camera = new Camera(60.0f, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0.0f, 0.1f, -3.0f));  // ⬅️ níž a dál
    camera->setTarget(glm::vec3(0.0f, 0.1f, 0.0f));
    scene->setCamera(camera);

    size_t vertexCount = sizeof(sphere) / (6 * sizeof(float));
    sph1.upload(sphere, vertexCount);

    auto reflector = new ReflectorLight(glm::vec3(0,1.5,0), glm::vec3(0,-1,0));
    reflector->setAttenuation(0, 0, 0);
    reflector->setAngle(10);
    scene->addLight(reflector);


    static Model planeModel;
    vertexCount = sizeof(plain) / (6 * sizeof(float));
    planeModel.upload(plain, vertexCount);

    DrawableObject* planeObj = new DrawableObject(&planeModel, brownShader);
    CompositeTransformation* plainTrans = new CompositeTransformation();
    plainTrans->add(new Scale(glm::vec3(10, 1, 10)));
    planeObj->setObjectColor(glm::vec3(0, 1, 0));
    scene->addObject(planeObj);

    
    DrawableObject* ball1 = new DrawableObject(&sph1, brownShader);
    CompositeTransformation* ball1Trans = new CompositeTransformation();
    ball1Trans->add(new Scale(glm::vec3(0.08f)));
    ball1->setTransformation(ball1Trans);
    ball1->setObjectColor(glm::vec3(0.5, 0.5, 0.5));
    scene->addObject(ball1);

    DrawableObject* ball2 = new DrawableObject(&sph1, brownShader);
    CompositeTransformation* ball2Trans = new CompositeTransformation();
    ball2Trans->add(new Scale(glm::vec3(0.08f)));
    ball2Trans->add(new Translation(glm::vec3(-0.2,0.2,0)));

    ball2->setTransformation(ball2Trans);
    ball2->setObjectColor(glm::vec3(0.5, 0.5, 0.5));
    scene->addObject(ball2);

    DrawableObject* ball3 = new DrawableObject(&sph1, brownShader);
    CompositeTransformation* ball3Trans = new CompositeTransformation();
    ball3Trans->add(new Scale(glm::vec3(0.08f)));
    ball3Trans->add(new Translation(glm::vec3(+0.2, 0.2, 0)));

    ball3->setTransformation(ball3Trans);
    ball3->setObjectColor(glm::vec3(0.5, 0.5, 0.5));
    scene->addObject(ball3);

    DrawableObject* ball4 = new DrawableObject(&sph1, brownShader);
    CompositeTransformation* ball4Trans = new CompositeTransformation();
    ball4Trans->add(new Scale(glm::vec3(0.08f)));
    ball4Trans->add(new Translation(glm::vec3(-0.0, 0.5, 0)));

    ball4->setTransformation(ball4Trans);
    ball4->setObjectColor(glm::vec3(0.5, 0.5, 0.5));
    scene->addObject(ball4);

    camera->registerObserver(brownShader);
    reflector->registerObserver(brownShader);
    camera->notifyObservers();
    reflector->notifyObservers();

    return scene;
}