#include "ReflectorTestScene.h"

void ReflectorTestScene::create()
{

    ShaderLoader vertPhong("../zpg/Shaders/uni.vert", GL_VERTEX_SHADER);
    ShaderLoader fragPhong("../zpg/Shaders/coloruni/phongmulti.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> phongShaders = { &vertPhong, &fragPhong };
    ShaderProgram* phongShader = new ShaderProgram(phongShaders);


    Camera* camera = new Camera(60.0f, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0.0f, 0.1f, -3.0f));
    camera->setTarget(glm::vec3(0.0f, 0.1f, 0.0f));
    this->setCamera(camera);

    Model* sph1 = new Model();
    size_t vertexCount = sizeof(sphere) / (6 * sizeof(float));
    sph1->upload(sphere, vertexCount);

    auto reflector = new ReflectorLight(glm::vec3(0, 1.5, 0), glm::vec3(0, -1, 0));
    reflector->setAttenuation(0, 0, 0);
    reflector->setAngle(10);
    this->addLight(reflector);


    static Model planeModel;
    vertexCount = sizeof(plain) / (6 * sizeof(float));
    planeModel.upload(plain, vertexCount);

    DrawableObject* planeObj = new DrawableObject(&planeModel, phongShader);
    CompositeTransformation* plainTrans = new CompositeTransformation();
    plainTrans->add(new Scale(glm::vec3(10, 1, 10)));
    planeObj->setObjectColor(glm::vec3(0, 1, 0));
    this->addObject(planeObj);


    DrawableObject* ball1 = new DrawableObject(sph1, phongShader);
    CompositeTransformation* ball1Trans = new CompositeTransformation();
    ball1Trans->add(new Scale(glm::vec3(0.08f)));
    ball1->setTransformation(ball1Trans);
    ball1->setObjectColor(glm::vec3(0.5, 0.5, 0.5));
    this->addObject(ball1);

    DrawableObject* ball2 = new DrawableObject(sph1, phongShader);
    CompositeTransformation* ball2Trans = new CompositeTransformation();
    ball2Trans->add(new Scale(glm::vec3(0.08f)));
    ball2Trans->add(new Translation(glm::vec3(-0.2, 0.2, 0)));

    ball2->setTransformation(ball2Trans);
    ball2->setObjectColor(glm::vec3(0.5, 0.5, 0.5));
    this->addObject(ball2);

    DrawableObject* ball3 = new DrawableObject(sph1, phongShader);
    CompositeTransformation* ball3Trans = new CompositeTransformation();
    ball3Trans->add(new Scale(glm::vec3(0.08f)));
    ball3Trans->add(new Translation(glm::vec3(+0.2, 0.2, 0)));

    ball3->setTransformation(ball3Trans);
    ball3->setObjectColor(glm::vec3(0.5, 0.5, 0.5));
    this->addObject(ball3);

    DrawableObject* ball4 = new DrawableObject(sph1, phongShader);
    CompositeTransformation* ball4Trans = new CompositeTransformation();
    ball4Trans->add(new Scale(glm::vec3(0.08f)));
    ball4Trans->add(new Translation(glm::vec3(-0.0, 0.5, 0)));

    ball4->setTransformation(ball4Trans);
    ball4->setObjectColor(glm::vec3(0.5, 0.5, 0.5));
    this->addObject(ball4);

    camera->registerObserver(phongShader);
    reflector->registerObserver(phongShader);
    camera->notifyObservers();
    reflector->notifyObservers();

}
