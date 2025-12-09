#include "SkyBoxScene.h"


void SkyBoxScene::create()
{

    const float plainUV[] = {
        //pos norm UV
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 10.0f,

       -1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 10.0f
    };

    ShaderLoader vert("../zpg/Shaders/uv.vert", GL_VERTEX_SHADER);
    ShaderLoader frag("../zpg/Shaders/phongmultexture.frag", GL_FRAGMENT_SHADER);

    std::vector<Shader*> shaders = { &vert, &frag };


    ShaderProgram* textureshader = new ShaderProgram(shaders);

    ShaderLoader skyboxVert("../zpg/Shaders/skybox.vert", GL_VERTEX_SHADER);
    ShaderLoader skyboxFrag("../zpg/Shaders/skybox.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> skyboxShaders = { &skyboxVert, &skyboxFrag };
    ShaderProgram* skyboxShader = new ShaderProgram(skyboxShaders);

    /*
    ShaderLoader skyDomeVert("../zpg/Shaders/skydome.vert", GL_VERTEX_SHADER);
    ShaderLoader skyDomeFrag("../zpg/Shaders/skydome.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> skyDomeShaders = { &skyDomeVert, &skyDomeFrag };
    ShaderProgram skyDomeShader(skyDomeShaders);
    */

    this->enableMouseActions();
    Camera* camera = new Camera(60.0f, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0.0f, 2.0f, 3.0f));
    camera->setTarget(glm::vec3(0.0f, 0.1f, 0.0f));
    this->setCamera(camera);


    std::vector<std::string> skyboxFaces = {
"../zpg/Objects/sky/posx.jpg",   // +X
"../zpg/Objects/sky/negx.jpg",    // -X
"../zpg/Objects/sky/posy.jpg",    // +Y  
"../zpg/Objects/sky/negy.jpg",  // -Y
"../zpg/Objects/sky/posz.jpg",   // +Z
"../zpg/Objects/sky/negz.jpg",   // -Z
    };


    SkyBox* skybox = new SkyBox(skyboxShader, skyboxFaces);
    this->setSkyObject(skybox);


    //SkyDome* skyDome = new SkyDome(&skyDomeShader,"../zpg/Objects/sky/skydome.obj","../zpg/Objects/sky/skydome.png" );
    //scene->setSkyObject(skyDome);

    auto dirlight = new DirectionalLight(glm::vec3(0, -1, -1));
    dirlight->setIntensity(1);
    //dirlight->setAttenuation(0, 0, 0);
    this->addLight(dirlight);

    Material woodMaterial(

        glm::vec3(0.1),
        glm::vec3(0, 1, 0),
        glm::vec3(0.1),
        32.0f
    );

    Material defaultMaterial;


    Texture* woodTexture = new Texture(woodMaterial, "../zpg/Objects/wooden_fence.png");


    Model* planeModel = new Model();
    size_t vertexCount = sizeof(plainUV) / (8 * sizeof(float));
    planeModel->uploadWithUV(plainUV, vertexCount);

    DrawableObject* planeObj = new DrawableObject(planeModel, textureshader, woodTexture);
    CompositeTransformation* plainTrans = new CompositeTransformation();
    plainTrans->add(new Scale(glm::vec3(10, 1, 10)));
    planeObj->setTransformation(plainTrans);
    planeObj->setObjectColor(glm::vec3(0, 1, 0));
    planeObj->setNotMovable();
    this->addObject(planeObj);


    Material shrekMaterial;
    shrekMaterial.loadFromMTL("../zpg/Objects/shrek.mtl");
    Texture* shrekTexture = new Texture(shrekMaterial, "../zpg/Objects/shrek.png");


    ObjModel* shrek = new ObjModel("../zpg/Objects/shrek.obj");
    DrawableObject* shrekObject = new DrawableObject(shrek, textureshader, shrekTexture);
    shrekObject->setObjectColor(glm::vec3(0, 1, 0));
    this->addObject(shrekObject);

    Texture* fionaTexture = new Texture(defaultMaterial, "../zpg/Objects/fiona.png");


    ObjModel* fiona = new ObjModel("../zpg/Objects/fiona.obj");
    DrawableObject* fionaObject = new DrawableObject(fiona, textureshader, fionaTexture);
    CompositeTransformation* fionaTrans = new CompositeTransformation();
    fionaTrans->add(new Translation(glm::vec3(2, 0, 0)));
    fionaObject->setTransformation(fionaTrans);
    this->addObject(fionaObject);

    Texture* toiletTexture = new Texture(defaultMaterial, "../zpg/Objects/toiled.jpg");
    ObjModel* toilet = new ObjModel("../zpg/Objects/toiled.obj");
    DrawableObject* toiletObject = new DrawableObject(toilet, textureshader, toiletTexture);
    CompositeTransformation* toiletTrans = new CompositeTransformation();
    toiletTrans->add(new Translation(glm::vec3(0, 0, -3)));
    toiletObject->setTransformation(toiletTrans);
    this->addObject(toiletObject);

    Texture* catTexture = new Texture(defaultMaterial, "../zpg/Objects/cat.jpg");
    ObjModel* cat = new ObjModel("../zpg/Objects/cat.obj");
    DrawableObject* catObject = new DrawableObject(cat, textureshader, catTexture);
    CompositeTransformation* catTrans = new CompositeTransformation();
    catTrans->add(new Rotation(-3.1415926 / 2, glm::vec3(1, 0, 0)));
    catTrans->add(new Scale(glm::vec3(0.03)));
    catTrans->add(new Translation(glm::vec3(-2, 0, 0)));
    catObject->setTransformation(catTrans);
    this->addObject(catObject);

    Model* sphereModel = new Model();
    sphereModel->upload(sphere, sizeof(sphere) / (6 * sizeof(float)));
    this->setClickCreateModel(sphereModel);
    this->setClickCreateShader(textureshader);





    camera->registerObserver(textureshader);
    //camera->registerObserver(&skyDomeShader);

    dirlight->registerObserver(textureshader);
    camera->notifyObservers();
    dirlight->notifyObservers();




}