#include "App.h"
void App::setCurrentScene(int index) {
    if (index >= 0 && index < scenes.size()) {
        currentScene = index;
        scenes[index]->activate();
    }
}



void App::init() {
    glfwSetErrorCallback(CallbackHandler::error);
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
    glfwSetWindowUserPointer(window, this); //seve pointer to This to glfw window....
    glfwSetKeyCallback(window, CallbackHandler::key);
    glfwSetMouseButtonCallback(window, CallbackHandler::mouseButton);
    glfwSetCursorPosCallback(window, CallbackHandler::cursorPos);
    glfwSetFramebufferSizeCallback(window, CallbackHandler::framebufferSize);
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



void App::createScenes() {
    SkyBoxScene * scene = new SkyBoxScene();
    scene->create();
    this->scenes.push_back(scene);


    SolarSystemScene* scene2 = new SolarSystemScene();
    scene2->create();
    this->scenes.push_back(scene2);


    BallScene* scene3 = new BallScene();
    scene3->create();
    this->scenes.push_back(scene3);

    ForrestScene* complexscene = new ForrestScene();
    complexscene->create();
    this->scenes.push_back(complexscene);


    ReflectorTestScene* sceneReflector = new ReflectorTestScene();
    sceneReflector->create();
    this->scenes.push_back(sceneReflector);


    currentScene = 3;

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

