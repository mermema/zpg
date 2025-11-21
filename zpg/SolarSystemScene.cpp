#include "SolarSystemScene.h"
#include "SkyDome.h"



void SolarSystemScene::create()
{
    Camera* camera = new Camera(60.0f, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(10.0f, 20.0f, 0.0f));  
    camera->setTarget(glm::vec3(0.0f));
    this->setCamera(camera);

    size_t vertexCount = sizeof(sphere) / (6 * sizeof(float));
    static Model sph;
    sph.upload(sphere, vertexCount);

    ShaderLoader vertUV("../zpg/Shaders/uv.vert", GL_VERTEX_SHADER);
    ShaderLoader fragPhong("../zpg/Shaders/coloruni/phongmulti.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> phongShaders = { &vertUV, &fragPhong };
    ShaderProgram* shaderPrg = new ShaderProgram(phongShaders);



    ShaderLoader constantFrag("../zpg/Shaders/constanttexture.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> constantShaders = { &vertUV, &constantFrag };
    ShaderProgram* shaderPrgConstant = new ShaderProgram(constantShaders);

    ShaderLoader phongFrag("../zpg/Shaders/phongmultexture.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> phongMultShaders = { &vertUV, &phongFrag };
    ShaderProgram* shaderPrgPhong = new ShaderProgram(phongMultShaders);




    ShaderLoader skyDomeVert("../zpg/Shaders/skydome.vert", GL_VERTEX_SHADER);
    ShaderLoader skyDomeFrag("../zpg/Shaders/skydome.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> skyDomeShaders = { &skyDomeVert, &skyDomeFrag };
    ShaderProgram * skyDomeShader = new ShaderProgram(skyDomeShaders);

    SkyDome* skyBall = new SkyDome(skyDomeShader, "../zpg/Objects/solarsystem/planet.obj", "../zpg/Objects/solarsystem/stars.jpg" );
    this->setSkyObject(skyBall);






    auto sunLight = new PointLight(glm::vec3(0.0f), glm::vec3(1.0f, 0.9f, 0.7f), 1.0f);
    sunLight->setAttenuation(0, 0, 0);
    this->addLight(sunLight);

    ///// SUN
    
    
    Material defaultMaterial;
    Texture* sunTexture = new Texture(defaultMaterial, "../zpg/Objects/solarsystem/sun.jpg");
    ObjModel* planet = new ObjModel("../zpg/Objects/solarsystem/planet.obj");

    DrawableObject* sun = new DrawableObject(planet, shaderPrgConstant, sunTexture);
    CompositeTransformation* sunTrans = new CompositeTransformation();
    sunTrans->add(new Scale(glm::vec3(1.5f)));
    sun->setTransformation(sunTrans);
    this->addObject(sun);


    //// EARTH

    
    Texture* earthTexture = new Texture(defaultMaterial, "../zpg/Objects/solarsystem/earth.jpg");

    DrawableObject* earth = new DrawableObject(planet, shaderPrgPhong, earthTexture);
    CompositeTransformation* earthTrans = new CompositeTransformation();
    earthTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 5.0f));
    earthTrans->add(new Translation(glm::vec3(11.0f, 0, 0)));
    earthTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 0.5f));
    earth->setTransformation(earthTrans);
    this->addObject(earth);


    Texture* moonTexture = new Texture(defaultMaterial, "../zpg/Objects/solarsystem/moon.jpg");

    DrawableObject* moon = new DrawableObject(planet, shaderPrgPhong, moonTexture);
    CompositeTransformation* moonTrans = new CompositeTransformation();
    moonTrans->add(new Scale(glm::vec3(0.5)));

    moonTrans->add(new Translation(glm::vec3(2.0f, 0.5, 0)));
    moonTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 4.0f));

    moonTrans->add(new Translation(glm::vec3(11.0f, 0, 0)));
    moonTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 0.5f));
    moon->setTransformation(moonTrans);
    moon->setObjectColor(glm::vec3(0.5, 0.5, 0.5));

    this->addObject(moon);

    //Register
    camera->registerObserver(shaderPrgConstant);
    sunLight->registerObserver(shaderPrgConstant);
    camera->registerObserver(shaderPrgPhong);
    sunLight->registerObserver(shaderPrgPhong);
    camera->registerObserver(shaderPrg);
    sunLight->registerObserver(shaderPrg);
    camera->notifyObservers();
    sunLight->notifyObservers();








}