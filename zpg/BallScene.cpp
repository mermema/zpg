#include "BallScene.h"


void BallScene::create()
{


    static ShaderLoader vert("../zpg/Shaders/uni.vert", GL_VERTEX_SHADER);
    static ShaderLoader frag("../zpg/Shaders/coloruni/phongmulti.frag", GL_FRAGMENT_SHADER);

    std::vector<Shader*> shaders = { &vert, &frag };


    static ShaderProgram shaderProgram(shaders);

    Camera* camera = new Camera(60, 0.1f, 100.0f);
    this->setCamera(camera);
    size_t vertexCount = sizeof(sphere) / (6 * sizeof(float));
    sph1.upload(sphere, vertexCount);
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
        { 0.0f,-0.5f, 0.0f},
        // { 0.0f,-0.0f, 0.0f}
    };

    for (auto& pos : positions) {
        DrawableObject* sphereObj = new DrawableObject(&sph1, &shaderProgram);
        sphereObj->setObjectColor(glm::vec3(0.8));
        CompositeTransformation* t = new CompositeTransformation();
        t->add(new Scale(glm::vec3(0.1f)));
        t->add(new Translation(pos));
        sphereObj->setTransformation(t);

        this->addObject(sphereObj);
        camera->registerObserver(&shaderProgram);
    }
    this->registerShaderToAllLightsAndNotify(&shaderProgram);
    camera->notifyObservers();
    
}

