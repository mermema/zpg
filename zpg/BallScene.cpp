#include "BallScene.h"


void BallScene::create()
{


    static ShaderLoader vert("../zpg/Shaders/uv.vert", GL_VERTEX_SHADER);
    static ShaderLoader frag("../zpg/Shaders/phongmultexture.frag", GL_FRAGMENT_SHADER);

    std::vector<Shader*> shaders = { &vert, &frag };
   

    ShaderProgram * shaderProgram = new ShaderProgram(shaders);

    Camera* camera = new Camera(60, 0.1f, 100.0f);
    this->setCamera(camera);
    size_t vertexCount = sizeof(sphere) / (6 * sizeof(float));
    ObjModel * sph1 = new ObjModel("../zpg/Objects/solarsystem/planet.obj");
    this->addLight(new Light(
        glm::vec3(0.0f, 0.0f, 0.0f),    //?ightpos
        glm::vec3(1.0f, 1.0f, 1.0f),    //light color
        1.0f
    ));

    /*this->createLight(
        glm::vec3(1.0f, 1.0f, 1.0f),    //?ightpos
        glm::vec3(1.0f, 1.0f, 0.5f),    //light color
        1.0f
    );
    
    */



    //vector3 for position of ball in space
    vector<glm::vec3> positions = {
        {-0.5f, 0.0f, 0.0f},
        { 0.5f, 0.0f, 0.0f},
        { 0.0f, 0.5f, 0.0f},
        { 0.0f, -0.5f, 0.0f },

    };


    Texture* blankteTexture = new Texture(Material(glm::vec3(0.1f), glm::vec3(0.5f),glm::vec3(0.8f), 32.0f));

    Texture* earthTexture = new Texture(Material(glm::vec3(0.1f), glm::vec3(0.6f), glm::vec3(0.8f), 32.0f), "../zpg/Objects/solarsystem/earth.jpg");

    for (auto& pos : positions) {
        DrawableObject* sphereObj = new DrawableObject(sph1, shaderProgram, blankteTexture);
        sphereObj->setObjectColor(glm::vec3(0.8));
        CompositeTransformation* t = new CompositeTransformation();
        t->add(new Scale(glm::vec3(0.1f)));
        t->add(new Translation(pos));
        sphereObj->setTransformation(t);

        this->addObject(sphereObj);
        camera->registerObserver(shaderProgram);
    }


    /*
    DrawableObject* sphereObj = new DrawableObject(sph1, shaderProgram, earthTexture);
    sphereObj->setObjectColor(glm::vec3(0.8));
    CompositeTransformation* t = new CompositeTransformation();
    t->add(new Scale(glm::vec3(0.1f)));
    t->add(new Translation(glm::vec3({ 0.0f, -0.5f, 0.0f })));
    sphereObj->setTransformation(t);

    this->addObject(sphereObj);
    camera->registerObserver(shaderProgram);

    */

    this->registerShaderToAllLightsAndNotify(shaderProgram);
    camera->notifyObservers();
    
}

