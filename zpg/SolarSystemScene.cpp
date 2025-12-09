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
    ShaderProgram* skyDomeShader = new ShaderProgram(skyDomeShaders);

    SkyDome* skyBall = new SkyDome(skyDomeShader, "../zpg/Objects/solarsystem/planet.obj", "../zpg/Objects/solarsystem/stars.jpg");
    this->setSkyObject(skyBall);






    auto sunLight = new PointLight(glm::vec3(0.0f), glm::vec3(1.0f, 0.9f, 0.7f), 1.0f);
    sunLight->setAttenuation(0, 0, 0);
    this->addLight(sunLight);

    Material defaultMaterial;


    /////SUN


    Texture* sunTexture = new Texture(defaultMaterial, "../zpg/Objects/solarsystem/sun.jpg");
    ObjModel* planet = new ObjModel("../zpg/Objects/solarsystem/planet.obj");

    DrawableObject* sun = new DrawableObject(planet, shaderPrgConstant, sunTexture);
    CompositeTransformation* sunTrans = new CompositeTransformation();
    sunTrans->add(new Scale(glm::vec3(1.5f)));
    sun->setTransformation(sunTrans);
    this->addObject(sun);


    //////EARTH


    Texture* earthTexture = new Texture(defaultMaterial, "../zpg/Objects/solarsystem/earth.jpg");

    auto earthTranslation = new Translation(glm::vec3(11.0f, 0, 0));
    auto earthRotationAroundSun = new DynamicRotation(glm::vec3(0, 1, 0), 0.5f);

    DrawableObject* earth = new DrawableObject(planet, shaderPrgPhong, earthTexture);
    CompositeTransformation* earthTrans = new CompositeTransformation();
    earthTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 5.0f));
    earthTrans->add(earthTranslation);
    earthTrans->add(earthRotationAroundSun);
    earth->setTransformation(earthTrans);
    this->addObject(earth);



    /// login
    Texture* testTexture = new Texture(Material(glm::vec3(0.1,0.1,0.1), glm::vec3(0.3,0.9,0.3), glm::vec3(1), 32));
    ObjModel* login = new ObjModel("../zpg/Objects/mer0126.obj");
    DrawableObject* LoginObj = new DrawableObject(login, shaderPrgPhong, testTexture);

    CompositeTransformation* loginTrans = new CompositeTransformation();
    // loginTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 5.0f));
    loginTrans->add(new Rotation(glm::radians(90.0), glm::vec3(1, 0, 0)));
    loginTrans->add(new Rotation(glm::radians(180.0), glm::vec3(0, 0, 1)));
    loginTrans->add(new Scale(glm::vec3(0.3)));

    // rotation around earth (around genessis rn)
    loginTrans->add(new Translation(glm::vec3(4.0f, 0.0, 0)));
    loginTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), -4.0f));

    loginTrans->add(earthTranslation);
    loginTrans->add(earthRotationAroundSun);
    LoginObj->setTransformation(loginTrans);
    LoginObj->setObjectColor(glm::vec3(0, 0.8, 0));
    this->addObject(LoginObj);


    ////MOON
    Texture* moonTexture = new Texture(defaultMaterial, "../zpg/Objects/solarsystem/moon.jpg");

    DrawableObject* moon = new DrawableObject(planet, shaderPrgPhong, moonTexture);
    CompositeTransformation* moonTrans = new CompositeTransformation();
    moonTrans->add(new Scale(glm::vec3(0.5)));

    moonTrans->add(new Translation(glm::vec3(2.0f, 0.5, 0)));
    moonTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 4.0f));

    moonTrans->add(earthTranslation);
    moonTrans->add(earthRotationAroundSun);
    moon->setTransformation(moonTrans);

    this->addObject(moon);

    ///MERCUR
    Texture* mercurTexture = new Texture(defaultMaterial, "../zpg/Objects/solarsystem/mercur.jpg");

    DrawableObject* mercur = new DrawableObject(planet, shaderPrgPhong, mercurTexture);
    CompositeTransformation* mercurTrans = new CompositeTransformation();
    mercurTrans->add(new Scale(glm::vec3(0.4f)));
    mercurTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 8.0f));
    mercurTrans->add(new Translation(glm::vec3(3.5f, 0, 0)));
    mercurTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 2.0f));
    mercur->setTransformation(mercurTrans);
    this->addObject(mercur);

    ///VENUS
    Texture* venusTexture = new Texture(defaultMaterial, "../zpg/Objects/solarsystem/venus.jpg");

    DrawableObject* venus = new DrawableObject(planet, shaderPrgPhong, venusTexture);
    CompositeTransformation* venusTrans = new CompositeTransformation();
    venusTrans->add(new Scale(glm::vec3(1.1f)));
    venusTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 3.0f));
    venusTrans->add(new Translation(glm::vec3(6.5f, 0, 0)));
    venusTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 1.2f));
    venus->setTransformation(venusTrans);
    this->addObject(venus);

    //MARS
    Texture* marsTexture = new Texture(defaultMaterial, "../zpg/Objects/solarsystem/mars.jpg");

    DrawableObject* mars = new DrawableObject(planet, shaderPrgPhong, marsTexture);
    CompositeTransformation* marsTrans = new CompositeTransformation();
    marsTrans->add(new Scale(glm::vec3(0.8f)));
    marsTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 6.0f));

    auto marsTranslation = new Translation(glm::vec3(15.0f, 0, 0));
    auto marsRotation = new DynamicRotation(glm::vec3(0, 1, 0), 0.4f);
    marsTrans->add(marsTranslation);
    marsTrans->add(marsRotation);

    mars->setTransformation(marsTrans);
    this->addObject(mars);

    //PHOBOS mars moon
    DrawableObject* phobos = new DrawableObject(planet, shaderPrgPhong, moonTexture);
    CompositeTransformation* phobosTrans = new CompositeTransformation();
    phobosTrans->add(new Scale(glm::vec3(0.25f)));

    phobosTrans->add(new Translation(glm::vec3(1.5f, 0, 0)));
    phobosTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 8.0f));

    phobosTrans->add(marsTranslation);
    phobosTrans->add(marsRotation);

    phobos->setTransformation(phobosTrans);
    this->addObject(phobos);

    //DEIMOS also mars moon
    DrawableObject* deimos = new DrawableObject(planet, shaderPrgPhong, moonTexture);
    CompositeTransformation* deimosTrans = new CompositeTransformation();
    deimosTrans->add(new Scale(glm::vec3(0.15f)));
    deimosTrans->add(new Translation(glm::vec3(2.5f, 0, 0)));
    deimosTrans->add(new DynamicRotation(glm::vec3(0, 1, 0), 4.0f));

    deimosTrans->add(marsTranslation);
    deimosTrans->add(marsRotation);

    deimos->setTransformation(deimosTrans);
    this->addObject(deimos);



    //register
    camera->registerObserver(shaderPrgConstant);
    sunLight->registerObserver(shaderPrgConstant);
    camera->registerObserver(shaderPrgPhong);
    sunLight->registerObserver(shaderPrgPhong);
    camera->registerObserver(shaderPrg);
    sunLight->registerObserver(shaderPrg);
    camera->notifyObservers();
    sunLight->notifyObservers();








}