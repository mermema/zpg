#include "ForrestScene.h"
#include "ObjModel.h"
#include <unordered_set>

void ForrestScene::create()
{
    const float plainUV[] = {
        //vrchol, normála, uv souøadnice
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 10.0f,

       -1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 10.0f
    };
    this->enableMouseActions();

    static Model modeltree;
    static Model medelbush;
    static Model sph1;
    size_t vertexCount = sizeof(sphere) / (6 * sizeof(float));
    sph1.upload(sphere, vertexCount);


    static ShaderLoader vert("../zpg/Shaders/uni.vert", GL_VERTEX_SHADER);
    static ShaderLoader frag("../zpg/Shaders/phongmulti.frag", GL_FRAGMENT_SHADER);
    static ShaderLoader fragConstant("../zpg/Shaders/coloruni/constant.frag", GL_FRAGMENT_SHADER);

    static ShaderLoader textureVert("../zpg/Shaders/uv.vert", GL_VERTEX_SHADER);
    static ShaderLoader textureFrag("../zpg/Shaders/phongmultexture.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> shaders = { &textureVert, &textureFrag };
    static ShaderProgram textureshader(shaders);


    shaders = { &vert, &frag };

    static ShaderProgram shaderProgram(shaders);

    shaders = { &vert, &fragConstant };
    static ShaderProgram fireflyshader(shaders);


    Camera* camera = new Camera(60.0f, 0.1f, 100.0f);
    camera->setPosition(glm::vec3(0.0f, 0.5f, 8.0f));
    camera->setTarget(glm::vec3(0.0f));
    this->setCamera(camera);

    auto* mainLight = new DirectionalLight(glm::vec3(1, -1, 1), glm::vec3(1.0f, 0.9f, 0.7f), 0.1f);

    this->addLight(mainLight);


    auto* flashlight = new Flashlight(camera);
    this->addLight(flashlight);

    std::vector<Light*> allLights;
    allLights.push_back(mainLight);
    allLights.push_back(flashlight);
    for (int i = 0; i < 10; i++) {
        float posX = (rand() % 400 - 200) / 10.0f;
        float posZ = (rand() % 400 - 200) / 10.0f;
        float posY = 2.0f;

        auto* fireflyLight = new Light(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1, 1, 1),
            0.2f 
        );
        fireflyLight->setAttenuation(0.2, 0.2, 0.5);
        CompositeTransformation* t = new CompositeTransformation();
        t->add(new Scale(glm::vec3(0.1f)));
        t->add(new Translation(glm::vec3(posX, posY, posZ)));
        t->add(new RandomTranslation(2.0f));


        Firefly* firefly = new Firefly(&sph1, &fireflyshader, fireflyLight, t, glm::vec3(1, 1, 1));
        firefly->addToScene(this);
        allLights.push_back(fireflyLight);
   
    }
    vertexCount = sizeof(plainUV) / (8 * sizeof(float));
    Model* planeModel = new Model();

    Material defaultMaterial

    (

        glm::vec3(0.01f),
        glm::vec3(0),
        glm::vec3(0),     
        32.0f               
        );

    Texture* grassTexture = new Texture(defaultMaterial, "../zpg/Objects/grass.png");

    planeModel->uploadWithUV(plainUV, vertexCount);
    CompositeTransformation* planeScale = new CompositeTransformation();
    planeScale->add(new Scale(glm::vec3(30, 1, 30)));
    DrawableObject* planeobject = new DrawableObject(planeModel, &textureshader, grassTexture);
    planeobject->setTransformation(planeScale);
    this->addObject(planeobject);




    vertexCount = sizeof(tree) / (6 * sizeof(float));
    modeltree.upload(tree, vertexCount);

    int gridSize = 9;
    float spacing = 5.0f;
    float startOffset = -(gridSize - 1) * spacing * 0.5f;

    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            DrawableObject* bushObj = new DrawableObject(&modeltree, &shaderProgram);
            CompositeTransformation* t = new CompositeTransformation();
            t->add(new Scale(glm::vec3(0.5f)));

            float baseX = startOffset + col * spacing;
            float baseZ = startOffset + row * spacing;

            float randomOffsetX = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 2.5f;
            float randomOffsetZ = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 2.5f;

            t->add(new Translation(glm::vec3(baseX + randomOffsetX, 0, baseZ + randomOffsetZ)));
            bushObj->setTransformation(t);
            this->addObject(bushObj);
        }
    }


    vertexCount = sizeof(bushes) / (6 * sizeof(float));
    medelbush.upload(bushes, vertexCount);

    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            DrawableObject* bushObj = new DrawableObject(&medelbush, &shaderProgram);
            CompositeTransformation* t = new CompositeTransformation();
            t->add(new Scale(glm::vec3(2.0f)));

            float baseX = startOffset+ col * spacing;
            float baseZ = startOffset+ row * spacing;

            float randomOffsetX = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 2.5f;
            float randomOffsetZ = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 2.5f;

            t->add(new Translation(glm::vec3(baseX + randomOffsetX, 0, baseZ + randomOffsetZ)));
            bushObj->setTransformation(t);
            this->addObject(bushObj);
        }
    }

    ///fiona shrek cat toilet
    Texture* shrekTexture = new Texture(defaultMaterial, "../zpg/Objects/shrek.png");


    static ObjModel shrek("../zpg/Objects/shrek.obj");
    DrawableObject* shrekObject = new DrawableObject(&shrek, &textureshader, shrekTexture);
    shrekObject->setObjectColor(glm::vec3(0, 1, 0));
    this->addObject(shrekObject);

    Texture* fionaTexture = new Texture(defaultMaterial, "../zpg/Objects/fiona.png");


    static ObjModel fiona("../zpg/Objects/fiona.obj");
    DrawableObject* fionaObject = new DrawableObject(&fiona, &textureshader, fionaTexture);
    CompositeTransformation* fionaTrans = new CompositeTransformation();
    fionaTrans->add(new Translation(glm::vec3(2, 0, 0)));
    fionaObject->setTransformation(fionaTrans);
    this->addObject(fionaObject);

    Texture* toiletTexture = new Texture(defaultMaterial, "../zpg/Objects/toiled.jpg");
    static ObjModel toilet("../zpg/Objects/toiled.obj");
    DrawableObject* toiletObject = new DrawableObject(&toilet, &textureshader, toiletTexture);
    CompositeTransformation* toiletTrans = new CompositeTransformation();
    toiletTrans->add(new Translation(glm::vec3(0, 0, -3)));
    toiletObject->setTransformation(toiletTrans);
    this->addObject(toiletObject);

    Texture* catTexture = new Texture(defaultMaterial, "../zpg/Objects/cat.jpg");
    static ObjModel cat("../zpg/Objects/cat.obj");
    DrawableObject* catObject = new DrawableObject(&cat, &textureshader, catTexture);
    CompositeTransformation* catTrans = new CompositeTransformation();
    catTrans->add(new Rotation(-3.1415926 / 2, glm::vec3(1, 0, 0)));
    catTrans->add(new Scale(glm::vec3(0.03)));
    catTrans->add(new Translation(glm::vec3(-2, 0, 0)));
    catObject->setTransformation(catTrans);
    this->addObject(catObject);




    //registration
    this->registerAllShadersToAllLightsAndNotify();



    this->registerAllObjectsToCameraAndNotify();
    this->enableFlashlight();




    }
