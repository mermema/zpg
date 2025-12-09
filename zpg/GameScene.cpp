#include "GameScene.h"

void GameScene::create()
{
    srand(time(NULL)); 

    ShaderLoader vert("../zpg/Shaders/uv.vert", GL_VERTEX_SHADER);
    ShaderLoader frag("../zpg/Shaders/phongmultexture.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> shaders = { &vert, &frag };
    textureshader = new ShaderProgram(shaders);

    this->enableMouseActions();
    Camera* camera = new Camera(60.0f, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0.0f, 2.0f, 12.0f));
    camera->setTarget(glm::vec3(0.0f, 0.1f, 0.0f));
    this->setCamera(camera);

    ShaderLoader skyDomeVert("../zpg/Shaders/skydome.vert", GL_VERTEX_SHADER);
    ShaderLoader skyDomeFrag("../zpg/Shaders/skydome.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> skyDomeShaders = { &skyDomeVert, &skyDomeFrag };
    ShaderProgram* skyDomeShader = new ShaderProgram(skyDomeShaders);
    SkyDome* skyDome = new SkyDome(skyDomeShader, "../zpg/Objects/sky/skydome.obj", "../zpg/Objects/sky/dome2final.png");
    this->setSkyObject(skyDome);

    auto dirlight = new DirectionalLight(glm::vec3(0, -1, -1));
    dirlight->setIntensity(1);
    this->addLight(dirlight);

    const float plainUV[] = {
        //pos norm uv
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1000.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1000.0f, 1000.0f,

       -1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1000.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1000.0f, 1000.0f
    };    
    
    Model* planeModel = new Model();
    planeModel->uploadWithUV(plainUV, sizeof(plainUV) / (8 * sizeof(float)));

    Material defaultMaterial;
    Texture* grassTexture = new Texture(defaultMaterial, "../zpg/Objects/grass.png");

    DrawableObject* planeObj = new DrawableObject(planeModel, textureshader, grassTexture);
    CompositeTransformation* plainTrans = new CompositeTransformation();
    plainTrans->add(new Scale(glm::vec3(1000, 1, 1000)));
    planeObj->setTransformation(plainTrans);
    planeObj->setNotMovable();
    this->addObject(planeObj);

    models.push_back(new ObjModel("../zpg/Objects/shrek.obj"));
    textures.push_back(new Texture(Material(), "../zpg/Objects/shrek.png"));

    models.push_back(new ObjModel("../zpg/Objects/fiona.obj"));
    textures.push_back(new Texture(Material(), "../zpg/Objects/fiona.png"));

    models.push_back(new ObjModel("../zpg/Objects/cat.obj"));
    textures.push_back(new Texture(Material(), "../zpg/Objects/cat.jpg"));

    models.push_back(new ObjModel("../zpg/Objects/toiled.obj"));
    textures.push_back(new Texture(Material(), "../zpg/Objects/toiled.jpg"));



    spawnRandomObject();
    camera->registerObserver(textureshader);
    dirlight->registerObserver(textureshader);
    camera->notifyObservers();
    dirlight->notifyObservers();
}




DrawableObject* GameScene::createObjectByIndex(int index)
{
    auto model = models[index];
    auto tex = textures[index];

    auto obj = new DrawableObject(model, textureshader, tex, false);
    obj->setStencilID(255);


    if (index == 2)//cat
    {
    
        auto t = new CompositeTransformation();
        t->add(new Scale(glm::vec3(0.03)));
        t->add(new Rotation(glm::radians(-90.0f), glm::vec3(1, 0, 0)));
        obj->setTransformation(t);
    }
    return obj;
}



void GameScene::spawnRandomObject()
{


    int type = rand() % 4;
    DrawableObject* obj = createObjectByIndex(type);

    float x = (rand() % 16) - 8;
    float z = (rand() % 16) - 8;

    glm::vec3 v1(rand() % 18 - 9, 0, rand() % 18 - 9);;


    if (!obj->getTransformation())
    {
        obj->setTransformation(new CompositeTransformation());
    }

    float speed = 0.25 + (rand() % 1000) / 1000.0f;
    obj->getTransformation()->add(new Scale(glm::vec3(2)));

    if (rand() % 2)
    {
        obj->getTransformation()->add(new DynamicTranslation(glm::vec3(0), v1, speed));
    }
    else
    {
        glm::vec3 v2(rand() % 18 - 9, 0, rand() % 18 - 9);
        obj->getTransformation()->add(new DynamicTranslationTwoSegment(glm::vec3(0), v1, v2,speed));
    }

    
    addObject(obj);
    activeObject = obj;

    lastSpawnTime = glfwGetTime();
}




void GameScene::draw()
{
    double now = glfwGetTime();

    if (!activeObject)
    {
        if (now - lastSpawnTime >= spawnInterval)
        {
            spawnRandomObject();
            spawnInterval = (rand() % 2000) / 1000.0; 
        }
    }

    Scene::draw();
}




void GameScene::handleMouseClick(double x, double y) {
    if (!camera || !isMouseSensitive) return;

    GLint viewport[4];
    GLfloat depth;
    GLuint stencilIndex;

    glGetIntegerv(GL_VIEWPORT, viewport);

    //revers y axis 
    GLint glY = viewport[3] - (GLint)y - 1;

    //load into buffers
    glReadPixels((GLint)x, glY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels((GLint)x, glY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilIndex);


    //save stencilid 
    selectedObjectID = stencilIndex;

   if (activeObject && activeObject->getID() == stencilIndex)
        {
            removeObjectByID(stencilIndex);
            activeObject = nullptr;

            float reaction = glfwGetTime() - lastSpawnTime;
            int points = 0;
            if (reaction < 0.25)
                points = 9;     
            else if (reaction < 0.75)
                points = 6;
            else if (reaction < 1.5)
                points = 3;
            else
                points = 0;
            score += points;
            std::cout << "Object catched! \t Score: " << points << "\t  Total Score = " << score << std::endl;
            lastSpawnTime = glfwGetTime();
        }
    }

