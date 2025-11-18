#include "SolarSystemScene.h"
#include "./Models/sphere.h"
#include "Scale.h"
#include "DynamicRotation.h"
#include "ShaderLoader.h"


void SolarSystemScene::create()
{
    Camera* camera = new Camera(60.0f, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0.0f, 15.0f, 0.0f));  // ?? níž a dál
    camera->setTarget(glm::vec3(0.0f));
    this->setCamera(camera);

    size_t vertexCount = sizeof(sphere) / (6 * sizeof(float));
    static Model sph;
    sph.upload(sphere, vertexCount);

    ShaderLoader vertPhong("../zpg/Shaders/uni.vert", GL_VERTEX_SHADER);
    ShaderLoader fragPhong("../zpg/Shaders/coloruni/phongmulti.frag", GL_FRAGMENT_SHADER);

    std::vector<Shader*> phongShaders = { &vertPhong, &fragPhong };
    static ShaderProgram shaderPrg(phongShaders);

    auto sunLight = new Light(glm::vec3(0.0f), glm::vec3(1.0f, 0.9f, 0.7f), 1.0f);
    sunLight->setAttenuation(0, 0, 0);
    this->addLight(sunLight);

    DrawableObject* sun = new DrawableObject(&sph, &shaderPrg);
    CompositeTransformation* sunTrans = new CompositeTransformation();
    sunTrans->add(new Scale(glm::vec3(1.5f)));
    sun->setTransformation(sunTrans);
    sun->setObjectColor(glm::vec3(1, 0.2, 0.2));
    this->addObject(sun);

    DrawableObject* earth = new DrawableObject(&sph, &shaderPrg);
    CompositeTransformation* earthTrans = new CompositeTransformation();
    earthTrans->add(new Translation(glm::vec3(8.0f, 0, 0)));
    earthTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 0.5f));
    earth->setTransformation(earthTrans);
    earth->setObjectColor(glm::vec3(0, 0.5, 0.5));
    this->addObject(earth);

    DrawableObject* moon = new DrawableObject(&sph, &shaderPrg);
    CompositeTransformation* moonTrans = new CompositeTransformation();
    moonTrans->add(new Scale(glm::vec3(0.5)));

    moonTrans->add(new Translation(glm::vec3(4.0f, 0, 0)));
    moonTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 4.0f));

    moonTrans->add(new Translation(glm::vec3(8.0f, 0, 0)));
    moonTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 0.5f));




    moon->setTransformation(moonTrans);
    moon->setObjectColor(glm::vec3(0.5, 0.5, 0.5));

    this->addObject(moon);

    //Register
    camera->registerObserver(&shaderPrg);
    sunLight->registerObserver(&shaderPrg);
    camera->notifyObservers();
    sunLight->notifyObservers();








}